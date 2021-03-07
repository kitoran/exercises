#include "alsathread.h"
#include <thread>
#include <vector>
#include "audio.h"
#include <math.h>
extern const double frequencyMultiplent;
extern const double freqMin;
extern const double tau;
extern const double max;

extern uint alsaSampleRate;
extern uint framesPerPeriod;
void startAlsathread()
{


    std::thread th([](){
#define POWER 15
#define TABLE_SIZE (1 << POWER)
        double lookup[TABLE_SIZE];
        for(int i = 0; i < TABLE_SIZE; i++) {
          lookup[i] = /*(*/sin(i*tau/TABLE_SIZE)/*/2+1)*UINT16_MAX*/;
        }

        std::vector<double> frequencies;
        stereo16 *buffer;
        initAudio();
        buffer = (stereo16 *) malloc(sizeof(stereo16)*framesPerPeriod);
        message spectr = {0, -1};
        int phase = 0;
        int output = 0;
        while(true) {
            output++;
            mut.lock();
            if(full) {
                phase = 0;
                fprintf(stderr, "Got medssage! %d", m.h);
                spectr = m;
                full = false;
                if(m.h > frequencies.size()) {
                    int i = frequencies.size();
                    for(; i < m.h; i++) {
                        double mul = pow(frequencyMultiplent, i);
                        double prod = freqMin * mul;
                        frequencies.push_back(prod);
                    }
                }

            }
            mut.unlock();

            for(int j = 0; j < framesPerPeriod; j++) {
                double v = 0;
                for(int i = 0; i < spectr.h; i++) {
                    v += spectr.data[i]*lookup[int(frequencies[i]/alsaSampleRate
                                            *phase*TABLE_SIZE)%TABLE_SIZE]/max*3;
//                    if(output % 128 == 0) {
//                        fprintf(stdout, "freq %lf, amp %lf  ", frequencies[i], spectr.data[i]);
//                    }
                }
//                if(spectr.h > 0 && output % 128 == 0) fprintf(stdout, "\n");
                phase++;
                buffer[j].r = buffer[j].l = v*400;
            }
            writeFrames(buffer, framesPerPeriod);
//            if(phase%16 == 0) {
//               fprintf(stderr,
//                       "data %p height %d phase %d",
//                       spectr.data, spectr.h, phase);
//            }
        }
    });
    th.detach();
}
