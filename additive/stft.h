#ifndef STFT_H
#define STFT_H
#include <inttypes.h>
#include <vector>


inline std::vector<double> window;

inline double max = 0;

void makeHammingWindow(int windowSize);
void stft(int16_t *data, int size,
          int window, int step, int samplerate,
          double** res, int* resH, int* resW);
template<typename inptype>
void fft(inptype *data, int size, double* res);
namespace std {
template<typename inptype>
class complex;
}
template<typename inptype>
void complex_fft(inptype *data, int size, std::complex<double>* res);
void stfft(int16_t *data, int size,
          int window /* should be power of 2 */, int step,
          double** res, int* resW); // resH = window
void complex_stfft(int16_t *data, int size, int windowSize, int step, std::complex<double> **res, int *resW);

struct harmonic {
    double freq;
    double amp;
};

struct continuousHarmonic {
    harmonic h;
    int prev;
    int continuity;
    bool hasRightCounterpart;
};

std::vector<harmonic> maxes(double *data, int h, int w);
std::vector<std::vector<harmonic> > maxesLinear(double *data, int h, int w, int samplerate);

void shiftandmul(double *src, int h, int w, double** dest, int *resH);
void shiftandmulLinear(double *src, int h, int w, double** dest, int *resH);
void multiplyFundamentalLinear(double *src, int h, int w, double** dest, int *resH, int harmonics, double treshold);
void divideFundamentalLinear(double *src, int h, int w, double** dest, int *resH, int harmonics, double treshold);
void isolateMaxima(int w, double *transform, int h, double *data);

#endif // STFT_H
