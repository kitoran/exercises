#ifndef SYNTHESIS_H
#define SYNTHESIS_H

#include <sndfile.h>
#include <vector>

#include "stft.h"

void resynthesizeOne(double* data, int w, SF_INFO inpi, int stepSize, int h);
void resynthesizeAll(double* data, int w, SF_INFO inpi, int stepSize, int h);

void resynthesizeAllLinear(double* data, int w, int stepSize, SF_INFO inpi, int specrtSize, int windowSize);
class ContMaximaSpectrogram;
void resynthesizeMaxima(ContMaximaSpectrogram *s, int start=-1, int end=-1);

std::vector<std::vector<continuousHarmonic> > prepareHarmonics(const std::vector<std::vector<harmonic> > &data
                                                               , int* continuities);


#endif // SYNTHESIS_H
