#ifndef SYNTHESIS_H
#define SYNTHESIS_H

#include <sndfile.h>

#include "stft.h"

void resynthesizeOne(double* data, int w, SF_INFO inpi, int stepSize, int h);
void resynthesizeAll(double* data, int w, SF_INFO inpi, int stepSize, int h);

void resynthesizeAllLinear(double* data, int w, int stepSize, SF_INFO inpi, int specrtSize, int windowSize);
struct ContMaximaSpectrogram;
void resynthesizeMaxima(struct ContMaximaSpectrogram *s, int start, int end);

struct continuousHarmonic** prepareHarmonicsStbArray(struct harmonic* const* dataStbArray
                                                               , int* continuities);


#endif // SYNTHESIS_H
