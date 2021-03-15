#ifndef SYNTHESIS_H
#define SYNTHESIS_H

#include <sndfile.h>

enum spectrogram_mode {linear, logarithmic};
void resynthesizeOne(double* data, int w, SF_INFO inpi, int stepSize, int h);
void resynthesizeAll(double* data, int w, SF_INFO inpi, int stepSize, int h);

void resynthesizeAllLinear(double* data, int w, int stepSize, SF_INFO inpi, int specrtSize, int windowSize);

#endif // SYNTHESIS_H
