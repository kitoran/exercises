

/*

This example reads standard from input and writes
to the default PCM device for 5 seconds of data.

*/

/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <math.h>
#include <memory.h>
#include "alsa.h"
#include <stdio.h>
#define MIN(a,b) ((a)<(b)?(a):(b))

extern uint sampleRate;
extern uint framesPerPeriod;
double envelope(int sample, long long int length) {
    int a = sampleRate/30;
    if(sample < a) {
        return 1.0/a*sample;
    }
    if(length > sample && length - sample < sampleRate) {
        return 1/44100.0*(length-sample);
    }
    if(sample > length)  {
      fprintf(stderr, "bad! %d %lld", sample, length);
      exit(0);
        return 0;
    }
    return 1;
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
    fprintf(stderr, "%d loops\n", loops);
    long long int totalFrames = loops * framesPerPeriod;
    int frameNumber = 0;
    while(frameNumber < totalFrames) {
        int remainingSize = framesPerPeriod;
        if(frameNumber + framesPerPeriod >= totalFrames) {
            remainingSize = totalFrames - frameNumber;
        }
        for(int j = 0; j < remainingSize; j++, frameNumber++) {
            buffer[j].l = valueTable[frameNumber%(int)framesPerOsc]*envelope((frameNumber), totalFrames);
            buffer[j].r = buffer[j].l;
        }

        writeFrames(buffer, framesPerPeriod);
        sortIter(indexTable, valueTable, framesPerOsc);
    }
    drain();
//  free(buffer); // there is no need to free

  return 0;
}
