#ifndef STFT_H
#define STFT_H
#include <inttypes.h>
#include <vector>
inline double max = 0;
void stft(int16_t *data, int size,
          int window, int step, int samplerate,
          double** res, int* resH, int* resW);
template<typename inptype>
void fft(inptype *data, int size, double* res);
void stfft(int16_t *data, int size,
          int window /* should be power of 2 */, int step,
          double** res, int* resW); // resH = window
struct harmonic {
    double freq;
    double amp;
};
std::vector<harmonic> maxes(double *data, int h, int w);
std::vector<harmonic> maxesLinear(double *data, int h, int w, int samplerate,
                                  int windowSize);

void shiftandmul(double *src, int h, int w, double** dest, int *resH);
void multiplyFundamentalLinear(double *src, int h, int w, double** dest, int *resH, int harmonics, double treshold);
void divideFundamentalLinear(double *src, int h, int w, double** dest, int *resH, int harmonics, double treshold);
void isolateMaxima(int w, double *transform, int h, double *data);

#endif // STFT_H
