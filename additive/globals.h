#ifndef GLOBALS_H
#define GLOBALS_H

extern const double freqMax;
extern const double freqMin;
extern const double frequencyMultiplent;//sqrt(sqrt(freqMax/freqMin));

extern const int windowSize;
extern const int stepSize;

extern int16_t *sampls;
class Spectrogram;
extern Spectrogram *spectrogram;
extern const int cutoff;
#endif // GLOBALS_H1
