#include <algorithm>
#include <array>
#include <cmath>
#include <complex>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include "fftw3.h"
#include "sndfile.h"
#include "vocoder.hpp"
#include "vocoder_types.hpp"
#include "util_math.hpp"
#include <numbers>

vocoder::vocoder (const voc_args &init_args) : user_args(init_args) { }

vocoder::vocoder (const voc_args &init_args, int frame_size) : user_args(init_args), frame_size(frame_size) { }

vocoder::~vocoder() {
    delete [] inbuff;
    delete [] outbuff;
    delete [] window_hann;
    delete [] prev_phase;
    delete [] prev_synth_phase;
    delete pitchfind;
    sf_close(input_fh);
    sf_close(output_fh);
    fftw_destroy_plan(fft);
    fftw_destroy_plan(ifft);
    fftw_free(fftw_input);
    fftw_free(fftw_output);
}

vocoder::status vocoder::vocoder_init() {
    input_fh  = sf_open(user_args.input_filename.c_str(), SFM_READ, &file_data);
    output_fh = sf_open(user_args.output_filename.c_str(), SFM_WRITE, &file_data);
    if (input_fh == NULL || output_fh == NULL) {
        return status::FILE_READ_FAIL;
    }
    inbuff   = new dtype[frame_size]();
    outbuff  = new dtype[outbuff_size]();
    prev_phase = new dtype[frame_size]();
    prev_synth_phase = new dtype[frame_size]();
    // FFTW Initializations
    fftw_input = (complex*) fftw_malloc(sizeof(fftw_complex) * frame_size);
    fftw_output = (complex*) fftw_malloc(sizeof(fftw_complex) * frame_size);
    fft = fftw_plan_dft_1d(frame_size, reinterpret_cast<fftw_complex*>(fftw_input),
                         reinterpret_cast<fftw_complex*>(fftw_output), FFTW_FORWARD, FFTW_MEASURE);
    ifft = fftw_plan_dft_1d(frame_size, reinterpret_cast<fftw_complex*>(fftw_output),
                         reinterpret_cast<fftw_complex*>(fftw_input), FFTW_BACKWARD, FFTW_MEASURE);

    window_hann = new dtype[frame_size];
    math::compute_hann_win(window_hann, frame_size, analysis_hop_size);
    if (user_args.sel_effect != ROBOT) {
        synthesis_hop_size = analysis_hop_size * user_args.mod_factor.first / user_args.mod_factor.second;
        std::cout << "Synthesis Hopsize: " << synthesis_hop_size << '\n';
    }
    return read_samples(inbuff, 0, frame_size);
}

vocoder::status vocoder::analysis() {
    // Left-shift input buffer to load 'analysis_hop_size' new samples
    std::copy(inbuff + analysis_hop_size, inbuff + frame_size, inbuff);
    status ret_status = read_samples(inbuff, frame_size - analysis_hop_size, analysis_hop_size);
    // Copy input buffer to fft input
    std::copy_n(inbuff, frame_size, fftw_input);
    for (int i = 0; i < frame_size; ++i) {
        fftw_input[i].real(fftw_input[i].real() * window_hann[i]);
    }
    fftw_execute(fft);
    for (int i = 0; i < frame_size; ++i) {
        fftw_output[i] = complex(abs(fftw_output[i]), arg(fftw_output[i]));
    }
    return ret_status;
}

vocoder::status vocoder::modify_phase_r() {
    for(int i = 0; i < frame_size; ++i) {
        fftw_output[i].imag(0);
    }
    return status::SUCCESS;
}

vocoder::status vocoder::modify_phase_t() {
    dtype bin_freqs[frame_size]; dtype phase_adv[frame_size];
    dtype inter_result[frame_size];
    // Constants
    for (int i = 0; i < frame_size; ++i) {
        bin_freqs[i] = 2 * std::numbers::pi * i / frame_size;
        phase_adv[i] = bin_freqs[i] * analysis_hop_size;
    }
    // Wrapped phase advance
    for (int i = 0; i < frame_size; ++i) {
        inter_result[i] = fftw_output[i].imag() - prev_phase[i] - phase_adv[i];
        inter_result[i] = inter_result[i] - 2 * std::numbers::pi * 
                        std::floor((inter_result[i] + std::numbers::pi) / (2 * std::numbers::pi));
        prev_phase[i] = fftw_output[i].imag();
    }
    // Instantaneous frequency
    for (int i = 0; i < frame_size; ++i) {
        inter_result[i] = bin_freqs[i] + inter_result[i] / analysis_hop_size;
    }
    // Phase propagation and synthesis
    for (int i = 0; i < frame_size; ++i) {
        inter_result[i] = prev_synth_phase[i] + synthesis_hop_size * inter_result[i];
        fftw_output[i].imag(inter_result[i]);
        prev_synth_phase[i] = inter_result[i];
    }
    return status::SUCCESS;
}

vocoder::status vocoder::resynthesis() {
    for (int i = 0; i < frame_size; ++i) {
        fftw_output[i] = std::polar(fftw_output[i].real(), fftw_output[i].imag());
    }
    fftw_execute(ifft);
    for (int i = 0; i < frame_size; ++i) {
        outbuff[outbuff_offset + i] += fftw_input[i].real() * window_hann[i] / frame_size;
    }
    outbuff_offset += synthesis_hop_size;
    if (outbuff_offset >= frame_size) {
        sf_write_double(output_fh, outbuff, frame_size);
        // Copy data backwards in buffer
        std::copy(outbuff + frame_size, outbuff + outbuff_size, outbuff);
        // Empty old portion of buffer
        for (int i = outbuff_offset; i < outbuff_size; ++i) {
            outbuff[i] = 0;
        }
        outbuff_offset -= frame_size;
    }
    return status::SUCCESS;
}

vocoder::status vocoder::read_samples(double *buffer, int buff_offset, int num_samples) {
    sf_count_t samples_read = sf_read_double(input_fh, buffer + buff_offset, num_samples);
    if (samples_read < num_samples) {
        return status::BUFFER_NOT_FULL;
    }
    return status::BUFFER_FULL;
}