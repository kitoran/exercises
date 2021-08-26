#include <inttypes.h>
//#include <spectrogram.h>
#include <sndfile.h>
extern const double freqMax = 40000;
extern const double freqMin=30;
extern const double frequencyMultiplent = 1.04;//sqrt(sqrt(freqMax/freqMin));

extern const int windowSize = 1<<12;//12;
extern const int stepSize = 256;

int16_t *sampls = 0;
struct Spectrogram *spectrogram = 0;



extern const int cutoff = 20000;

int16_t* audioOutputStbArray = 0;
double denominator = 170;

SF_INFO inpi;
