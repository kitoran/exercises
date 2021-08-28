#ifndef GRAPH_H
#define GRAPH_H

#include <spectrogram.h>
#include <gtk/gtk.h>

//#include <synthesis.h>

struct graphClass {

};
struct _cairo_surface;
struct graph
{
    GtkWidget p;

    _Bool selecting;
    int selectStart;
    int selectEnd;
    struct _cairo_surface *spectrogramDrawing;
//    int spectrogramWidth;
//    int spectrogramHeight;
//    QWidget


};
void initializeGraph(struct graph *g);
int rangeEndInArray(struct graph*g);
int rangeStartInArray(struct graph*);
void paintEvent(struct graph*,GdkEventVisibility*event);
void resizeEvent(struct graph*,GdkEventConfigure  *event);
void selectRange(struct graph*);
void mouseMoveEvent(struct graph*,GdkEventMotion *event);
void mousePressEvent(struct graph*,GdkEventButton *event);
void mouseReleaseEvent(struct graph*,GdkEventButton *event);

#endif // GRAPH_H
