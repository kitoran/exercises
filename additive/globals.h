#ifndef GLOBALS_H
#define GLOBALS_H

#include <inttypes.h>
extern const double freqMax;
extern const double freqMin;
extern const double frequencyMultiplent;//sqrt(sqrt(freqMax/freqMin));

extern const int windowSize;
extern const int stepSize;

extern int16_t *samplsStbArray;
extern struct Spectrogram *spectrogram;
extern int cutoff;
extern int16_t* audioOutputStb;
extern double denominator;
extern int numberOfHarmonics;
extern double* originalFourierTransform;
extern int originalFourierTransformH, originalFourierTransformW;

extern double max;
extern struct SF_INFO inpi;
struct graph;
extern struct graph widget;

#endif // GLOBALS_H1
