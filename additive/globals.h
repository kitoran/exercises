#ifndef GLOBALS_H
#define GLOBALS_H
#include <vector>
#include <inttypes.h>
extern const double freqMax;
extern const double freqMin;
extern const double frequencyMultiplent;//sqrt(sqrt(freqMax/freqMin));

extern const int windowSize;
extern const int stepSize;

extern int16_t *sampls;
class Spectrogram;
extern Spectrogram *spectrogram;
extern const int cutoff;
extern std::vector<int16_t> audioOutput;
extern double denominator;
inline int numberOfHarmonics = 10;
inline double* originalFourierTransform{0};
inline int originalFourierTransformH{-1}, originalFourierTransformW{-1};

struct SF_INFO;
extern SF_INFO inpi;
#endif // GLOBALS_H1
