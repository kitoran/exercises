#include <inttypes.h>
//#include <spectrogram.h>
#include <sndfile.h>
#include "graph.h"
/*extern */const double freqMax = 40000;
/*extern */const double freqMin=30;
/*extern */const double frequencyMultiplent = 1.04;//sqrt(sqrt(freqMax/freqMin));

/*extern */const int windowSize = 1<<12;//12;
/*extern */const int stepSize = 256;

int16_t *sampls = 0;
struct Spectrogram *spectrogram = 0;


int16_t* audioOutputStb = 0;
int numberOfHarmonics = 4;
int originalFourierTransformH = -1, originalFourierTransformW = -1;
/*extern *//*const*/ int cutoff = 20000;
double* originalFourierTransform = 0;
int16_t* audioOutputStbArray = 0;
double denominator = 170;
double max = -1;
SF_INFO inpi;
graph widget;
