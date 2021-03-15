

/*

This example reads standard from input and writes
to the default PCM device for 5 seconds of data.

*/

/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <math.h>
#include <limits.h>
#include <memory.h>
#include "audio.h"
#include "midi.h"
#include <stdio.h>
#define MIN(a,b) ((a)<(b)?(a):(b))

extern uint alsaSampleRate;
extern uint framesPerPeriod;
float attack(long long int sample) {
    long long int a = alsaSampleRate/30;
    if(sample < a) {
        return 1.0/a*sample;
    }
    return 1;
}

double release(int sample, double init) {
    const int l = 10;
    if(sample > l) {
        fprintf(stderr, "bad! %d %d", sample, l);
        exit(1);
    }
    return init*(1-((double)sample)/l);
}

void swap(int* indexTable, int16_t* valueTable, int i, int j) {
    int temp;
    temp = valueTable[i];
    valueTable[i] = valueTable[j];
    valueTable[j] = temp;
    temp = indexTable[i];
    indexTable[i] = indexTable[j];
    indexTable[j] = temp;
}


void sortIter(int* indexTable, int16_t* valueTable, int framesPerOsc) {
    static int iteration = 0;
    for(int i = 0; i <  framesPerOsc-1; i++) {
        if(indexTable[i+1] < indexTable[i]) {
            swap(indexTable, valueTable, i, i+1);
        }
    }
    for(int i = 0; i <  framesPerOsc; i++) {
        printf("%d ", indexTable[i]);
    }
    printf("\n");
}
void iter(int16_t* valueTable, int framesPerOsc) {
    for(int i = 0; i <  framesPerOsc; i++) {
        printf("%d ", valueTable[i]);
    }
    printf("\n");
    bool even;
    even = !even;
    valueTable[0] = ((valueTable[1] ) + (valueTable[framesPerOsc-1] ))/2 + (even?100:-300);
    for(int i = 1; i <  framesPerOsc-1; i++) {
        valueTable[i] = (valueTable[i-1] >> 1) + (valueTable[i+1] >> 1);
    }
    valueTable[framesPerOsc-1] = (valueTable[framesPerOsc-2] >> 1)
            + (valueTable[0] >> 1) ;
//    valueTable[0] = 0;
}
extern snd_pcm_t * handle;
int main() {
    midi_open();
    long loops;
    //  int size;
    int16_t *buffer;

    initAudio(1, SND_PCM_FORMAT_S16_LE);
    buffer = (int16_t *) malloc(sizeof(int16_t)*framesPerPeriod);
    const double freq = 440;
    const double framesPerOsc = alsaSampleRate/freq;
    fprintf(stderr, "%lf frames int one osc\n", framesPerOsc);
    int* indexTable = (int*)malloc(sizeof(int)*framesPerOsc);
//    memset(indexTable, 0, framesPerPeriod);
    int16_t* valueTable = (int16_t*)malloc(sizeof(int16_t)*framesPerOsc);
    for(int i = 0; i < (int)framesPerOsc; i++) {
        indexTable[i] = i;
        valueTable[i] = 0;
    }
//    for(int n = 1; n < 600; n+=2) {
//        float f = 0;//(float)rand()/(float)(RAND_MAX/2.0/M_PI);
        for(int i = 0; i < (int)framesPerOsc; i++) {

//            valueTable[i] += cos(freq*2*M_PI*(n)/sampleRate*i + f)*400.0/n;
////            fprintf(stderr, "i=%d\n", i);
//            valueTable[i] += freq/sampleRate*i*i*20;
            double pv = sin(freq*2*M_PI/alsaSampleRate*i);
            double sqr = copysign(exp(pv)-1, pv);
            valueTable[i] += sqr*400.0;

        }
//        for(int i = (int)framesPerOsc/2; i < (int)framesPerOsc; i++) {

//            valueTable[i] += valueTable[(int)framesPerOsc/2-1] - freq/sampleRate*i*i*20;
//        }
//        fprintf(stderr, "n=%d\n", n);
//    }
//    for(int i = 0; i < (int)framesPerOsc; i++) {
//        valueTable[i] += (i>=framesPerOsc/2)*400.0;
////            fprintf(stderr, "i=%d\n", i);
//    }

//    for(int i = 0; i < (int)framesPerOsc; i++) {
//        int j = rand()%(int)framesPerOsc;
//        swap(indexTable, valueTable, i, j);
//    }

    double periodInSecs = 32.0/44100.0;
    /* 5 seconds in microseconds divided by
     * period time */
    loops = 5 / periodInSecs;
    fprintf(stderr, "%d loops %lf millisecs in period\n", loops, periodInSecs*1000);
//    long long int totalFrames = loops * framesPerPeriod;
    int frameNumber = 0;
    double phase = 0;
    double frequency = 50;
    long long int sample = 0;
//    struct timeval timecheck;
    subscribe();
    fprintf(stderr, "avail: %d\n", snd_pcm_avail(handle));
//    return 0;
    int framesPerOscNormal = framesPerOsc;
    uint32_t value = 3;
    int simulation[100];
    int index = 0;

    while(true) {
//        time();
            snd_seq_event_t* ev = midi_read();
            if(ev) {
                fprintf(stderr, "%c %d\n", ev->data.note.note, (int)(ev->data.note.note));
                if(ev->type == SND_SEQ_EVENT_NOTEON) {
                    frequency = 440 * pow(2, ((double)(ev->data.note.note-69))/12);
                }
            }
//        fprintf(stderr, ".");
//        int remainingSize = framesPerPeriod;
//        if(frameNumber + framesPerPeriod >= totalFrames) {
//            remainingSize = totalFrames - frameNumber;
//        }

        int last;
        for(int j = 0; j < framesPerPeriod; j++, frameNumber++) {
//            buffer[j].l = 0;
//            buffer[j].r = buffer[j].l = valueTable[sample%framesPerOscNormal];
            sample++;
//            int fewfwe = double(value)/UINT_MAX;
//            buffer[j] = (fewfwe + simulation[index])/2000000;
//            simulation[index] += fewfwe;
//            value = value * (value + 1);
//            index++;
//            index = index % 100;
//            phase += M_PI*2/sampleRate*frequency;
            buffer[j] = ((long long int)(sample*frequency*2/alsaSampleRate)%2 * 2 - 1)*1600;
        }

        writeFrames(buffer, framesPerPeriod);
//        iter(valueTable, framesPerOscNormal);
    }
    drainAudio();
//  free(buffer); // there is no need to free

  return 0;
}
