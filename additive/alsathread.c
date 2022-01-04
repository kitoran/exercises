#include "alsathread.h"
#include "globals.h"
#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include "stb_ds.h"
#include "audio.h"
#include <math.h>
#include "mathext.h"
#include "stft.h"



extern uint alsaSampleRate;
extern uint framesPerPeriod;
struct Channel channel;
_Thread_local char thread_name[30];

void* alsathread(void* d) {
    (void)d;
    //        vdouble* lookup = sinLookupTable();
    //        int*lookup = sinLookupTableInt();
//    double* frequenciesStbArray = 0;//
    typedef int16_t frameType;
    frameType *buffer;
    initAudio(1, SND_PCM_FORMAT_S16_LE);//SND_PCM_FORMAT_FLOAT64_LE
    buffer = (frameType *) malloc(sizeof(frameType)*framesPerPeriod);
    struct message pos = {-1};
    uint64_t phase = 0;
    int output = 0;

//    int logWindowSize = 0;
    //        QElapsedTimer t;
    //        t.start();
    //        int samples = 0;
    //        int changes = 0;
    //        bool signPositive = 1;
//    double* unFftStbArray;
//    int* spectrumIntStbArray;
    wait(&channel);
//    int spectrumSize;

    while(true) {
        //            qDebug() << "hey";
//        int unfftIndex = 0;
        output++;
        if(channel.full) {
            //                phase = 0;
            //                fprintf(stderr, "Got medssage! %d", m.h);
            takeC(&channel, &pos, sizeof(pos));

            //                spectrumSize = spectr.complex?spectr.data.sizec():spectr.data.sized;
            if(pos.pos < 0 || pos.pos >= spectrogram->width(spectrogram)) {
                fprintf(stderr, "waitinf...");
                wait(&channel);
                fprintf(stderr, "waited!");
                continue;
            }
        }
        spectrogram->fillBuffer(spectrogram, buffer, framesPerPeriod, pos.pos, phase);
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
    return NULL;
}


void startAlsathread()
{
    pthread_t newthread;

    pthread_create(&newthread, 0, &alsathread, 0);

    pthread_setname_np(newthread, "alsathread");
}

