#ifndef STFT_H
#define STFT_H
#include <inttypes.h>
#include "stb_ds.h"

extern double* windowStbArray;


void makeHammingWindow(int windowSize);
void stft(int16_t *data, int size,
          int window, int step, int samplerate,
          double** res, int* resH, int* resW);

void fftI16(int16_t *data, int size, double* res);

void complex_fftI16(int16_t *data, int size, double _Complex* res);
void stfft(int16_t *data, int size,
          int window /* should be power of 2 */, int step,
          double** res, int* resW); // resH = window
void complex_stfft(int16_t *data, int size, int windowSize, int step, double _Complex **res, int *resW);

typedef struct harmonic {
    double freq;
    double amp;
} harmonic;

typedef struct continuousHarmonic {
    struct harmonic h;
    int prev;
    int continuity;
    _Bool hasRightCounterpart;
} continuousHarmonic;

struct harmonic* maxesStbArray(double *data, int h, int w);
struct harmonic** maxesLinearStbArray(double *data, int h, int w, int samplerate);

void shiftandmul(double *src, int h, int w, double** dest, int *resH);
void shiftandmulLinear(double *src, int h, int w, double** dest, int *resH);
void multiplyFundamentalLinear(double *src, int h, int w, double** dest, int *resH, int harmonics, double treshold);
void divideFundamentalLinear(double *src, int h, int w, double** dest, int *resH, int harmonics, double treshold);
void isolateMaxima(int w, double *transform, int h, double *data);

#endif // STFT_H
