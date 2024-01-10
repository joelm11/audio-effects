# audio-effects
A C++ implementation of phase vocoder-based vocal audio effects

# Usage:  
./effects_app <infile.wav> [options]

options  
    -o  <outfile.wav>    = Specify an output file  
    -e  <effect>         = Specify an effect from one of 'robot', 'tstretch', 'pitch'  
    -mf <num/den>        = Modification factor as a positive rational  


# Requirements:
- Libsndfile
- FFTW3