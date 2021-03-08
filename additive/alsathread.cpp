#include "alsathread.h"
#include <thread>
#include <vector>
#include "audio.h"
#include <math.h>
#include "mathext.h"


#include <QElapsedTimer>

extern const double frequencyMultiplent;
extern const double freqMin;
extern const double max;

extern uint alsaSampleRate;
extern uint framesPerPeriod;
inline unsigned int hash(unsigned int x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}
void startAlsathread()
{
    std::thread th([](){
        double* lookup = sinLookupTable();

        std::vector<double> frequencies;
        stereo16 *buffer;
        initAudio();
        buffer = (stereo16 *) malloc(sizeof(stereo16)*framesPerPeriod);
        message spectr = {0, 0};
        uint phase = 0;
        int output = 0;

        int logWindowSize = 0;
//        QElapsedTimer t;
//        t.start();
//        int samples = 0;
        int changes = 0;
        bool signPositive = 1;
        while(true) {
            output++;
            mut.lock();
            if(full) {
//                phase = 0;
//                fprintf(stderr, "Got medssage! %d", m.h);
                spectr = m;
                full = false;
                logWindowSize = intLog2(m.h);
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

            if(spectr.mode == logarithmic) {
                for(int j = 0; j < framesPerPeriod; j++) {
                    double v = 0;
                    for(int i = 0; i < spectr.h; i++) {
                        v += spectr.data[i]*lookup[int((frequencies[i]/alsaSampleRate
                                                *phase+i)*LOOKUP_TABLE_SIZE)%LOOKUP_TABLE_SIZE]/max*3;
                    }
                    phase++;
                    buffer[j].r = buffer[j].l = v*400;
                }
            } else {
                for(int j = 0; j < framesPerPeriod; j++) {
                    double v = 0;
                    for(uint i = 0; i < spectr.h/5; i++) {
//                        if(abs(int(alsaSampleRate*i/spectr.h - 211)) < 50) continue;
                        //                        v += spectr.data[i]*lookup[
                        //                                (((long long uint)(phase)*LOOKUP_TABLE_SIZE*i)>>logWindowSize)%LOOKUP_TABLE_SIZE]/max;
                        v += spectr.data[i]*lookup[
                                ((long long int)(phase*LOOKUP_TABLE_SIZE*i+hash(i))>>logWindowSize)%LOOKUP_TABLE_SIZE]/max;
//                        if(phase%32 == 0) {
//                           fprintf(stderr,
//                                   "freq %lf freq1 %lf v % lf data %lf lookup % lf max %lf\n",
//                                   double(alsaSampleRate)/spectr.h*i,
//                                   double(changes)*alsaSampleRate/phase,
//                                   v, spectr.data[i], lookup[
//                                   int((double(phase)/spectr.h+i)
//                                       *LOOKUP_TABLE_SIZE)%LOOKUP_TABLE_SIZE], max);
//                        }
//                        if(signPositive && v < 0) {
//                            signPositive = false;
////                            changes++;
//                        }
//                        if(!signPositive && v > 0) {
//                            signPositive = true;
//                            changes++;
//                        }
                    }
                    phase++;

                    buffer[j].r = buffer[j].l = v*400;
                }
            }
            if(phase > alsaSampleRate * 7) {
                phase = 0;
            }
            if(output%32 == 0) {
//               fprintf(stderr,
//                       "freq %lf freq1 %lf data %p height %d phase %d val %s",
//                      double(alsaSampleRate)/spectr.h*9,
//                      double(changes)*alsaSampleRate/phase,
//                       spectr.data, spectr.h, phase, buffer[2]);
            }
            writeFrames(buffer, framesPerPeriod);
        }
    });
    th.detach();
}
