#include <X11/extensions/Xrender.h>
#include "spectrogram.h"
#include "globals.h"
#include "audio.h"
#include "mathext.h"
#include "graph.h"
extern uint framesPerPeriod;
typedef int16_t frameType; // fix

void demo(/*Picture * picture, */int position/*, int w, int h*/) {
    Picture * picture = & widget.spectrogramDrawingPicture;
    int w = widget.width;
    int h = widget.height;
    MaximaSpectrogram* s = spectrogram;
    harmonic* hs = s->maxima[position];
    frameType * buffer = (frameType *) malloc(sizeof(frameType)*framesPerPeriod);

    int phase = 0;
    double howLongOneHarmonic = 0.1;
    double howLongPeriod = (double)(framesPerPeriod)/alsaSampleRate;

    for(int whichHarmonic = 0; whichHarmonic < arrlen(hs); whichHarmonic++) {
        for(int j = 0; j < howLongOneHarmonic/
                            howLongPeriod; j++) {
            for(int k = 0; k < framesPerPeriod; k++) {
                double v = 0;
                for(int i = 0; i < whichHarmonic; i++) {
                    v += hs[i].amp*sinLookupTable[(int64_t)((hs[i].freq
                                            *phase/alsaSampleRate+i)*LOOKUP_TABLE_SIZE)
                            %LOOKUP_TABLE_SIZE]/max/*3*/;
                }
                phase++;
                buffer[j] = v*800/*/*400   /INT16_MAX*/;
            }
            writeFrames(buffer, framesPerPeriod);
        }

        XRenderColor c = {
            65535,
            0,
            0,
            hs[whichHarmonic].amp/max*65535
        };
//        guiDrawRectangle(r, left+10, h-10-top, right-left, top-bottom);
        double left = (w-20)/(double)(arrlen(s->maxima))*position;
        double right = (w-20)/(double)(arrlen(s->maxima))*(position+1);
        double bottom = (h-20)/(double)(cutoff)*hs[whichHarmonic].freq;
        double top = bottom+1;/*(h-20)/double(cutoff)*(j+1);*/

        XRenderFillRectangle(xdisplay, PictOpAtop,
                             *picture, &c, left+10, h-10-top, right-left, top-bottom);
        XFlush(xdisplay);
    }
    free(buffer);
}
