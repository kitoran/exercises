#include "graph.h"
#include "globals.h"
#include "spectrogram.h"
#include "synthesis.h"
#include "audio.h"
#include "math.h"
#include "mathext.h"
#include "alsathread.h"
#include <stdbool.h>
#include <gdk/gdk.h>

initializeGraph(struct graph *g) {
    g->selectEnd = -1;
    g->selecting = false;
    g->selectStart = -1;
    g->spectrogramDrawing = NULL;
}

int rangeStartInArray(struct graph* g)
{
    return (double)(g->selectStart-10) / (gtk_widget_get_allocated_width(&g->parent)-20) * spectrogram->width(spectrogram);
}
int rangeEndInArray(struct graph* g)
{
    return (double)(g->selectEnd-10) / (gtk_widget_get_allocated_width(&g->parent)-20) * spectrogram->width(spectrogram);
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

void paintEvent(struct graph* g, GdkEventVisibility* event) {

    struct gdk_gc* gc = gdk_gc_new (g);
    gdk_draw_pixbuf(g, gc, g->spectrogramDrawing, 0, 0, 0, 0, -1, -1, 0, 0, 0);
    if(g->selectStart >= 0) {
        GdkRGBA c;
        c.alpha = 0.3;
        c.blue = 1;
        c.green = 1;
        c.red = 1;
        gdk_gc_set_foreground(gc, c);
        gdk_gc_set_opacity(gc, 0.3);
        gdk_draw_rectangle(gc, g->selectStart, 0, g->selectEnd-g->selectStart, gtk_widget_get_allocated_height(g));

    }
    gdk_gc_free(gc);
//    QPainter p(this);
//    spectrogram->draw(&p, width(), height());
}

void resizeEvent(struct graph* g, GdkEventConfigure  *event)
{
//    fprintf(stderr, "real %d %d need %d", width()-20, height()-20, spectrogram->width());
    g->spectrogramDrawing = gdk_image_new(0, g,
                event->width, event->height);
    struct gdk_gc* gc = gdk_gc_new (g);
    spectrogram->draw(spectrogram, gc, width(), height());

    gdk_gc_free(gc);
//    gdk_gc_destroy(gc);
}

void selectRange(struct graph* g)
{
    g->selecting = true;

    GdkCursor* cursor;
    cursor = gdk_cursor_new(GDK_CROSS);
    gdk_window_set_cursor(g, cursor);
    gdk_cursor_destroy(cursor);



//    QCursor c();
//    setCursor(Qt::CrossCursor);
}

void mouseMoveEvent(struct graph* g, GdkEventMotion *event) {
//    double sortabase = log(20000.0/30)/(width()-20);
//    double freq = 30 * exp(sortabase*x);

//    int indh = double(height()-event->y()-10) / (height()-20) * heights;
    int indw = (double)(event->x-10) / (gtk_widget_get_allocated_width(&g)-20) * spectrogram->width(spectrogram);
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
    if(indw < 0 || indw >= width()) {
        return;
    }
    struct message m;
    m.pos = indw;
//    qDebug() << "putting message:" << m.data.data << m.data.sized ;
    blockAndPut(&channel, &m, sizeof(m));

    if(g->selecting) {
        g->selectEnd = event->x;
        repaint();
    }
    //    fprintf(stderr, "indh %d indw %d freq %lf val %lf", indh, indw, freq, val);
//    fprintf(stderr, "freq %lf", freq);
}

void mousePressEvent(struct graph* g, GdkEventButton *event)
{
    int indw = (double)(event->x-10) / (gtk_widget_get_allocated_width(&g->parent)-20) * spectrogram->width(spectrogram);
    struct message m = {indw};
//    if(complex) {
//        m = message{data.slicec(indw*heights, heights), mode, true};
//    } else {
//        m = {data.sliced(indw*heights, heights), mode, true};
//    }
//    qDebug() << "putting message:" << m.data.data << m.data.sized ;
    blockAndPut(&channel, &m, sizeof(m));
    if(g->selecting) {
        if(event->button == 3) {
            g->selectStart = g->selectEnd = -1;
            g->selecting = false;
        }
        g->selectStart = g->selectEnd = event->x;
    }
//    fprintf(stderr, "heights %d\n", heights);
//    for(int i = 0; i < heights; i++) {
//        if(data.c(indw*heights+i) != 0.) {
//            qDebug() << i << data.c(indw*heights+i) <<
//                    double(samplerate)/windowSize*i;

//        }
//    }
}

void mouseReleaseEvent(struct graph* g, GdkEventButton *event)
{
    struct message m = {-1};
//    qDebug() << "putting message:" << m.data.data << m.data.sized;
    blockAndPut(&channel, &m, sizeof(m));
    if(g->selecting) {
        g->selecting = false;

        GdkCursor* cursor;
        cursor = gdk_cursor_new(GDK_ARROW);
        gdk_window_set_cursor(g, cursor);
        gdk_cursor_destroy(cursor);

        if(g->selectEnd < g->selectStart) {
            typeof(g->selectStart) temp = g->selectStart;
            g->selectStart = g->selectEnd;
            g->selectEnd = temp;
        }

        struct ContMaximaSpectrogram* s = spectrogram;
        resynthesizeMaxima(s, rangeStartInArray(g), rangeEndInArray(g));
        alsaPlayBlock(audioOutput, arrlen(audioOutput));
    }
}

