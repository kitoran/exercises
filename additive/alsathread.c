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



#include "graph.h"



extern uint alsaSampleRate;
extern uint framesPerPeriod;
const char alsaThreadName[] = "alsathread";
struct Channel channelForPlayback = {
    {0},
    false,
    PTHREAD_COND_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    -1,
    alsaThreadName,
};

typedef int16_t frameType;
void* alsathread(void* d) {
    (void)d;

    frameType *buffer;
    initAudioPlayback(1, SND_PCM_FORMAT_S16_LE);//SND_PCM_FORMAT_FLOAT64_LE
    buffer = (frameType *) malloc(sizeof(frameType)*framesPerPeriod);
    struct message pos = {-1};
    uint64_t phase = 0;
    int output = 0;
    wait(&channelForPlayback);

    while(true) {
        //            qDebug() << "hey";
//        int unfftIndex = 0;
        output++;
        if(channelForPlayback.full) {
            //                phase = 0;
            //                fprintf(stderr, "Got medssage! %d", m.h);
            takeC(&channelForPlayback, &pos, sizeof(pos));

            //                spectrumSize = spectr.complex?spectr.data.sizec():spectr.data.sized;
            if(pos.pos < 0 || pos.pos >= spectrogram->width(spectrogram)) {
                fprintf(stderr, "waitinf...");
                wait(&channelForPlayback);
                fprintf(stderr, "waited!");
                continue;
            }
        }
        if(action == actionPlay) {
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
        } else {
            MaximaSpectrogram* sgfdgsfd = spectrogram;
            struct harmonic** s = sgfdgsfd->maxima;
//            static int harmonic_number = 0;
            static int periodCount;
//            if(harmonic_number > arrlen(s[pos.pos])) {
//                harmonic_number = 0;
//            }
            double howLongOneHarmonic = 0.3;
            double howLongPeriod = (double)(framesPerPeriod)/alsaSampleRate;
            int whichHarmonic = periodCount*howLongPeriod/howLongOneHarmonic;

            whichHarmonic  = whichHarmonic % (arrlen(s[pos.pos]));

            for(int j = 0; j < framesPerPeriod; j++) {
                double v = 0;

                v += s[pos.pos][whichHarmonic].amp*sinLookupTable
                        [(int64_t)((s[pos.pos][whichHarmonic].freq
                                            *phase/alsaSampleRate)*LOOKUP_TABLE_SIZE)
                            %LOOKUP_TABLE_SIZE]/max/*3*/;

                phase++;
                buffer[j] = v*800/*/*400   /INT16_MAX*/;
            }


            writeFrames(buffer, framesPerPeriod);
            periodCount++;
        }
    }
    return NULL;
}


void startAlsathread()
{
    pthread_t newthread;

    pthread_create(&newthread, 0, &alsathread, 0);

    pthread_setname_np(newthread, alsaThreadName);
}
const char recordingThreadName[] = "recordingthread";
struct Channel channelForRecording = {
    {0},
    false,
    PTHREAD_COND_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    -1,
    recordingThreadName
};
bool recordingInAThread = false;
_Thread_local char recording_thread_name[30];
//typedef struct MessageRec {
    typedef enum Action {
        record,
//        pause,
        stop
    } Action;
//} MessageRec ;
void* recordingThread(void* d) {
    (void)d;
    initAudioCaptureS16LE();

//    struct MessageRec message;
    Action action;
    wait(&channelForRecording);
    int pos = 0;
    fprintf(stderr, "recordingThread before while pos=%d size=%d\n", pos, arrlen(samplsStbArray));
    while(true) { external:
        fprintf(stderr, "recordingThread start of loop pos=%d size=%d\n", pos, arrlen(samplsStbArray));
        if(channelForRecording.full) {
            fprintf(stderr, "recordingThread inside if pos=%d size=%d\n", pos, arrlen(samplsStbArray));
            takeC(&channelForRecording, &action, sizeof(action));
            if(action == stop) {
                alsaDropCapture();
                fprintf(stderr, "recordingThread inside stop if pos=%d size=%d\n", pos, arrlen(samplsStbArray));
                pos = 0;
                wait(&channelForRecording);
                goto external;
            }
            else {
                initAudioCaptureS16LE();
                fprintf(stderr, "recordingThread inside stop else pos=%d size=%d\n", pos, arrlen(samplsStbArray));
                pos = 0;
//                arrsetlen(samplsStbArray, 0);
            }
        }
        if(pos + framesPerPeriod > arrlen(samplsStbArray)) {
            fprintf(stderr, "recordingThread inside enlarging if pos=%d size=%d\n", pos, arrlen(samplsStbArray));
            arrsetlen(samplsStbArray,
                      arrlen(samplsStbArray)+framesPerPeriod);
            memset(samplsStbArray+ pos, 0, arrlen(samplsStbArray)-pos-1);
        }
        alsaRecordBlock(samplsStbArray+ pos, framesPerPeriod);
        pos += framesPerPeriod;
    }
    return NULL;
}

void startRecordingInAThread()
{

//    pos = 0;
    arrsetlen(samplsStbArray, 0);
    recordingInAThread = true;
    static pthread_t newThread = 0;
    if(!newThread) {
        pthread_create(&newThread, 0, &recordingThread, 0);
        pthread_setname_np(newThread, "recordingthread");
    }
    Action action = record;
    blockAndPut(&channelForRecording, &action, sizeof(action));
}

void stopRecordingInAThread()
{
    recordingInAThread = false;
    Action action = stop;
    blockAndPut(&channelForRecording, &action, sizeof(action));
}
