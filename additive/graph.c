﻿#include "graph.h"
#include "globals.h"
#include "spectrogram.h"
#include "synthesis.h"
#include "audio.h"
#include "math.h"
#include "gui.h"
#include "mathext.h"
#include "alsathread.h"
#include <X11/cursorfont.h>
#include <stdbool.h>
#include <time.h>
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

//#include <gdk/gdk.h>
//void graph_onDraw(struct graph* g, cairo_t* cr,
//                  gpointer user_data
//                );
void initializeGraph(struct graph *g) {
    g->selectEnd = -1;
    g->selecting = false;
    g->selectStart = -1;
    g->spectrogramDrawing = 0;
    g->width = g->height = 0;

    g->window = XCreateSimpleWindow(xdisplay,
                                    rootWindow,
                                    0,
                                    0,
                                    100,
                                    100,
                                    0,
                                    0,
                                    0x00000000
                                );
    fprintf(stderr, "created window %lud",
            g->window);
//    XRenderPictFormat * format =
//        XRenderFindStandardFormat (xdisplay,
//                       PictStandardRGB24);
//    g->spectrogramDrawing = XRenderCreatePicture (xdisplay, rootWindow,
//                              format,
//                              0,
//                              NULL);
    XSelectInput(xdisplay, g->window , 0);

    XMapWindow(xdisplay, g->window);
//    g->spectrogramData = NULL;
  //  g->spectrogramWidth = 0;
   // g->spectrogramHeight = 0;
}

int rangeStartInArray(struct graph* g)
{
    return (double)(g->selectStart-10) / (g->width-20) * spectrogram->width(spectrogram);
}
int rangeEndInArray(struct graph* g)
{
    return (double)(g->selectEnd-10) / (g->width-20) * spectrogram->width(spectrogram);
}

//void graph::setData(Spectrogram* data_) {
//    heights = height_;
//    data = data_;
//    widths = width_;
//    max = max_;
//    freqMax = freqMax_;
//    mode = spectrogram_mode::logarithmic;
//}

//void graph::setLinearData(double *data_, int width_, int height_, int windowSize_, int samplerate_, double max_) {
//    heights = height_;
//    data = {data_, height_*width_};
//    widths = width_;
//    max = max_;
//    samplerate = samplerate_;
//    mode = spectrogram_mode::linear;
//    windowSize = windowSize_;
//}

//void graph::setComplexData(std::complex<double> *data_, int width_, int height_, int windowSize_, int samplerate_, double max_) {
//    heights = height_;
//    data = {(double*)data_, height_*width_*2};
//    widths = width_;
//    max = max_;
//    samplerate = samplerate_;
//    mode = spectrogram_mode::linear;
//    complex = true;
//    windowSize = windowSize_;
//}

void drawGraph(graph *g,/* Painter *p,*/ int x, int y) {
//    cairo_surface_t *image = cairo_image_surface_create_for_data(g->spectrogramDrawing,
//                                                                 CAIRO_FORMAT_ARGB32,
//                                                                 g->spectrogramWidth,
//                                                                 g->spectrogramHeight, 0);
//    gdk_draw_pixbuf(g, gc, , 0, 0, 0, 0, -1, -1, 0, 0, 0);
//    fprintf(stderr, "move window to %d %d\n", x, y);
    XMoveWindow(xdisplay, g->window, x, y);
    XSetWindowBackground(xdisplay, g->window, 0xffffff);
    GC gc = XCreateGC(xdisplay, g->window, 0, NULL);
    XSetGraphicsExposures(xdisplay, gc, False);
    if(g->spectrogramDrawing) {
        XCopyArea(xdisplay, g->spectrogramDrawing, g->window,
                  gc, 0,0, g->width,
                  g->height, 0, 0);
    }
//    XClearWindow(xdisplay, g->window);

    Painter p = {
        g->window,
        gc
    };
    if(g->selectStart >= 0) {
        XRenderColor c = {
            10000,
            10000,
            10000,
            10000
        };
//        guiSetForeground65535(&p, 30000, 65535, 65535, 65535);
        XRenderFillRectangle(xdisplay, PictOpAtop,
                g->spectrogramDrawingPicture, &c,
                g->selectStart, 0,
                g->selectEnd-g->selectStart, g->height);
    }
    XFreeGC(xdisplay, gc);
    //    QPainter p(this);
//    spectrogram->draw(&p, width(), height());
}

void redrawSpectrogram(struct graph* g)
{
//    fprintf(stderr, "real %d %d need %d", width()-20, height()-20, spectrogram->width());
//    g->spectrogramData = realloc(g->spectrogramData, event->width * event->height);
//    g->spectrogramHeight = event->height;
//    g->spectrogramWidth = event->width;

//    cairo_surface_t *image = cairo_image_surface_create_for_data(g->spectrogramData,
//                                                                 CAIRO_FORMAT_ARGB32,
//                                                                 g->spectrogramWidth,
//                                                                 g->spectrogramHeight, 0);
    if(g->spectrogramDrawing) {
        XFreePixmap(xdisplay, g->spectrogramDrawing);
        XRenderFreePicture(xdisplay, g->spectrogramDrawingPicture);
    }
    fprintf(stderr, "createpixmap %d x %d \n", g->width, g->height);
    g->spectrogramDrawing = XCreatePixmap(xdisplay, rootWindow, g->width, g->height, xDepth);

    Painter p = {
        g->spectrogramDrawing,
        XCreateGC(xdisplay, g->spectrogramDrawing, 0, NULL)
    };
    guiFillRectangle(&p, 0, 0, g->width, g->height);


    clock_t start, end;
    double cpu_time_used;
    start = clock();
    MaximaSpectrogramVtable.draw(spectrogram, &p, g->width, g->height);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    fprintf(stderr, "drawing took %lf seconds\n", cpu_time_used);

    XFreeGC(xdisplay, p.gc);


    fprintf(stderr, "resize window to %d %d\n", g->width, g->height);
    XResizeWindow(xdisplay, g->window, g->width, g->height);

    XRenderPictFormat * format =
        XRenderFindStandardFormat (xdisplay,
                       PictStandardRGB24);
    g->spectrogramDrawingPicture = XRenderCreatePicture (xdisplay, g->window,
                              format,
                              0,
                              NULL);
//    gdk_gc_destroy(gc);
}

void selectRange(struct graph* g)
{
    g->selecting = true;
    Cursor c = XCreateFontCursor(xdisplay, XC_cross);
    XDefineCursor(xdisplay, g->window, c);
    XFreeCursor(xdisplay, c);
}

void mouseMoveEvent(struct graph* g, int x) {
//    double sortabase = log(20000.0/30)/(width()-20);
//    double freq = 30 * exp(sortabase*x);

//    int indh = double(height()-event->y()-10) / (height()-20) * heights;
    int indw = (double)(x-10) / (g->width-20) * spectrogram->width(spectrogram);
    fprintf(stderr, "moveevent %d (of %d), %d\n", x, g->width, indw);
//    double freq = 0;// = freqMin * log(freqMax/freqMin)/log(double(height()-event->y()-10) / (height()-20));//pow(frequencyMultiplent, indh);
//    if(mode == spectrogram_mode::linear) {
//        double(y) / (height()-20) * 1000;
//        freq = double(height()-event->y()-10) / (height()-20)*samplerate;
//    }
    //    double val;
//    int ind = indw*heights+indh;
//    if(ind < 0 || ind >= heights*widths) {
//        val = nan("hello");
//    } else {
//        val = data[ind];
//    }
    if(indw < 0 || indw >= spectrogram->width(spectrogram)) {
        return;
    }
    struct message m;
    m.pos = indw;
//    qDebug() << "putting message:" << m.data.data << m.data.sized ;
    blockAndPut(&channelForPlayback, &m, sizeof(m));

    if(g->selecting) {
        g->selectEnd = x;
        guiRedraw();
    }
    //    fprintf(stderr, "indh %d indw %d freq %lf val %lf", indh, indw, freq, val);
//    fprintf(stderr, "freq %lf", freq);
}

void mousePressEvent(struct graph* g, int x)
{
    int indw = (double)(x-10) / (g->width-20) * spectrogram->width(spectrogram);
    struct message m = {indw};
//    if(complex) {
//        m = message{data.slicec(indw*heights, heights), mode, true};
//    } else {
//        m = {data.sliced(indw*heights, heights), mode, true};
//    }
//    qDebug() << "putting message:" << m.data.data << m.data.sized ;
    blockAndPut(&channelForPlayback, &m, sizeof(m));
    if(g->selecting) {
        if(xEvent.xbutton.button == Button3) {
            g->selectStart = g->selectEnd = -1;
            g->selecting = false;
        }
        g->selectStart = g->selectEnd = x;

        Cursor c = XCreateFontCursor(xdisplay, XC_arrow);
        XDefineCursor(xdisplay, g->window, c);
        XFreeCursor(xdisplay, c);

        guiRedraw();
    }
//    fprintf(stderr, "heights %d\n", heights);
//    for(int i = 0; i < heights; i++) {
//        if(data.c(indw*heights+i) != 0.) {
//            qDebug() << i << data.c(indw*heights+i) <<
//                    double(samplerate)/windowSize*i;

//        }
//    }
}

void mouseReleaseEvent(struct graph* g)
{
    struct message m = {-1};
//    qDebug() << "putting message:" << m.data.data << m.data.sized;
    blockAndPut(&channelForPlayback, &m, sizeof(m));
    if(g->selecting) {
        g->selecting = false;

        Cursor c = XCreateFontCursor(xdisplay, XC_arrow);
        XDefineCursor(xdisplay, g->window, c);
        XFreeCursor(xdisplay, c);
        if(g->selectEnd < g->selectStart) {
            typeof(g->selectStart) temp = g->selectStart;
            g->selectStart = g->selectEnd;
            g->selectEnd = temp;
        }

        struct ContMaximaSpectrogram* s = (struct ContMaximaSpectrogram*)spectrogram;
        resynthesizeMaxima(s, rangeStartInArray(g), rangeEndInArray(g));
        alsaPlayBlock(audioOutputStb, arrlen(audioOutputStb));
    }
}


void graphProcessEvent(graph *g, /*Painter *p, */int x, int y, int w, int h) {
//    if(xEvent.type == Expose) {
        drawGraph(g, /*p,*/ x, y);
   /* } else*/
    int nw = MAX(25, w);
    int nh = MAX(25, h);
    if(nw != g->width || nh != g->height) {
        g->width = nw;
        g->height = nh;
        redrawSpectrogram(g);
    }
    if((xEvent.type == MotionNotify ||
              xEvent.type == ButtonPress ||
              xEvent.type == ButtonRelease) &&
              xEvent.xmotion.x >= x && xEvent.xmotion.x <= x+w &&
              xEvent.xmotion.y >= y && xEvent.xmotion.y <= y+h) {
        if(xEvent.type == MotionNotify) {
            mouseMoveEvent(g, xEvent.xmotion.x - x);
        } else if(xEvent.type == ButtonPress) {
            mousePressEvent(g, xEvent.xbutton.x - x);
        } else if(xEvent.type == ButtonRelease) {
            mouseReleaseEvent(g);
        }
    }
}

//void graphRedrawSpectrogram(graph *g)
//{
//    Painter p = {
//        g->spectrogramDrawing,
//        XCreateGC(xdisplay, g->spectrogramDrawing, 0, NULL)
//    };
//    guiFillRectangle(&p, 0, 0, g->width, g->height);
//    spectrogram->draw(spectrogram, &p, g->width, g->height);
//    XFreeGC(xdisplay, p.gc);

//    guiRedraw();
//}
