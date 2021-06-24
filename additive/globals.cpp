#include <inttypes.h>
#include <spectrogram.h>
extern const double freqMax = 40000;
extern const double freqMin=30;
extern const double frequencyMultiplent = 1.04;//sqrt(sqrt(freqMax/freqMin));

extern const int windowSize = 1<<10;
extern const int stepSize = 512;

int16_t *sampls = 0;
Spectrogram *spectrogram = 0;



extern const int cutoff = 10000;
