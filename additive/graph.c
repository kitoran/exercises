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
void graph_onDraw(struct graph* g, cairo_t* cr,
                  gpointer user_data
                );
void initializeGraph(struct graph *g) {
    g->selectEnd = -1;
    g->selecting = false;
    g->selectStart = -1;
    g->spectrogramDrawing = NULL;
    g_signal_connect(G_OBJECT(g), "draw", G_CALLBACK( graph_onDraw), NULL);
//    g->spectrogramData = NULL;
  //  g->spectrogramWidth = 0;
   // g->spectrogramHeight = 0;
}

int rangeStartInArray(struct graph* g)
{
    return (double)(g->selectStart-10) / (gtk_widget_get_allocated_width(&g->p)-20) * spectrogram->width(spectrogram);
}
int rangeEndInArray(struct graph* g)
{
    return (double)(g->selectEnd-10) / (gtk_widget_get_allocated_width(&g->p)-20) * spectrogram->width(spectrogram);
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

void graph_onDraw(struct graph* g, cairo_t* cr,
                  gpointer user_data
                ) {
(void)user_data;
//    cairo_surface_t *image = cairo_image_surface_create_for_data(g->spectrogramDrawing,
//                                                                 CAIRO_FORMAT_ARGB32,
//                                                                 g->spectrogramWidth,
//                                                                 g->spectrogramHeight, 0);
    cairo_set_source_surface(cr, g->spectrogramDrawing, 0, 0);

    cairo_paint(cr);
//    cairo_surface_destroy(image);
//    gdk_draw_pixbuf(g, gc, , 0, 0, 0, 0, -1, -1, 0, 0, 0);
    if(g->selectStart >= 0) {
        cairo_set_source_rgba(cr,1, 1,1,0.3);
        cairo_rectangle(cr, g->selectStart, 0, g->selectEnd-g->selectStart, gtk_widget_get_allocated_width(&g->p));

    }

    //    QPainter p(this);
//    spectrogram->draw(&p, width(), height());
}

void resizeEvent(struct graph* g, GdkEventConfigure  *event)
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
        cairo_surface_destroy(g->spectrogramDrawing);
    }

    g->spectrogramDrawing = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, event->width, event->height);
    cairo_t* cr = cairo_create(g->spectrogramDrawing);
    spectrogram->draw(spectrogram, cr, event->width, event->height);

    cairo_destroy(cr);
//    gdk_gc_destroy(gc);
}

void selectRange(struct graph* g)
{
    g->selecting = true;

    GdkCursor* cursor;
    cursor = gdk_cursor_new_for_display(gdk_display_get_default(),
                            GDK_CROSS);
    gdk_window_set_cursor(gtk_widget_get_window(&g->p), cursor);
    g_object_unref(cursor);



//    QCursor c();
//    setCursor(Qt::CrossCursor);
}

void mouseMoveEvent(struct graph* g, GdkEventMotion *event) {
//    double sortabase = log(20000.0/30)/(width()-20);
//    double freq = 30 * exp(sortabase*x);

//    int indh = double(height()-event->y()-10) / (height()-20) * heights;
    int indw = (double)(event->x-10) / (gtk_widget_get_allocated_width(&g->p)-20) * spectrogram->width(spectrogram);
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
    if(indw < 0 || indw >= gtk_widget_get_allocated_width(&g->p)) {
        return;
    }
    struct message m;
    m.pos = indw;
//    qDebug() << "putting message:" << m.data.data << m.data.sized ;
    blockAndPut(&channel, &m, sizeof(m));

    if(g->selecting) {
        g->selectEnd = event->x;
        gtk_widget_queue_draw(&g->p);
    }
    //    fprintf(stderr, "indh %d indw %d freq %lf val %lf", indh, indw, freq, val);
//    fprintf(stderr, "freq %lf", freq);
}

void mousePressEvent(struct graph* g, GdkEventButton *event)
{
    int indw = (double)(event->x-10) / (gtk_widget_get_allocated_width(&g->p)-20) * spectrogram->width(spectrogram);
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
    (void)(event);
    struct message m = {-1};
//    qDebug() << "putting message:" << m.data.data << m.data.sized;
    blockAndPut(&channel, &m, sizeof(m));
    if(g->selecting) {
        g->selecting = false;

        GdkCursor* cursor;
        cursor = gdk_cursor_new_for_display(gdk_display_get_default(),
                                            GDK_CROSS);
        gdk_window_set_cursor(gtk_widget_get_window(&g->p), cursor);
        g_object_unref(cursor);

        if(g->selectEnd < g->selectStart) {
            typeof(g->selectStart) temp = g->selectStart;
            g->selectStart = g->selectEnd;
            g->selectEnd = temp;
        }

        struct ContMaximaSpectrogram* s = (struct ContMaximaSpectrogram*)spectrogram;
        resynthesizeMaxima(s, rangeStartInArray(g), rangeEndInArray(g));
        alsaPlayBlock(audioOutput, arrlen(audioOutput));
    }
}

