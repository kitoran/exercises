

/*

This example reads standard from input and writes
to the default PCM device for 5 seconds of data.

*/

/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <math.h>
#include <memory.h>
#include "audio.h"
#include "midi.h"
#include <stdio.h>
#define MIN(a,b) ((a)<(b)?(a):(b))

extern uint sampleRate;
extern uint framesPerPeriod;
double attack(long long int sample) {
    long long int a = sampleRate/30;
    if(sample < a) {
        return 1.0/a*sample;
    }
    return 1;
}

double release(int sample, double init) {
    const int l = 10;
    if(sample > l) {
        fprintf(stderr, "bad! %d %lld", sample, l);
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
int main() {
    midi_open();
    long loops;
    //  int size;
    stereo16 *buffer;

    init();
    buffer = (stereo16 *) malloc(sizeof(stereo16)*framesPerPeriod);
    const double freq = 440;
    const double framesPerOsc = sampleRate/freq;
    fprintf(stderr, "%lf frames int one osc\n", framesPerOsc);
    int* indexTable = (int*)malloc(sizeof(int)*framesPerOsc);
//    memset(indexTable, 0, framesPerPeriod);
    int16_t* valueTable = (int16_t*)malloc(sizeof(int16_t)*framesPerOsc);
    for(int i = 0; i < (int)framesPerOsc; i++) {
        valueTable[i] = sin(freq*2*M_PI/sampleRate*i)*10000;
        indexTable[i] = i;
    }

    for(int i = 0; i < (int)framesPerOsc; i++) {
        int j = rand()%(int)framesPerOsc;
        swap(indexTable, valueTable, i, j);
    }

    double periodInSecs = 32.0/44100.0;
    /* 5 seconds in microseconds divided by
     * period time */
    loops = 5 / periodInSecs;
    fprintf(stderr, "%d loops %lf millisecs in period\n", loops, periodInSecs*1000);
//    long long int totalFrames = loops * framesPerPeriod;
    int frameNumber = 0;
    double phase = 0;
    double frequency = 440;
    long long int sample = 0;
//    struct timeval timecheck;
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
        for(int j = 0; j < framesPerPeriod; j++, frameNumber++) {
            buffer[j].l = sin(phase)*10000*attack(sample);
            buffer[j].r = buffer[j].l;
            sample++;
            phase += M_PI*2/sampleRate*frequency;
        }

        writeFrames(buffer, framesPerPeriod);
        usleep(periodInSecs*200000);
    }
    drain();
//  free(buffer); // there is no need to free

  return 0;
}
