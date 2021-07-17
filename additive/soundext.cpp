#include "soundext.h"
#include "audio.h"
#include "globals.h"
#include "stft.h"
#include "math.h"


int freqToFftIndex(double freq, int sampleRate) {
    return freq*windowSize/sampleRate;
}
double indToFftFreq(int ind, int sampleRate) {
    return double(ind)/windowSize*sampleRate;
}


void alsaPlayBlock(int16_t* buffer, int size) {
    int index = 0;
    while(index + framesPerBuffer <size) {
//        for(int i = 0; i < framesPerBuffer; i++) {
//            assert(fabs(buffer[index + framesPerBuffer+i]) <= 1/denominator);
//        }
        writeFrames(buffer+index, framesPerBuffer);
        index+=framesPerBuffer;
    }
    if(index < size) {
        writeFrames(buffer+index, size-index);
    }
}
