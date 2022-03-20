#ifndef GRAPH_H
#define GRAPH_H

#include <spectrogram.h>
#include <X11/extensions/Xrender.h>
#include "gui.h"
#include "shittyintrospection.h"
//#include <synthesis.h>
INTROSPECT_ENUM(MouseAction,
                         actionPlay,
                         actionDemo)
extern MouseAction action;
typedef struct graph
{
    int width, height;
    _Bool selecting;
    int selectStart;
    int selectEnd;
    Pixmap spectrogramDrawing;
    Picture spectrogramDrawingPicture;
    Window window;
//    int spectrogramWidth;
//    int spectrogramHeight;
//    QWidget
} graph;

void graphProcessEvent(graph* g, int x, int y, int w, int h);
void initializeGraph(struct graph *g);
void redrawSpectrogram(struct graph *g);
//int rangeEndInArray(struct graph*g);
//int rangeStartInArray(struct graph*);
void selectRange(struct graph*);
//void drawSpectrogram(struct graph*);
//void drawGraph(struct graph*, int x, int y);
//void resizeEvent(struct graph*);
//void mouseMoveEvent(struct graph*);
//void mousePressEvent(struct graph*);
//void mouseReleaseEvent(struct graph*);

#endif // GRAPH_H
