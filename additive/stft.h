#ifndef STFT_H
#define STFT_H
#include <inttypes.h>
inline double max = 0;
void stft(int16_t *data, int size,
          int window, int step, int samplerate,
          double** res, int* resH, int* resW);

#endif // STFT_H
