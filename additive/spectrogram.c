#include "spectrogram.h"
#include "globals.h"
#include "mathext.h"
#include "audio.h"
#include "soundext.h"
#include <stdbool.h>
#include "gui.h"

//draw() {
//p.setPen(transparent());
//double cutoff = 10000;//samplerate;//10000.0;//*heights/samplerate;
//double cutoffind = mode == spectrogram_mode::linear?(cutoff*heights/44100)
//            :log(cutoff/freqMin)/log(frequencyMultiplent);//samplerate;

//for(int i = 0; i < widths; i++) {
//    for(int j = 0; j < cutoffind; j++) {
//        double left = (width()-20)/double(widths)*i;
//        double right = (width()-20)/double(widths)*(i+1);
//        double bottom = (height()-20)/double(cutoffind)*j;
//        double top = (height()-20)/double(cutoffind)*(j+1);
//        double value = complex?std::abs(data.c(i*heights+j))
//                             :data.d(i*heights+j);
//        int g = value/max*255;
//        p.setBrush(QColor(g,g,g));
//        if(g < 0 || g > 255) {
//            qDebug() << "i" << i << "j" << j << "amplitude" << value // data.c(i*heights+j)
//                     << "max" << max <<
//                        "color" << g;
//        }
//        p.drawRect(QRectF(left+10, height()-10-top, right-left, top-bottom));
//    }
//}
//}

GdkRGBA transparent() {
    static GdkRGBA r;
    static bool ined = false;
    if(!ined) {
        bool rt
            = gdk_rgba_parse(&r, "transparent");
           assert(rt);
           ined=true;
    }
    return r;
}
GdkRGBA black() {
    static GdkRGBA r;
    static bool ined = false;
    if(!ined) {
        bool rt
            = gdk_rgba_parse( &r, "black");
           assert(rt);
           ined=true;
    }
    return r;
}

void drawAxes(struct Spectrogram* sg, Painter* image, int w, int h) {
    guiSetForeground(image, 0xff880088);
//    double sortabase = log(freqMax/freqMin)/(height()-20);

//    guiDrawLine(image, 10, w-10, 10, h-10-y);

    for(int y = 0; y < h-20; y += 100) {
        double freq;
//        if(mode == spectrogram_mode::logarithmic) {
//             freq = 30 * exp(sortabase*y);
//        } else {
//             freq = double(y) / (height()-20) * cutoff;
//        }
        freq = sg->frequencyAtProportion(sg, (double)(y) / (h-20));
        char text[20];
        guiDrawLine(image,
                      10, h-10-y,
                       w-10, h-10-y);
        int len = sprintf(text, "%2.lf", freq);
        guiDrawTextWithLen(image, 10, h-10-y, text, len);
    }
}




//prepareFrequenciesAndIntSpectrum(){
//logWindowSize = intLog2(spectrumSize);
//int size = spectr.data.sized;
//if(spectr.complex) size /= 2;
//if(size > arrlen(frequencies) && spectr.mode == spectrogram_mode::logarithmic) {
//    int i = arrlen(frequencies);
//    for(; i < size; i++) {
//        double mul = pow(frequencyMultiplent, i);
//        double prod = freqMin * mul;
//        frequencies.push_back(prod);
//    }
//}
//spectrumInt.resize(spectrumSize);
//for(int i = 0; i < spectrumSize; i++) {
//    spectrumInt[i] = spectr.data.d(i)*(INT16_MAX/16)/max;
//}
//}


//fillBuffer() {

//    if(spectr.mode == spectrogram_mode::logarithmic) {
//        for(int j = 0; j < framesPerPeriod; j++) {
//            int v = 0;
//            for(int i = 0; i < spectrumSize; i++) {
//                v += spectrumInt[i]*lookup[int((frequencies[i]/alsaSampleRate
//                                        *phase+i)*LOOKUP_TABLE_SIZE)%LOOKUP_TABLE_SIZE];
//            }
//            phase++;
//            buffer[j] = v/INT16_MAX;
//        }
//    }  else  {

//        for(int j = 0; j < framesPerPeriod; j++) {
//            int v = 0;
////                    static float r[] = {220, 261.6, 329.6};
//            for(int i = 0; i < 3/*spectrumSize*/; i++) {
//                v += /*spectrumInt*/r[i]*lookup[int((1.0/spectrumSize*i
//                                        *phase+i)*LOOKUP_TABLE_SIZE)%LOOKUP_TABLE_SIZE]/*/max*3*/;
//            }
//            phase++;
//            buffer[j] = v/*/*400*/   /INT16_MAX;



//        }
////                int written = 0;
////                while(spectr.windowSize > 0 && written < framesPerPeriod) {
////                    int amount = std::min(framesPerPeriod-written,
////                                          spectr.windowSize);
////                    memcpy(buffer, &unFft[unfftIndex], amount);
////                    for(int i = 0; i < amount; i++) {
////                        buffer[i] = unFft[unfftIndex + i];
////                    }
////                    written+=amount;
////                    unfftIndex += amount;
////                    unfftIndex &= (spectr.windowSize-1);
////                }
//    } /**/

//}

void MaximaSpectrogramdraw(struct MaximaSpectrogram* self, Painter *r, int w, int h)
{
    guiSetForeground(r, 0x0000000);
//    cairo_set_source_rgb(p, 0, 0, 0);
    guiDrawRectangle(r, 10, 10, w-20, h-20);
    int spectrWidth = arrlen(self->maxima);
    for(int i = 0; i < spectrWidth; i++) {
        for(int j = 0; j < arrlen(self->maxima[i]); j++) {
            struct harmonic har = self->maxima[i][j];
            double left = (w-20)/(double)(spectrWidth)*i;
            double right = (w-20)/(double)(spectrWidth)*(i+1);
            double bottom = (h-20)/(double)(cutoff)*har.freq;
            double top = bottom+1;/*(h-20)/double(cutoff)*(j+1);*/
            double value = self->maxima[i][j].amp;
            int g = value/max*255;
//            if(g > 100) {
//                qDebug() << g;
//            }
            guiSetForeground(r, rgb(g,g,g));
            if(g < 0 || g > 255) {
                fprintf(stderr, "i %d j %d amplitude %lf max %lf color %d",
                        i, j, value, max, g);
            }
            guiDrawRectangle(r, left+10, h-10-top, right-left, top-bottom);
        }
    }
    drawAxes(&self->ff, r, w, h);
}

void MaximaSpectrogramfillBuffer(struct MaximaSpectrogram* self, int16_t *buffer, int bufferSize, int pos, unsigned int phase)
{
    struct harmonic* hs = self->maxima[pos];
    int size = arrlen(self->maxima[pos]);
//    double ph  = phase;
    for(int j = 0; j < bufferSize; j++) {
        double v = 0;
        for(int i = 0; i < size; i++) {
//            struct harmonic deb32 = hs[i];
//            int64_t deb = ((hs[i].freq
//                        *phase+i)*LOOKUP_TABLE_SIZE);
//            int64_t deb15 = (int64_t)((hs[i].freq
//                        *phase+i)*LOOKUP_TABLE_SIZE)%LOOKUP_TABLE_SIZE;
//            int64_t deb2 = sinLookupTableInt[(int64_t)((hs[i].freq
//                                   *phase+i)*LOOKUP_TABLE_SIZE)
//                   %LOOKUP_TABLE_SIZE];
//            double deb3 = hs[i].amp;
            v += hs[i].amp*sinLookupTable[(int64_t)((hs[i].freq
                                    *phase/alsaSampleRate+i)*LOOKUP_TABLE_SIZE)
                    %LOOKUP_TABLE_SIZE]/max/*3*/;
        }
        phase++;
        buffer[j] = v*800/*/*400   /INT16_MAX*/;
    }
}

//int* MaximaSpectrogram::lookup = sinLookupTableInt();

double MaximaSpectrogramfrequencyAtProportion(double proportion)
{
    return proportion*cutoff;
}

double ContMaximaSpectrogramfrequencyAtProportion(struct ContMaximaSpectrogram* self, double proportion)
{
    (void)self;
    return proportion*cutoff;
}

void LinearSpectrogramdraw(struct LinearSpectrogram* self, Painter* r, int w, int h)
{

    guiSetForeground(r, 0x0000000);
    guiDrawRectangle(r, 10, 10, w-20, h-20);
    int cutoffIndex = cutoff/self->freqStep;
//    qDebug() << "final freq is" << indToFftFreq(h,
    for(int i = 0; i < self->width_; i++) {
        for(int j = 0; j < cutoffIndex; j++) {

            double left = (w-20)/(double)(self->width_)*i;
            double right = (w-20)/(double)(self->width_)*(i+1);
            double bottom = (h-20)/(double)(cutoffIndex)*j;
            double top = (h-20)/(double)(cutoffIndex)*(j+1);
            if(j >= self->height) {

                guiSetForeground(r, rgbf(0,0,0.5));
            } else {
                double value = self->data[i*self->height+j];
                double norm = value/max;
                guiSetForeground(r, rgbf(norm,norm,norm));
//                gdk_set_background(gc, color);
                if(norm < 0 || norm > 1) {
                    fprintf(stderr, "i %d j %d amplitude %lf max %lf color %lf",
                            i, j, value, max, norm);
                }
            }
//            if(g > 100) {
//                qDebug() << g;
//            }

            guiDrawRectangle(r, left+10, h-10-top, right-left, top-bottom);
        }
    }
    drawAxes(&self->ff,r,w,h);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void LinearSpectrogramfillBuffer(struct LinearSpectrogram* self,int16_t *z, int bufferSize, int pos, unsigned int phase)
{
    abort();
}

double LinearSpectrogramfrequencyAtProportion(struct LinearSpectrogram* self, double proportion)
{
    return proportion*cutoff;
}

void ContMaximaSpectrogramdraw(struct ContMaximaSpectrogram* self, Painter *r, int w, int h)
{
//    cdCanvasSetForeground(canvas, CD_TRANSPARENT);
    //set_foreground

    guiSetForeground(r, 0x0000000);
    guiDrawRectangle(r, 10, w-20, 10, h-20);
    int spectrWidth = arrlen(self->maxima);
    for(int i = 1; i < spectrWidth; i++) {
        for(int j = 0; j < arrlen(self->maxima[i]); j++) {
            struct continuousHarmonic har = self->maxima[i][j];
            double rightX = (w-20)/(double)(spectrWidth)*(i+1);
            double rightY = (h-20)/(double)(cutoff)*har.h.freq;
            double leftX = (w-20)/(double)(spectrWidth)*(i);
            double leftY = har.prev == -1? rightY: (h-20)/(double)(cutoff)*self->maxima[i-1][har.prev].h.freq;
//            double value = self->maxima[i][j].h.amp;
//            int g = value/max*255;
//            if(g > 100) {
//                qDebug() << g;
//            }
            uint colors[] = {
                rgb(0,0,255),
                rgb(0,255,0),
                rgb(0,255,255),
                rgb(255,0,0),
                rgb(255,0,255),
                rgb(255,255,0)
            };
#define countof(a) ((sizeof((a)))/(sizeof(*(a))))
            guiSetForeground(r, colors[self->maxima[i][j].continuity % countof(colors)]);


//            if(g < 0 || g > 255) {
//                qDebug() << "i" << i << "j" << j << "amplitude" << value // data.c(i*heights+j)
//                         << "max" << max <<
//                            "color" << g;
//            }
            guiDrawLine(r, leftX+10, h-10-leftY, rightX+10, h-10-rightY);
        }
    }
    drawAxes(&self->ff, r,w,h);
}

void ContMaximaSpectrogramfillBuffer(struct ContMaximaSpectrogram* self, int16_t *buffer, int bufferSize, int pos, uint64_t phase)
{
    struct continuousHarmonic* hs = self->maxima[pos];
    int size = arrlen(self->maxima[pos]);
//    double ph  = phase;
    for(int j = 0; j < bufferSize; j++) {
        double v = 0;
        for(int i = 0; i < size; i++) {
//            auto deb32 = hs[i];
//            int64_t deb = ((hs[i].freq
//                        *phase+i)*LOOKUP_TABLE_SIZE);
//            int64_t deb15 = int64_t((hs[i].freq
//                        *phase+i)*LOOKUP_TABLE_SIZE)%LOOKUP_TABLE_SIZE;
//            int64_t deb2 = sinLookupTableInt[int64_t((hs[i].freq
//                                   *phase+i)*LOOKUP_TABLE_SIZE)
//                   %LOOKUP_TABLE_SIZE];
//            double deb3 = hs[i].h.amp;
            v += hs[i].h.amp*sinLookupTable[(int64_t)((hs[i].h.freq
                                    *phase/alsaSampleRate+i)*LOOKUP_TABLE_SIZE)
                    %LOOKUP_TABLE_SIZE]/max/*3*/;
        }
        phase++;
        buffer[j] = v*800/*/*400   /INT16_MAX*/;
    }
}

int MaximaSpectrogramWidth(struct MaximaSpectrogram*self) {
    return (arrlen(self->maxima));
}

int ContMaximaSpectrogramwidth(struct ContMaximaSpectrogram*self) {
    return arrlen(self->maxima);
}

int LinearSpectrogramwidth(struct LinearSpectrogram*self) {
    return self->width_;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
const struct Spectrogram contMaximaSpectrogramVtable = {
    ContMaximaSpectrogramwidth,
    ContMaximaSpectrogramdraw,
    ContMaximaSpectrogramfillBuffer,
    ContMaximaSpectrogramfrequencyAtProportion
};
#pragma GCC diagnostic pop
