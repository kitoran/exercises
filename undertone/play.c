

#include "audio.h"

#include <sndfile.h>
#include <math.h>
#include <stdbool.h>
#include "mathext.h"
#define countof(a) (sizeof(a)/sizeof(*(a)))
#define end(a) ((a)+countof(a))
extern uint alsaSampleRate;
extern uint framesPerPeriod;
void tri() {
    initializeMathExt();
    typedef int16_t frameType;
    frameType *buffer;
    initAudio(1, SND_PCM_FORMAT_S16_LE);//SND_PCM_FORMAT_FLOAT64_LE
    buffer = (frameType *) malloc(sizeof(frameType)*framesPerPeriod);


    uint64_t phase = 0;

    double freqs[] = {//110.0*16,
                      //110.0*16/2,
                      //110.0*16/3,
//                      110.0*16/6,
//                      110.0*16/6*pow(2, 3.0/12),
//                      110.0*16/6*pow(2, 7.0/12),
                      110.0*16/6,
                      110.0*16/6*7/6,
                      110.0*16/6*9/6,
                      //110.0*16/7,
                      //110.0*16/8,
                      //110.0*16/9,
                      //110.0*16/11
                     };//, 110.0*16/9,*/ 110.0*16/4, 110.0*16/5};

    SF_INFO inpi;
    inpi.format = 0;
    SNDFILE* inp = sf_open("/home/n/exercises/squaredsound/02-201019_1328m.wav", SFM_READ, &inpi);

    SF_INFO fi = inpi;
//    fi.channels = 1;
//    fi.format = SF_FORMAT_WAV | SF_FORMAT_PCM_32;
//    fi.frames = 44100*5;
//    fi.samplerate = 44100;
//    fi.sections = 1;
//    fi.seekable = 1;
    SNDFILE* f = sf_open("679minor.wav", SFM_WRITE, &fi);
//    while(true) {
    for(int i = 0; i < 44100*5/framesPerPeriod; i++) {
     for(int j = 0; j < framesPerPeriod; j++) {
         buffer[j] = 0;
         for(double* freq = freqs; freq < end(freqs); freq++) {
            buffer[j] += sinLookupTable[(int64_t)((*freq
                                             *phase/alsaSampleRate)*LOOKUP_TABLE_SIZE)
                             %LOOKUP_TABLE_SIZE]*800;
         }
        phase++;
     }

     writeFrames(buffer, framesPerPeriod);
     sf_write_double(f, buffer, framesPerPeriod);
    }
    sf_close(f);
}
