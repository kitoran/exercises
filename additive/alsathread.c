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

bool recordingInAThread = false;

_Thread_local char thread_name[30];

typedef int16_t frameType;
void* alsathread(void* d) {
    (void)d;

    frameType *buffer;
    initAudioPlayback(1, SND_PCM_FORMAT_S16_LE);//SND_PCM_FORMAT_FLOAT64_LE
    buffer = (frameType *) malloc(sizeof(frameType)*framesPerPeriod);
    struct message pos = {-1};
    uint64_t phase = 0;
    int output = 0;
    wait(&channel);

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

void* recordingThread(void* d) {
    (void)d;
    typedef int16_t frameType;
    frameType *buffer;

    snd_pcm_t *capture_handle;
    if (snd_pcm_open(&capture_handle, "default",
                     SND_PCM_STREAM_CAPTURE,
                     0) < 0) {
      abort();
    }
    snd_pcm_hw_params_t *hw_params;
    fprintf(stdout, "audio interface opened\n");
    if (snd_pcm_hw_params_malloc (&hw_params) < 0) {
        abort();
    }
    fprintf(stdout, "hw_params allocated\n");
    if(snd_pcm_hw_params_any (capture_handle, hw_params)<0) {
        abort();
    }
    if(snd_pcm_hw_params_set_access (
                capture_handle, hw_params,
                SND_PCM_ACCESS_RW_INTERLEAVED) <0) {
        abort();
    }
    if(snd_pcm_hw_params_set_format
            (capture_handle, hw_params, SND_PCM_FORMAT_S16_LE) < 0) {
        abort();
    }
    if(snd_pcm_hw_params_set_rate_near
            (capture_handle, hw_params, &rate, alsaSampleRate) < 0) {
        abort();
    }
    if(snd_pcm_hw_params_set_channels
            (capture_handle, hw_params, 1) < 0) {
        abort();
    }
    if(snd_pcm_hw_params (capture_handle, hw_params) < 0) {
        abort();
    }
    snd_pcm_hw_params_free (hw_params);
    if(snd_pcm_prepare (capture_handle) < 0) {
        abort();
    }
    buffer = (frameType *) malloc(sizeof(frameType)*framesPerPeriod);


    struct message pos = {-1};
    uint64_t phase = 0;
    int output = 0;

    wait(&channel);

    while(true) {
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

void startrRecordingInAThread(int16_t **data)
{
    static pthread_t recordingThread = 0;
    if(!recordingThread) {
        pthread_create(&newthread, 0, &recordingThread, 0);
    }
    pthread_setname_np(newthread, "alsathread");
}
