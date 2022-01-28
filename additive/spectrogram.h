#ifndef SPECTRUM_H
#define SPECTRUM_H
#include <inttypes.h>
#include <stft.h>
//#include <gdk/gdk.h>
#include "gui.h"

//enum class spectrogram_mode {linear, logarithmic};
struct Spectrogram
{
    int (*width)(void*);
    void (*draw)(void*, Painter *, int, int);
    void (*fillBuffer)(void*, int16_t*, int bufferSize, int pos, uint64_t phase);
    double (*frequencyAtProportion)(void*, double p);
};
//struct _cdCanvas;
//struct GuiImage;
void drawAxes(struct Spectrogram *sg, Painter* image, int w, int h);

struct MaximaSpectrogram
{
    struct Spectrogram ff;
    struct harmonic**  maxima;
    double max;
};
extern const struct Spectrogram MaximaSpectrogramVtable;


typedef struct ContMaximaSpectrogram
{
    struct Spectrogram ff;
    struct continuousHarmonic** maxima;
    int harmonics;
} ContMaximaSpectrogram;

extern const struct Spectrogram ContMaximaSpectrogramVtable;

typedef struct LinearSpectrogram
{
    struct Spectrogram ff;
    double* data;
    int width_;
    int height;
    double freqStep;
} LinearSpectrogram;
extern const struct Spectrogram LinearSpectrogramVtable;

#endif // SPECTRUM_H
