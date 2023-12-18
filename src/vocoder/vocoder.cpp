#include <algorithm>
#include <cmath>
#include <complex>
#include <cstddef>
#include <cstring>
#define _USE_MATH_DEFINES
#include <math.h>
#include "fftw3.h"
#include "sndfile.h"
#include "vocoder.hpp"
#include "vocoder_types.hpp"
#include "util_math.hpp"

vocoder::vocoder (const voc_args &init_args) : user_args(init_args) { }

vocoder::vocoder (const voc_args &init_args, int frame_size) : user_args(init_args), frame_size(frame_size) { }

vocoder::~vocoder() {
    delete [] inbuff;
    delete [] outbuff;
    delete [] hann_win;
    delete [] prev_phase;
    delete [] prev_synth_phase;
    fftw_destroy_plan(p);
    fftw_free(fftw_input);
    fftw_free(fftw_output);
}

vocoder::status vocoder::vocoder_init() {
    input_fh = sf_open(user_args.input_filename.c_str(), SFM_READ, &file_data);
    if (input_fh == NULL) {
        return status::FILE_READ_FAIL;
    }
    inbuff   = new dtype[frame_size * 2];
    outbuff  = new dtype[frame_size * 2];
    prev_phase = new complex[frame_size];
    prev_synth_phase = new complex[frame_size];
    
    fftw_input = (complex*) fftw_malloc(sizeof(fftw_complex) * frame_size);
    fftw_output = (complex*) fftw_malloc(sizeof(fftw_complex) * frame_size);
    p = fftw_plan_dft_1d(frame_size, reinterpret_cast<fftw_complex*>(fftw_input),
                         reinterpret_cast<fftw_complex*>(fftw_output), FFTW_FORWARD, FFTW_MEASURE);
    
    hann_win = new dtype[frame_size];
    compute_hann_win(hann_win, frame_size, analysis_hop_size);
    
    status ret_status = read_samples(inbuff, PAST);
    // ret_status = read_samples(inbuff, PRESENT);
    return ret_status;
}

vocoder::status vocoder::analysis() {
    status ret_status = read_samples(inbuff, PRESENT);
    for (int i = 0; i < frame_size; ++i) {
        inbuff[i] *= hann_win[i];
    }
    std::copy_n(inbuff + PRESENT, frame_size, fftw_input);
    fftw_execute(p);
    for (int i = 0; i < frame_size; ++i) {
        fftw_input[i] = std::polar(fftw_input[i].real(), fftw_input[i].imag());
    }
    return ret_status;
}

vocoder::status vocoder::modify_phase_r() {
    for(int i = 0; i < frame_size; ++i) {
        fftw_input[i].imag(0);
    }
    return status::SUCCESS;
}

vocoder::status vocoder::modify_phase_t() {
    double bin_freqs[frame_size]; double phase_adv[frame_size];
    double phase_inc[frame_size]; double inst_freqs[frame_size];
    double phase_prop[frame_size];
    for (int i = 0; i < frame_size; ++i) {
        bin_freqs[i] = 2 * M_PI * i / frame_size;
        phase_adv[i] = bin_freqs[i] * analysis_hop_size;
    }
    for (int i = 0; i < frame_size; ++i) {
        phase_inc[i] = fftw_input[i].imag() - prev_phase[i].imag() - phase_adv[i];
        prev_phase[i] = fftw_input[i].imag();
        phase_inc[i] = std::fmod(phase_inc[i] + M_PI, 2 * M_PI) - M_PI;
    }
    for (int i = 0; i < frame_size; ++i) {
        inst_freqs[i] = bin_freqs[i] + phase_inc[i] / analysis_hop_size;
    }
    for (int i = 0; i < frame_size; ++i) {
        phase_prop[i] = prev_synth_phase[i].imag() + synthesis_hop_size * inst_freqs[i];
        prev_synth_phase[i] = phase_prop[i];
    }
    // Write previous buffers for next computation
    return status::SUCCESS;
}

vocoder::status vocoder::read_samples(float *buffer, int buff_offset) {
    sf_count_t samples_read = sf_read_float(input_fh, buffer + buff_offset, frame_size);
    if (samples_read < frame_size) {
        return status::BUFFER_NOT_FULL;
    }
    return status::SUCCESS;
}

vocoder::status vocoder::read_samples(double *buffer, int buff_offset) {
    sf_count_t samples_read = sf_read_double(input_fh, buffer + buff_offset, frame_size);
    if (samples_read < frame_size) {
        return status::BUFFER_NOT_FULL;
    }
    return status::BUFFER_FULL;
}