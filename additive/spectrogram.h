#ifndef SPECTRUM_H
#define SPECTRUM_H
#include <inttypes.h>
#include <stft.h>
#include <gdk/gdk.h>


//enum class spectrogram_mode {linear, logarithmic};
struct Spectrogram
{
    int (*width)(void*);
    void (*draw)(void*, struct _cairo *, int, int);
    void (*fillBuffer)(void*, int16_t*, int bufferSize, int pos, uint64_t phase);
    double (*frequencyAtProportion)(void*, double p);
};
struct _cairo;
void dqrawAxes(struct Spectrogram *sg, struct _cairo *p, int w, int h);

struct MaximaSpectrogram
{
    const struct Spectrogram ff;
    struct harmonic**  maxima;
    double max;
};

struct ContMaximaSpectrogram
{
    const struct Spectrogram ff;
    struct continuousHarmonic** maxima;
    int harmonics;
};

struct LinearSpectrogram
{
    const struct Spectrogram ff;
    double* data;
    int width_;
    int height;
    double freqStep;
};

#endif // SPECTRUM_H
