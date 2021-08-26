#ifndef GRAPH_H
#define GRAPH_H

#include <spectrogram.h>
#include <gtk/gtk.h>

//#include <synthesis.h>

struct graphClass {

};

struct graph
{
    GtkWidget parent;

    _Bool selecting;
    int selectStart;
    int selectEnd;
    struct GdkPixbuf *spectrogramDrawing;
//    QWidget


};
int rangeEndInArray(struct graph*g);
int rangeStartInArray(struct graph*);
void paintEvent(struct graph*,GdkEventVisibility*event);
void resizeEvent(struct graph*,GdkEventConfigure  *event);
void selectRange(struct graph*);
void mouseMoveEvent(struct graph*,GdkEventMotion *event);
void mousePressEvent(struct graph*,GdkEventButton *event);
void mouseReleaseEvent(struct graph*,GdkEventButton *event);

#endif // GRAPH_H
