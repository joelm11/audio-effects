#include <sndfile.h>

class vocoder {
    public:
        enum effect {
            ROBOT = 0,
            TIME_STRETCH,
            PITCH_SHIFT,
            AUTO_TUNE
        };


    private:
        int load_inbuff();
        int write_out();


};