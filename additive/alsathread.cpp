#include "alsathread.h"
#include <thread>
#include <QDebug>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>
#include "audio.h"
#include <math.h>
#include "mathext.h"
#include "stft.h"


#include <QElapsedTimer>

extern const double frequencyMultiplent;
extern const double freqMin;

extern uint alsaSampleRate;
extern uint framesPerPeriod;
using namespace std::chrono_literals;


void startAlsathread()
{
    std::thread th([](){
        double* lookup = sinLookupTable();

        std::vector<double> frequencies;
        double *buffer;
        initAudio(1, SND_PCM_FORMAT_FLOAT64_LE);
        buffer = (double *) malloc(sizeof(double)*framesPerPeriod);
        message spectr = {{0, 0}, spectrogram_mode::linear};
        uint phase = 0;
        int output = 0;

        int logWindowSize = 0;
//        QElapsedTimer t;
//        t.start();
//        int samples = 0;
//        int changes = 0;
//        bool signPositive = 1;
        std::vector<double> unFft;
        while(true) {
            qDebug() << "hey";
            int unfftIndex = 0;
            output++;
            if(channel.full) {
//                phase = 0;
//                fprintf(stderr, "Got medssage! %d", m.h);
                spectr = channel.take();


                logWindowSize = intLog2(spectr.windowSize);
                int size = spectr.data.sized;
                if(spectr.complex) size /= 2;
                if(size > frequencies.size()) {
                    int i = frequencies.size();
                    for(; i < size; i++) {
                        double mul = pow(frequencyMultiplent, i);
                        double prod = freqMin * mul;
                        frequencies.push_back(prod);
                    }
                }
                unFft.resize(spectr.windowSize);

                fft((std::complex<double>*)spectr.data.data, spectr.windowSize, &unFft[0]);
                Q_ASSERT(spectr.windowSize == window.size());
                for(int i = 0; i < spectr.windowSize; i++) {
                    unFft[i] /= window[i];
                }
            }
            qDebug() << "hey";
            if(spectr.data.data == 0) {
                qDebug() << "waitinf...";
                channel.wait();
                qDebug() << "waited!";
                continue;
            }
/*
            if(spectr.mode == logarithmic) {
                for(int j = 0; j < framesPerPeriod; j++) {
                    double v = 0;
                    for(int i = 0; i < spectr.spectrumSize; i++) {
                        v += spectr.data[i]*lookup[int((frequencies[i]/alsaSampleRate
                                                *phase+i)*LOOKUP_TABLE_SIZE)%LOOKUP_TABLE_SIZE]/max*3;
                    }
                    phase++;
                    buffer[j] = v*400;
                }
            } else */{
                int written = 0;
                while(spectr.windowSize > 0 && written < framesPerPeriod) {
                    int amount = std::min(framesPerPeriod-written,
                                          spectr.windowSize);
                    memcpy(buffer, &unFft[0], amount);
                    written+=amount;
                    unfftIndex += amount;
                    unfftIndex &= (spectr.windowSize-1);
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
    pthread_setname_np(th.native_handle(), "alsathread");
    th.detach();
}

