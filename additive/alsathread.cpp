#include "alsathread.h"
#include "globals.h"
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

extern uint alsaSampleRate;
extern uint framesPerPeriod;
using namespace std::chrono_literals;


void startAlsathread()
{

    std::thread th([](){
//        double* lookup = sinLookupTable();
//        int*lookup = sinLookupTableInt();
        std::vector<double> frequencies;
        using frameType = int16_t;
        frameType *buffer;
        initAudio(1, SND_PCM_FORMAT_S16_LE);//SND_PCM_FORMAT_FLOAT64_LE
        buffer = (frameType *) malloc(sizeof(frameType)*framesPerPeriod);
        message pos = {-1};
        uint64_t phase = 0;
        int output = 0;

        int logWindowSize = 0;
//        QElapsedTimer t;
//        t.start();
//        int samples = 0;
//        int changes = 0;
//        bool signPositive = 1;
        std::vector<double> unFft;
        std::vector<int> spectrumInt;
        channel.wait();
        int spectrumSize;

        while(true) {
//            qDebug() << "hey";
            int unfftIndex = 0;
            output++;
            if(channel.full) {
//                phase = 0;
//                fprintf(stderr, "Got medssage! %d", m.h);
                pos = channel.take();

//                spectrumSize = spectr.complex?spectr.data.sizec():spectr.data.sized;
                if(pos.pos < 0 || pos.pos >= spectrogram->width()) {
                    qDebug() << "waitinf...";
                    channel.wait();
                    qDebug() << "waited!";
                    continue;
                }
            }
            spectrogram->fillBuffer(buffer, framesPerPeriod, pos.pos, phase);
            phase+=framesPerPeriod;
//            clock_t afterLoop = clock();
//            if(phase > alsaSampleRate * 7) {
//                phase = 0;
//            }
//            if(output%32 == 0) {
////               fprintf(stderr,
////                       "freq %lf freq1 %lf data %p height %d phase %d val %s",
////                      double(alsaSampleRate)/spectr.h*9,
////                      double(changes)*alsaSampleRate/phase,
////                       spectr.data, spectr.h, phase, buffer[2]);
//            }
            writeFrames(buffer, framesPerPeriod);
//            clock_t afterWrite = clock();
//            qDebug() << float(afterWrite-afterLoop)/CLOCKS_PER_SEC << float(afterLoop - start)/CLOCKS_PER_SEC;
        }
    });
    pthread_setname_np(th.native_handle(), "alsathread");
    th.detach();
}

