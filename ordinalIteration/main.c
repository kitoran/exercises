#include <stdio.h>
#include <string.h>
#include <settings.h>
#include <float.h>
#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include <X11/extensions/Xdbe.h>
//char appName[] = "ordinalIteration";
#include "gui.h"
#include <time.h>
#include "gridlayout.h"
#include "toolbuttongroup.h"
#include "shittyintrospection.h"
//#include "timeAFunction.h"
#include "math.h"
#include "persistent.h"
#include "color.h"
#include "complex.h"

#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)<(y)?(x):(y))

#define tau 6.283185307179586
//struct Ordinal {
//    int val[6];
//}
int iters[6] = {0};

void ordinalEdit(Painter*p/*, Ordinal max*/) {
//    Point pos = getPos();
//    Size size = {500,
//                30};
}

XEvent xEvent;
int width = 600;
int height = 600;
#define WIDTH_MAX 2000
#define HEIGHT_MAX 2000
unsigned char data[WIDTH_MAX*HEIGHT_MAX*4];
unsigned char dataWithSelection[WIDTH_MAX*HEIGHT_MAX*4];
int itNum = 2;
double c = 8;
double xm = 4;
double ym = 4;
void picToNumDD (double x, double y, double* restrict rx, double* restrict ry) {
    int scale = MIN(width, height);
    *rx = x*c/scale - xm;
    *ry = c - y*c/scale - ym;
}
void numToPicDD (double x, double y,double * restrict rx, double * restrict ry) {
    int scale = MIN(width, height);
    *rx = (x+xm)/c*scale;
    *ry = -(y +ym - c)/c*scale;
}
void numToPicDI64 (double x, double y,long long int* restrict rx, long long int* restrict ry) {
    int scale = MIN(width, height);
    *rx = round((x+xm)/c*scale);
    *ry = round(-(y +ym - c)/c*scale);
}
void recalculatePicture();
char text[30];
INTROSPECT_ENUM_FILENAMES( modeEnum, selectMode, select.png, moveMode, move.png, pointMode, point.png);
modeEnum mode;
Grid mainGrid;
void zoomin()
{
    c /= 2;
    xm = xm - c/2;
    ym = ym - c/2;
    recalculatePicture();
}

void zoomout()
{
    xm = xm + c/2;
    ym = ym + c/2;
    c *= 2;
    recalculatePicture();
}
int startx, starty;

void loop(Painter* pa, bool* consume) {
//    popGrid();
    setCurrentGridPos(&mainGrid, 0,0);
//    bool res = false;
    bool recalc = persistentNumberEdit(pa, 5, itNum, consume);

    gridNextColumn(&mainGrid);
    if(resourseToolButton(pa, "minus.png", consume)) {
        itNum--;
        SAVE_INT(itNum);
        recalc = /*res = */true;
    }
    gridNextColumn(&mainGrid);
    if(resourseToolButton(pa, "plus.png", consume)) {
        itNum++;
        SAVE_INT(itNum);
        recalc = /*res = */true;
    }
//    gridNextColumn();
//    resourseModeToolButton(pa, "select.png", consume);
//        mode = selectMode;
//        res = true;
//    }
    gridNextColumn(&mainGrid);
//    if(resourseToolButton(pa, "iters.png", consume)) {
//        recalc = true;
//        mode = itersMode;
////        res = true;
//    }
    gridNextColumn(&mainGrid);
    if(resourseToolButton(pa, "100percent.png", consume)) {
        c = 1;
        xm = 0;
        ym = 0;
        recalc/* = res */= true;
    }
    gridNextColumn(&mainGrid);
    if(resourseToolButton(pa, "zoomin.png", consume)) {
        zoomin();
    }
    gridNextColumn(&mainGrid);
    if(resourseToolButton(pa, "zoomout.png", consume)) {
        zoomout();
    }

    gridNextColumn(&mainGrid);
    if(persistentToolButtonGroup(pa, modeEnum, mode)) {
        startx = starty = -1;
    }

    gridNextColumn(&mainGrid);
    guiLabelZTWithBackground(pa, text, true);

    gridNextColumn(&mainGrid);
    STATIC(Grid, exprGrid, allocateGrid(12, 1, 1));

    exprGrid.gridStart = getPos();
    pushGrid(&exprGrid);
    setCurrentGridPos(&exprGrid, 0,0);

    char* sups[] = {"​", "", "²","³",  "⁴", "⁵"};
    for(int exponent = 5; exponent > 0; exponent--) {
        char* label[30];
        int len = snprintf(label, 30, "ω%s+", sups[exponent]);
        recalc |= persistentNumberEdit_(pa, 2, iters+exponent, label, consume);
        gridNextColumn(&exprGrid);
        guiLabelWithBackground(pa, label, len, true);
        gridNextColumn(&exprGrid);
    }
    recalc |= persistentNumberEdit(pa, 2, iters, consume);
    Size size; size.width = getGridWidth(&exprGrid);
               size.height = getGridHeight(&exprGrid);

    popGrid();
    feedbackSize(size);
//    guiLabel(pa, text, true);


    if(recalc) {
        recalculatePicture();
//        guiRedraw();
    }

//    return res;
}

int grey(int v) {
    int vv = v | v << 8;
    return vv | vv<<8;
}
int greyf(double v) {
    return grey(v*255);
}
double functionC (double x) {
    return -1/(x-1)-1;
}
double functionI (double x) {
    return 1-1/(x+1);
}
double functionInit (double x) {
    return x+1;
}
//typedef double (dtd)(double);
typedef long long int i64;
double nthfunction(int n, double x) {
    if(n == 0) {
        return x+1;
    }
    double intgr, fr = modf(x, &intgr);
    if(x < 0) { fr = 1+fr; intgr--; }
    return functionI(nthfunction(n-1, functionC(fr))) + intgr;
}
double function (double x) {
    return nthfunction( itNum,x);
}
const double asin13 = 0.3398369094;
const double sqrt2 = 1.4142135623730951;
int* pixel(int x, int y) {
    static int oob;
    if(x >= 0 && x < width && y >= 0 && y < height) {
        return (int*)(data+ (y*WIDTH_MAX+x)*4);
    } else {
        return &oob;
    }
}
int* pixelWithSelection(int x, int y) {
    static int oob;
    if(x >= 0 && x < width && y >= 0 && y < height) {
        return (int*)(dataWithSelection+ (y*WIDTH_MAX+x)*4);
    } else {
        return &oob;
    }
}
#define CORNER ((tau/4-asin13*2)*1.5/sqrt2)
void drawSmallCircle(int* getPixel(int , int ), int xp, int yp, int c) {
    *getPixel(xp+1, yp-1) = *getPixel(xp+1, yp)= *getPixel(xp, yp+1)=*getPixel(xp, yp-2) = //greyf(side);
    *getPixel(xp-1, yp+1) = *getPixel(xp-1, yp-2)=*getPixel(xp-2, yp-1)=*getPixel(xp-2, yp) = greyf(CORNER) & c;
    *getPixel(xp, yp-1) = *getPixel(xp, yp)= *getPixel(xp-1, yp-1)=*getPixel(xp-1, yp) = c;
}
#define SMALLFLOAT 0.001
void recalculatePicture() {
    int start = clock();

    memset(data, 30, WIDTH_MAX*height*4);
    double dummy;
    double x;
    {
        int i = 0;
        double xp, yp;
        picToNumDD(i,0,&x,&dummy);
        numToPicDD(x, x, &xp, &yp);
        int ys = round(yp);
        for(; i < width; i++) {
            *pixel(i, ys-i) = 0xffff0000;
        }
    }
    picToNumDD(0,0,&x,&dummy);
    double lasty = NAN;
    for(int i = 0; i < width - 1; i++) {
        double xp, yp;
        picToNumDD(i,0,&x,&dummy);
        numToPicDD(x, function(x), &xp, &yp);
        bool jump = fabs(yp-lasty) > 1+SMALLFLOAT;
        if(jump) {
            drawSmallCircle(pixel, xp, yp, 0xffffffff);

            *pixel(xp+1, lasty-1) = *pixel(xp+1, lasty)= *pixel(xp, lasty+1)=*pixel(xp, lasty-2) = //greyf(side);
            *pixel(xp-1, lasty+1) = *pixel(xp-1, lasty-2)=*pixel(xp-2, lasty-1)=*pixel(xp-2, lasty) = greyf(CORNER);
            *pixel(xp, lasty-1) = *pixel(xp, lasty)= *pixel(xp-1, lasty-1)=*pixel(xp-1, lasty) = 0;
        } else {
            double intgr, fr = modf(yp, &intgr);
            *pixel(round(xp), intgr+1) =grey(fr*255);
            *pixel(round(xp), intgr) =grey(255);
            *pixel(round(xp), intgr-1) =grey((1-fr)*255);
        }
        lasty = yp;
    }
    /*{
        long long int x1;
        numToPicDI64(x, 0, &x1, &dummy);
        if(x1 < 0) {
            i64 i = 0;
            double lastxp, lastyp, yp;
            picToNumDD(i,0,&x,&dummy);
            numToPicDD(0, function(x), &dummy, &yp);
            lastyp = yp;
            while(lastyp - yp < 1+SMALLFLOAT) {
                lastyp = yp;
                lastxp = x;
                i++;
                picToNumDD(i,0,&x,&dummy);
                numToPicDD(0, function(x), &dummy, &yp);
            }
            x = lastxp; // go back one iteration to get after the jump
        }
    }*/
//    for(int n = 0; n < 10; n++) {
    x = 0;
    long long int y2, x1;
    int colors[6] = {0xffffff00, 0xff00ff00, 0xff00ffff,
                    0xff0000ff, 0xffff00ff, 0xffffffff};
    int maxExponent = 5; for(; maxExponent >= 0; maxExponent--) { if(iters[maxExponent] > 0) break; }
//    int maxExponentVisibleOnUnitSegment = MIN(maxExponent, itNum);
//    double lastRes = 0;
//    for(int exponent = 5; exponent >= 0; exponent--) {
//        for(int i = 1; i <= iters[exponent]; i++) {
    double endingArg;
    if(maxExponent > itNum) {
        picToNumDD(width, 0, &endingArg, &dummy);
    } else {
        endingArg = iters[0];
        for(int it = 1; it <= itNum; it++) {
            endingArg = (it>5?0:iters[it]) + functionI(endingArg);
        }
    }
///*    for(int w1 = 0; w1 < iters[1] + 1; w1++)
    {

        double ci(int depth, double ar) {

            double intgr, fr = modf(ar, &intgr);
            if(ar < 0) { fr = 1+fr; intgr--; }
            if(depth == 0) {
                return intgr;
            }
            return functionI(ci(depth-1, functionC(fr))) + intgr;
        }
        picToNumDD(0, 0, &x, &dummy);
//        i64 startingx;
        if(x < 0) x = 0;
//        drawSmallCircle(pixel, x1, y2, 0xff00ff00);


        while(/*x1 <= width && */x <= endingArg+SMALLFLOAT) {
//            fprintf(stderr, "w1 = %d w0 = %d\n", w1, w0);
//            fprintf(stderr, "x = %lf endingArg+SMALLFLOAT = %lf\n", x, endingArg+SMALLFLOAT);

            volatile double y = function(x);
            volatile long long int y1, x2;
            numToPicDI64(x, x, &x1, &y1);
            numToPicDI64(y, y, &x2, &y2);
            if(x2 < 0 || y2 > height) {
                goto nextIter;
            }
            if(x1 > width || y1 < 0) {
                break;
            }
            for(int i = y1; i > y2; i--) {
                *pixel(x1, i) = 0xffff0000;
            }
            if(y < endingArg) {
                for(int i = x1; i < x2; i++) {
                    *pixel(i, y2) = 0xff00ff00;
                }
            }
            nextIter:
            if(x1 == x2) {
                fprintf(stderr, "hi^))");
                picToNumDD(x1+1, 0, &x, &dummy);
//                break;
            } else {
                x = y;
            }
        }


//        {
//            i64 endingx;
//            numToPicDI64(x, 0, &endingx, &dummy);
//            fprintf(stderr, "startingx %d endingx %d", startingx, endingx);
//            if(endingx <= startingx + 1) {

//                break;
//            }
//        }
//        {
//            long long int xi;
//            numToPicDI64(x, 0, &xi, &dummy);
//            volatile double y = function(x);
//            volatile long long int lastyi;
//            numToPicDI64(0, y, &dummy, &lastyi);
//            xi++;
//            while(true) { /*break;*/// ***
//                picToNumDD(xi, 0, &x, &dummy);
//                y = function(x);
//                long long int yi;
//                numToPicDI64(0, y, &dummy, &yi);
//                if(yi - lastyi < -2) break;
//                lastyi = yi;
//                xi++;
//            }

//            picToNumDD(xi+1, 0, &x, &dummy);
//        }
    }


    numToPicDI64(endingArg, function(endingArg), &x1, &y2);
    drawSmallCircle(pixel, x1, y2, 0xffff0000);

    memcpy(dataWithSelection, data, WIDTH_MAX*height*4);
    int end = clock();
    snprintf(text,sizeof(text), "%lf ms", (double)(end - start) / CLOCKS_PER_SEC*1000);
//    guiRedraw();
}
char *appName = "ordinalIteration";
int main()
{//char appName2[] = "ordinalIteration2";
    mainGrid = allocateGrid(50, 50, 5);
    pushGrid(&mainGrid);
    guiStartDrawing(appName);
    guiSetSize(rootWindow, width, height+50);
    recalculatePicture();
    XImage *res = XCreateImage(xdisplay, DefaultVisual(xdisplay, DefaultScreen(xdisplay)), 24,
                 ZPixmap, 0, dataWithSelection, WIDTH_MAX, HEIGHT_MAX, 32,
                         WIDTH_MAX*4);
//    XdbeBackBuffer bb = XdbeAllocateBackBufferName(xdisplay, rootWindow, XdbeUndefined);
    Window bb = rootWindow;
    GC gc = XCreateGC(xdisplay, bb, 0, NULL);
    XPutImage(xdisplay, rootWindow, gc, res, 0, 0, 0,50, width, height);

    Painter pa = {bb, gc};
    getPos = gridGetPos;
    feedbackSize = gridFeedbackSize;
    mainGrid.gridStart.x = 5;
    mainGrid.gridStart.y = 5;
//    Pixmap pixm = XCreatePixmap(xdisplay, bb, width, height, xDepth);
//    GC pixgc = XCreateGC(xdisplay, pixm, 0, NULL);

    while(true) {
        guiNextEvent();
//        if(xEvent.type == ButtonPress ) {
            fprintf(stderr, "received event %d \n", xEvent.type);
//        }
        {
            int queued = XEventsQueued(xdisplay, QueuedAlready);
            if(queued > 1){
                fprintf(stderr, "%d queued\n", queued);
            }
        }
        volatile int t;
        bool consume = false;
        if(xEvent.type == Expose)
            XClearWindow(xdisplay, rootWindow);

        int bottom = getGridBottom(&mainGrid) + 5;
        if(xEvent.type == DestroyNotify) {
            goto exit;
        };
        if(xEvent.type == KeyPress) {
            XKeyEvent e = xEvent.xkey;
            KeySym sym = XLookupKeysym(&e, 0);
            if((sym == XK_M || sym == XK_m) && (e.state & ControlMask)) {
                mode = !mode;
                guiRedraw();
            };
        };
        loop(&pa, &consume);
        if(xEvent.type == ConfigureNotify) {
            if(xEvent.xconfigure.width != width ||
                    xEvent.xconfigure.height-bottom != height) {
                width = MIN(xEvent.xconfigure.width, WIDTH_MAX);
                height = MIN(xEvent.xconfigure.height - bottom, WIDTH_MAX);
                recalculatePicture();
                guiRedraw();
            }
        }
        if(!consume /*&& mode == selectMode*/) {
            static int prevx, prevy;
            static bool sel = false;
            if(xEvent.type == ButtonPress  && xEvent.xbutton.button == Button1) {
                prevx = startx = xEvent.xbutton.x;
                prevy = starty = xEvent.xbutton.y-bottom;
            }
            if(xEvent.type == ButtonPress && xEvent.xbutton.y > bottom) {
                fprintf(stderr, "\n button %d starty %d\n",xEvent.xbutton.button, starty);
                switch (xEvent.xbutton.button){
                    case Button1:
                        if(mode == selectMode) sel = true;
                        if(mode == pointMode) {
                            recalculatePicture();
                            drawSmallCircle(pixelWithSelection, startx, starty, 0xffff0000);
                        }
                        break;
                    case Button4:
                        zoomin();
                        break;
                    case Button5:
                        zoomout();
                        break;
                }
            } else if(xEvent.type == MotionNotify) {
                if(starty < 0) continue;
                XEvent newEvent;
                bool predicate(Display*d, XEvent* e, XPointer ar) {
                    return e->type == MotionNotify;
                }
                while(XCheckIfEvent(xdisplay, &newEvent,predicate, NULL)) {
                    xEvent = newEvent;
                }

                if(mode == selectMode) {
                    int endx = xEvent.xmotion.x;
                    int endy = xEvent.xmotion.y-bottom;
                    int c = MAX(abs(endx-startx), abs(endy-starty));
                    int xm = endx>startx?startx:startx-c;
                    int ym = endy>starty?starty:starty-c;
                    memcpy(dataWithSelection, data, WIDTH_MAX*height*4);
                    for(int i = 0; i < c; i++) {
                        *pixelWithSelection(xm+i, ym) =
                        *pixelWithSelection(xm+i, ym+c) =
                        *pixelWithSelection(xm, ym+i) =
                        *pixelWithSelection(xm+c, ym+i) = 0xffffffff;

                    }
                } else if(mode == moveMode) {
                    int endx = xEvent.xmotion.x;
                    int endy = xEvent.xmotion.y-bottom;
                    double sx, sy, ex,ey;

                    picToNumDD(prevx, prevy, &sx, &sy);
                    picToNumDD(endx, endy, &ex, &ey);
                    xm += (ex-sx);
                    ym += (ey-sy);
                    prevx  = endx;
                    prevy  = endy;
                    recalculatePicture();

                }                    //                recalculatePicture();
            } if(xEvent.type == ButtonRelease && sel && xEvent.xbutton.y > bottom) {
                double sx, sy;
                picToNumDD(startx, starty,
                         &sx, &sy);
                double ex, ey;
                picToNumDD(xEvent.xbutton.x, xEvent.xbutton.y-bottom,
                         &ex, &ey);
                c = MAX(fabs(ex-sx), fabs(ey-sy));
                if(c < DBL_MIN) {
                    c = DBL_MIN*100;// normalized
                }
                xm = -MIN(sx,ex);
                ym = -MIN(sy,ey);
                sel = false;
                recalculatePicture();
            }
        }
        XPutImage(xdisplay, rootWindow, gc, res, 0, 0, 0, bottom, width, height);

        {int w = getGridWidth(&mainGrid);
        if(w > width) {
            guiSetSize(rootWindow, w, height+bottom);
        }}
    }
exit:
    XDestroyWindow(xdisplay,
                   rootWindow);

    XCloseDisplay(xdisplay);
    return 0;
}
