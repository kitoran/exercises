#ifndef STFT_H
#define STFT_H
#include <inttypes.h>
#include <vector>
inline double max = 0;
void stft(int16_t *data, int size,
          int window, int step, int samplerate,
          double** res, int* resH, int* resW);
struct harmonic {
    double freq;
    double amp;
};
std::vector<harmonic> maxes(double *data, int h, int w);

void shiftandmul(double *src, int h, int w, double** dest, int *resW);


#endif // STFT_H
