#include "soundext.h"
#include "audio.h"
#include "globals.h"
#include "stft.h"
#include "math.h"


int freqToFftIndex(double freq, int sampleRate) {
    return freq*windowSize/sampleRate;
}
double indToFftFreq(int ind, int sampleRate) {
    return (double)(ind)/windowSize*sampleRate;
}

