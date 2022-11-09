#include <stdio.h>
#include <string.h>
#include <settings.h>
#include <float.h>
#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include <X11/extensions/Xdbe.h>
#include "loadImage.h"
//#include "backend.h"
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
#include <assert.h>

#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)<(y)?(x):(y))
#define ELEMS(a) (sizeof(a)/sizeof(*(a)))
#define DEBUG_PRINT(a, f) fprintf(stderr, #a " = " f "\n", (a));
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

extern XEvent event;
//XEvent event;
int width = 600;
int height = 600;
#define WIDTH_MAX 2000
//700
//2000
#define HEIGHT_MAX 700
//2000
XImage* rawImage;
unsigned char data[WIDTH_MAX*HEIGHT_MAX*4];
unsigned char dataWithSelection[WIDTH_MAX*HEIGHT_MAX*4];
int itNum = 2;
double c = 8;
double xm = 4;
double ym = 4;
void picToNumDD (double x, double y, double* restrict rx, double* restrict ry) {
    int scale = MIN(width, height);
    if(rx) *rx = x*c/scale - xm;
    if(ry) *ry = c - y*c/scale - ym;
}
void numToPicDD (double x, double y,double * restrict rx, double * restrict ry) {
    int scale = MIN(width, height);
    if(rx) *rx = (x+xm)/c*scale;
    if(ry) *ry = -(y +ym - c)/c*scale;
}
void numToPicDI64 (double x, double y,long long int* restrict rx, long long int* restrict ry) {
    int scale = MIN(width, height);
    if(rx) *rx = round((x+xm)/c*scale);
    if(ry) *ry = round(-(y +ym - c)/c*scale);
}
void recalculatePicture();
char text[30];
INTROSPECT_ENUM_FILENAMES( modeEnum, selectMode, select.png, moveMode, move.png, pointMode, point.png);
modeEnum mode;
Grid mainGrid;
void zoomin()
{
    double factor = 1.38949549437;
/* picToNumDD (0, height) = -xm, -ym
    midy = cp - h/2*cp/h - ymp = cp/f - h/2*cp/f/h-ymn
    cp - cp/2 - ymp = cp/f - cp/f/2 - ymn
    cp(1-1/2-1/f+1/(2f)) = ymp - ymn
    cp(1/2-1/(2f)) = ymp-ymn
    cp(f-1)/2f = ymp-ymn
    ymn = ymp - cp(f-1)/2f = cn(f-1)/2
*/
    c /= factor;
    xm = xm - c*(factor-1)/factor;
    ym = ym - c*(factor-1)/2;
    recalculatePicture();
}

void zoomout()
{
    double factor = 1.38949549437;
    xm = xm + c*(factor-1)/factor;
    ym = ym + c*(factor-1)/2;
    c *= factor;
    recalculatePicture();
}
int startx, starty;
bool drawJumps = false;

void loop(Painter* pa);
int grey(int v) {
    int vv = v | v << 8;
    return vv | vv<<8;
}
int greyf(double v) {
    return grey(v*255);
}
double functionC (double x) {
    return -1/(x-1)-1; // =x/(1-x)
}
double functionI (double x) {
    return 1-1/(x+1); // = x/(x+1)
}
double xplus1 (double x) {
    return x+1;
}
typedef double (*dtd)(double);
typedef long long int i64;
double nthfunction(int n, double x) {
    if(n == 0) {
        return x+1;
    }
    double intgr, fr = modf(x, &intgr);
    if(x < 0) { fr = 1+fr; intgr--; }
    return functionI(nthfunction(n-1, functionC(fr))) + intgr;
}
#define SMALLFLOAT 0.001
//    return functionC(x);
//    return nthfunction(itNum, functionC(x));
//    return functionI(nthfunction(itNum, functionC(x)));
//    if(1-SMALLFLOAT < x && 1+SMALLFLOAT>x) return 1.1;
//    else return 1/(2-x);
//    return nthfunction( MAX((int)floor(x), 0),/*itNum,*/x);
//}
double oneovertwominusx(double x) {
    return 1/(2-x);
}
double oneovertwominusxjump(double x) {
//    if(1-SMALLFLOAT < x && 1+SMALLFLOAT>x) return 1.1;
    /*else*/ return 1/(2-x);
}
double frac(double x) {
    double intgr, fr = modf(x, &intgr);
    if(x < 0) { fr = 1+fr; intgr--; }
    return fr;
}
double oneovertwominusfracx(double x) {
    return 1/(2-frac(x));
}
double functionOne(double x) {
    return nthfunction( 1, x);
}
double functionInProccess (double x) {
    return nthfunction( 1, functionC(x));
}
double functionInProccess2 (double x) {
    return functionI(nthfunction( 1, functionC(x)));
}
double themainfunction (double x) {
    return nthfunction( itNum, x);
}
double functionOmega (double x) {
    return nthfunction( MAX((int)floor(x), 0),x);
}
dtd functions[] = {xplus1, oneovertwominusx, oneovertwominusxjump, frac, oneovertwominusfracx, functionOne,
                   functionC, functionInProccess, functionInProccess2, themainfunction, functionOmega};
dtd function = xplus1; //(double x) {
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
enum {drawLines, drawText};
extern Display* xdisplay;
extern Window rootWindow;
extern int xDepth;
extern XFontSet xFontSet;
struct _XftFont;
extern struct _XftFont *xFont;

void drawCoordinateLines(int mode) {

    double startx, starty,endx,endy;
    picToNumDD(0,0,&startx,&starty);
    picToNumDD(width-1,height-1,&endx,&endy);
    double diff = -endy + starty;
    double order = round(log(diff)/log(10)+0.5)+1;
//    fprintf(stderr, "order is %lf\n", order);
    double nextOrder = exp((order-2)*log(10));
    double line = floor(startx/nextOrder)*nextOrder;

    static XftColor                xftcolor;
    static Pixmap pixm;
    static GC pixgc;
    static XftDraw *xftdraw;
    { static bool init = false;\
        if(!init) {init = true;
            XRenderColor            xrcolor;
            xrcolor.red = 0xff * 256;
            xrcolor.green = 0xff * 256;
            xrcolor.blue = 0x00 * 256;
            xrcolor.alpha = 0xffff;
            XftColorAllocValue( xdisplay, DefaultVisual(xdisplay,DefaultScreen(xdisplay)),
                        DefaultColormap( xdisplay, DefaultScreen(xdisplay) ), &xrcolor, &xftcolor );
            pixm = XCreatePixmap(xdisplay, rootWindow, 100,100,xDepth);
            xftdraw = XftDrawCreate( xdisplay, pixm, DefaultVisual(xdisplay,DefaultScreen(xdisplay)),
                                       DefaultColormap( xdisplay, DefaultScreen(xdisplay) ) );
            pixgc = XCreateGC(xdisplay, pixm,0,NULL);
            XSetForeground(xdisplay,pixgc,0xff1e1e1e);
        }
    }

    i64 zx, zy;
    numToPicDI64(0,0,&zx,&zy);
    for(int i = 0; i < height; i++) {
        *pixel(zx, i) = *pixel(zx+1, i) = 0xff505050;
    }
    for(int i = 0; i < width; i++) {
        *pixel(i,zy) = *pixel(i,zy+1) = 0xff505050;
    }

    while(line < endx) {
        i64 p;
        numToPicDI64(line, 0, &p, NULL);
        for(int i = 0; i < height; i++) {
            *pixel(p, i) = 0xff505050;
        }
        char text[30];
        int len;
        len = snprintf(text, ELEMS(text), "%.*lf", (int)MAX(0,
                                                      -order+2), line);
        XGlyphInfo extents;
        XftTextExtentsUtf8( xdisplay, xFont, (XftChar8 *)text, len, &extents );
        XFillRectangle(xdisplay,
                 pixm, pixgc, 0,0,extents.width, extents.height);
        XftDrawStringUtf8(xftdraw, &xftcolor, xFont,
                          0, extents.height, (XftChar8 *)text, len );
        int xdest = p-extents.width/2,  ydest=height - extents.height - 5;
        if(xdest>=0&&xdest<width&&ydest>=0&&ydest<height) {
            XGetSubImage(xdisplay, pixm, 0,0, extents.width, extents.height,
                     AllPlanes, ZPixmap, rawImage,
                     xdest, ydest);
        }
        line += nextOrder;
    }
    line = floor(endy/nextOrder)*nextOrder;
    while(line < starty) {
        i64 p;
        numToPicDI64(0,line, NULL, &p);
        for(int i = 0; i < width; i++) {
            *pixel(i,p) = 0xff505050;
        }
        char text[30];
        int len;
        len = snprintf(text, ELEMS(text), "%.*lf", (int)MAX(0,
                                                      -order+2), line);
        XGlyphInfo extents;
        XftTextExtentsUtf8( xdisplay, xFont, (XftChar8 *)text, len, &extents );
        XFillRectangle(xdisplay,
                 pixm, pixgc, 0,0,extents.width, extents.height);
        XftDrawStringUtf8(xftdraw, &xftcolor, xFont,
                          0, extents.height, (XftChar8 *)text, len );
        int ydest = p- (extents.height)/2;
        if(ydest>=0&&ydest<height) {
            XGetSubImage(xdisplay, pixm, 0,0, extents.width, extents.height,
                         AllPlanes, ZPixmap, rawImage,
                         5, ydest);
        }
        line += nextOrder;
    }

}
void drawIterations();
void drawIterationsXplus1();
void drawIterationsOneovertwominusx();
void drawIterationsOneovertwominusxJump();
void recalculatePicture() {
    int start = clock();

    memset(data, 30, WIDTH_MAX*height*4);
    drawCoordinateLines(drawLines);
//    return;
    double x;
    {
        int i = 0;
        double xp, yp;
        picToNumDD(i,0,&x,NULL);
        numToPicDD(x, x, &xp, &yp);
        int ys = round(yp);
        for(; i < width; i++) {
            *pixel(i, ys-i) = 0xffff0000;
        }
    }
    picToNumDD(0,0,&x,NULL);
    double lasty = NAN;
    double lastx = NAN;
    for(int i = 0; i < width - 1; i++) {
        double xp, yp;
        picToNumDD(i,0,&x,NULL);
        numToPicDD(x, function(x), &xp, &yp);
        bool jump = fabs(yp-lasty) > 1+SMALLFLOAT;
        if(jump) {
/*  f(1-1/n) = t
 *  f(n) = -1/(t-1)
 * */       if(function == functionInProccess) {
                double lastJumpNumber = floor(-1/(x-1));
                double lastJump = 1-1/lastJumpNumber;
                if(lastJump > lastx && lastJump <= x) {
                    drawJumps =true;
                }
                else drawJumps = false;
            }


            if(drawJumps) {
                drawSmallCircle(pixel, xp, yp, 0xffffffff);

                *pixel(xp+1, lasty-1) = *pixel(xp+1, lasty)= *pixel(xp, lasty+1)=*pixel(xp, lasty-2) = //greyf(side);
                *pixel(xp-1, lasty+1) = *pixel(xp-1, lasty-2)=*pixel(xp-2, lasty-1)=*pixel(xp-2, lasty) = greyf(CORNER);
                *pixel(xp, lasty-1) = *pixel(xp, lasty)= *pixel(xp-1, lasty-1)=*pixel(xp-1, lasty) = 0;
            } else if(yp >= 0 && yp <= height
                      ||
                      lasty >= 0 && lasty <= height) {
                    int sign = yp > lasty? 1 : -1;
                    for(double yind = lasty/*+sign*/; abs(yind-lasty) <= abs(yp-lasty); yind+=sign) {
                        double greyRight = fabs(yind*1.0-yp)/fabs(lasty-yp);
                        *pixel(i-1,yind) = grey((greyRight)*255);
                        *pixel(i,yind) = grey(255);
                        *pixel(i+1, yind) = grey((1-greyRight)*255);
                    }
            }
        } else {
            double intgr, fr = modf(yp, &intgr);
            *pixel(round(xp), intgr+1) =grey(fr*255);
            *pixel(round(xp), intgr) =grey(255);
            *pixel(round(xp), intgr-1) =grey((1-fr)*255);
        }
        lasty = yp;
        lastx = x;
    }

    if(function == oneovertwominusxjump) {
        double y;
        numToPicDD(1, 1, &x, &y);
        x+=1;
        *pixel(x+1, y-1) = *pixel(x+1, y)= *pixel(x, y+1)=*pixel(x, y-2) = //greyf(side);
        *pixel(x-1, y+1) = *pixel(x-1, y-2)=*pixel(x-2, y-1)=*pixel(x-2, y) = greyf(CORNER);
        *pixel(x, y-1) = *pixel(x, y)= *pixel(x-1, y-1)=*pixel(x-1, y) = 0;

        numToPicDD(1, 1.1, &x, &y);
        drawSmallCircle(pixel, x+1, y, 0xffffffff);
    }

    if(function == xplus1) drawIterationsXplus1();
    else if(function == oneovertwominusx) drawIterationsOneovertwominusx();
    else if(function == oneovertwominusxjump) drawIterationsOneovertwominusxJump();
    else drawIterations();
    memcpy(dataWithSelection, data, WIDTH_MAX*height*4);
    int end = clock();
    snprintf(text,sizeof(text), "%lf ms", (double)(end - start) / CLOCKS_PER_SEC*1000);
}
int maxExponent = 5;
void getLastArg(double* lastArg, double* endingVal) {
//    {xplus1, oneovertwominusx, oneovertwominusxjump, frac, oneovertwominusfracx, themainfunction, functionOmega};
    maxExponent = 5; for(; maxExponent >= 0; maxExponent--) { if(iters[maxExponent] > 0) break; }
//    if(function == xplus1) {
//        if(maxExponent >= 1) {
//            *lastArg = INFINITY;
//        } else {
//            *lastArg = MAX(iters[0]-1,0);
//            *endingVal = MAX(iters[0]-1,0);
//        }
//        return;
//    }
//    int localItNum
    if(function == themainfunction) {
        if(maxExponent > itNum) {
            picToNumDD(width, 0, lastArg, NULL);
        } else {
            *lastArg = MAX(iters[0]-1, 0);
            *endingVal = 0;
            for(int it = 1; it <= itNum; it++) {
                *lastArg = (it>5?0:iters[it]) + functionI(*lastArg);
            }
        }
    }

    if(function == functionOne) {
        if(maxExponent > 1) {
            picToNumDD(width, 0, lastArg, NULL);
        } else {
            *lastArg = MAX(iters[0]-1, 0);
            *endingVal = 0;
            for(int it = 1; it <= 1; it++) {
                *lastArg = (it>5?0:iters[it]) + functionI(*lastArg);
            }
        }
    }
//    else {
//        *lastArg = 0;
//        *endingVal = 4;
//    }
}

void drawIterations() {
    if(function == oneovertwominusfracx) return;
    double x = 0;
    long long int y2, x1;
    double lastArg;
    double endingVal;
    int iterationsDoneOmegaZero = 0;
//    int iterationsDoneOmegaOne = 0;
    getLastArg(&lastArg, &endingVal);
    {
        if(function == themainfunction||function==functionOne) {
            double ci(int depth, double ar) {

                double intgr, fr = modf(ar, &intgr);
                if(ar < 0) { fr = 1+fr; intgr--; }
                if(depth == 0) {
                    return intgr;
                }
                return functionI(ci(depth-1, functionC(fr))) + intgr;
            }
            picToNumDD(0, 0, &x, NULL);
            if(x < 0) x = 0;
            else x = ci(itNum, x);
        }
        bool continuecondition = maxExponent >= 0;
        while(continuecondition) {
            double y = function(x);
            long long int y1, x2;
            numToPicDI64(x, x, &x1, &y1);
            numToPicDI64(y, y, &x2, &y2);
    //            if(x2 < 0 || y2 > height) {
    //                goto nextIter;
    //            }
            if(x1 > width || y1 < 0) {
                break;
            }
            for(int i = MIN(y1, height); i > y2 && i > 0; i--) {
                *pixel(x1, i) = 0xffff0000;
            }
            if(((function == themainfunction||function==functionOne) && x < lastArg) ||
                    (function != themainfunction&&function!=functionOne)) {
                for(int i = MAX(x1, 0); i < x2 && i < width; i++) {
                    *pixel(i, y2) = 0xff00ff00;
                }
            }
//            nextIter:
            if(x1 == x2 || y <= x) {
    //                fprintf(stderr, "hi^))");
                double lastx=x;
                picToNumDD(x1+1, 0, &x, NULL);
                if(lastx <= x) {
                    (*((i64*)(&x)))++;
                }
            } else {
    //                prevX = x;
                x = y;
            }

            if(function == themainfunction || function == functionOne) {
                continuecondition = x < lastArg;
//                if(iters[0] > 0) {
//                    x = lastArg;
//                    double y = function(x);
//                    long long int y1, x2;
//                    numToPicDI64(x, x, &x1, &y1);
//                    numToPicDI64(y, y, &x2, &y2);

//                    for(int i = y1; i > y2 && i > 0; i--) {
//                        *pixel(x1, i) = 0xffff0000;
//                    }
//                }
            }
            if(function == oneovertwominusxjump) continuecondition = x < lastArg;
            if(function == xplus1) continuecondition = ((maxExponent == 0 &&
                                                iters[0]-2 > iterationsDoneOmegaZero)
                    || maxExponent > 0);
            iterationsDoneOmegaZero++;
        }
    }
//    DEBUG_PRINT(maxExponent, "%d");
    if(function == xplus1) {
        if(maxExponent <= 0) {
            i64 y1;
            numToPicDI64(MAX(iters[0]-1,0), MAX(iters[0]-1,0), &x1, &y1);
            numToPicDI64(0, iters[0], NULL, &y2);
            drawSmallCircle(pixel, x1, y2, 0xffff0000);
            for(int i = y1; i > y2 && i > 0; i--) {
                *pixel(x1, i) = 0xffff0000;
            }
        }

    } else if(function != frac) {
        numToPicDI64(lastArg, iters[0]==0?lastArg:function(lastArg), &x1, &y2);
        drawSmallCircle(pixel, x1, y2, 0xffff0000);
    }
    if(function == themainfunction || function == functionOne) {
//        continuecondition = x < lastArg;
        if(iters[0] > 0) {
            x = lastArg;
            double y = function(x);
            long long int y1, x2;
            numToPicDI64(x, x, &x1, &y1);
            numToPicDI64(y, y, &x2, &y2);

            for(int i = y1; i > y2 && i > 0; i--) {
                *pixel(x1, i) = 0xffff0000;
            }
        }
    }

}
void drawIterationsXplus1() {
    double x = 0;
    long long int y2, x1;
    maxExponent = 5; for(; maxExponent >= 0; maxExponent--) { if(iters[maxExponent] > 0) break; }
    for(int i=1; (maxExponent <= 0?i < iters[0]:true); i++) {
        double y = function(x);
        long long int y1, x2;
        numToPicDI64(x, x, &x1, &y1);
        numToPicDI64(y, y, &x2, &y2);
//            if(x2 < 0 || y2 > height) {
//                goto nextIter;
//            }
        if(x1 > width || y1 < 0) {
            break;
        }
        for(int i = MIN(y1, height); i > y2 && i > 0; i--) {
            *pixel(x1, i) = 0xffff0000;
        }
//            if(maxEponent <= 0?i+1 < iters[0]:true) {
            for(int i = MAX(x1, 0); i < x2 && i < width; i++) {
                *pixel(i, y2) = 0xff00ff00;
            }
//            }
//        nextIter:
        if(x1 == x2 || y <= x) {
//                fprintf(stderr, "hi^))");
            double lastx=x;
            picToNumDD(x1+1, 0, &x, NULL);
            if(lastx <= x) {
                (*((i64*)(&x)))++;
            }
        } else {
//                prevX = x;
            x = y;
        }
    }

    if(maxExponent <= 0) {
        i64 y1;
        numToPicDI64(MAX(iters[0]-1,0), MAX(iters[0]-1,0), &x1, &y1);
        numToPicDI64(0, iters[0], NULL, &y2);
        drawSmallCircle(pixel, x1, y2, 0xffff0000);
        for(int i = y1; i > y2 && i > 0; i--) {
            *pixel(x1, i) = 0xffff0000;
        }
    }

}
void drawIterationsOneovertwominusx() {
    double x = 0;
    long long int y2, x1;
    maxExponent = 5; for(; maxExponent >= 0; maxExponent--) { if(iters[maxExponent] > 0) break; }
    for(int i=1; (maxExponent <= 0?i < iters[0]&&x<1:x<1); i++) {
        double y = function(x);
        long long int y1, x2;
        numToPicDI64(x, x, &x1, &y1);
        numToPicDI64(y, y, &x2, &y2);
//            if(x2 < 0 || y2 > height) {
//                goto nextIter;
//            }
        if(x1 > width || y1 < 0) {
            break;
        }
        for(int i = MIN(y1, height); i > y2 && i > 0; i--) {
            *pixel(x1, i) = 0xffff0000;
        }
//            if(maxEponent <= 0?i+1 < iters[0]:true) {
            for(int i = MAX(x1, 0); i < x2 && i < width; i++) {
                *pixel(i, y2) = 0xff00ff00;
            }
//            }
//        nextIter:
        if(x1 == x2 || y <= x) {
//                fprintf(stderr, "hi^))");
            double lastx=x;
            picToNumDD(x1+1, 0, &x, NULL);
            if(lastx <= x) {
                (*((i64*)(&x)))++;
            }
        } else {
//                prevX = x;
            x = y;
        }
    }

    if(maxExponent == 0) {
        i64 y1;
        numToPicDI64(x, x, &x1, &y1);
        numToPicDI64(0, function(x), NULL, &y2);
        drawSmallCircle(pixel, x1, y2, 0xffff0000);
        for(int i = y1; i > y2 && i > 0; i--) {
            *pixel(x1, i) = 0xffff0000;
        }
    } else if(maxExponent == -1) {
        numToPicDI64(0,0, &x1, &y2);
        drawSmallCircle(pixel, x1, y2, 0xffff0000);

    } else {
        numToPicDI64(1,1, &x1, &y2);
        drawSmallCircle(pixel, x1, y2, 0xffff0000);

    }

}
void drawIterationsOneovertwominusxJump() {
    double x = 0;
    long long int y2, x1;
    maxExponent = 5; for(; maxExponent >= 0; maxExponent--) { if(iters[maxExponent] > 0) break; }
    for(int i=1; (i < iters[0]||maxExponent>0)&&x < 1; i++) {
        double y = function(x);
        long long int y1, x2;
        numToPicDI64(x, x, &x1, &y1);
        numToPicDI64(y, y, &x2, &y2);
//            if(x2 < 0 || y2 > height) {
//                goto nextIter;
//            }
        if(x1 > width || y1 < 0) {
            break;
        }
        for(int i = MIN(y1, height); i > y2 && i > 0; i--) {
            *pixel(x1, i) = 0xffff0000;
        }
//            if(maxEponent <= 0?i+1 < iters[0]:true) {
            for(int i = MAX(x1, 0); i < x2 && i < width; i++) {
                *pixel(i, y2) = 0xff00ff00;
            }
//            }
//        nextIter:
        if(x1 == x2 || y <= x) {
//                fprintf(stderr, "hi^))");
            double lastx=x;
            picToNumDD(x1+1, 0, &x, NULL);
            if(lastx <= x) {
                (*((i64*)(&x)))++;
            }
        } else {
//                prevX = x;
            x = y;
        }
    }
    if(maxExponent > 0) {
        x=1;
        bool condition(int i) {
            if(maxExponent > 1 || iters[1] > 1) return true;
            return i < iters[0];
        }
        for(int i=1; condition(i); i++) {
            double y;
            if(i == 1) y=1.1;else y = function(x);
            long long int y1, x2;
            numToPicDI64(x, x, &x1, &y1);
            numToPicDI64(y, y, &x2, &y2);
            if(x1 > width || y1 < 0) {
                break;
            }
            for(int i = MIN(y1, height); i > y2 && i > 0; i--) {
                *pixel(x1, i) = 0xffff0000;
            }
    //            if(maxEponent <= 0?i+1 < iters[0]:true) {
                for(int i = MAX(x1, 0); i < x2 && i < width; i++) {
                    *pixel(i, y2) = 0xff00ff00;
                }
    //            }
            if(x1 == x2 || y <= x) {
    //                fprintf(stderr, "hi^))");
                double lastx=x;
                picToNumDD(x1+1, 0, &x, NULL);
                if(lastx <= x) {
                    (*((i64*)(&x)))++;
                }
            } else {
    //                prevX = x;
                x = y;
            }
        }
    }
    if(maxExponent == -1) {
        i64 y1;
        numToPicDI64(0, 0, &x1, &y1);
    } else if( maxExponent == 0) {
        i64 y1;
        numToPicDI64(x, x, &x1, &y1);
        numToPicDI64(0, function(x), NULL, &y2);
        drawSmallCircle(pixel, x1, y2, 0xffff0000);
        for(int i = MIN(y1, height); i > y2 && i > 0; i--) {
            *pixel(x1, i) = 0xffff0000;
        }
    } else if(maxExponent == 1 && iters[1] == 1 && iters[0] == 0) {
        i64 y1;
        numToPicDI64(x, x, &x1, &y1);
        numToPicDI64(0, 1, NULL, &y2);
        drawSmallCircle(pixel, x1, y2, 0xffff0000);
        for(int i = MIN(y1, height); i > y2 && i > 0; i--) {
            *pixel(x1, i) = 0xffff0000;
        }
    } else if(maxExponent == 1 && iters[1] == 1 && iters[0] == 1) {
        i64 y1;
        numToPicDI64(x, x, &x1, &y1);
        numToPicDI64(0, 1.1, NULL, &y2);
        drawSmallCircle(pixel, x1, y2, 0xffff0000);
        for(int i = MIN(y1, height); i > y2 && i > 0; i--) {
            *pixel(x1, i) = 0xffff0000;
        }
    } else if(maxExponent == 1) {
        i64 y1;
        numToPicDI64(x, x, &x1, &y1);
        numToPicDI64(0, function(x), NULL, &y2);
        drawSmallCircle(pixel, x1, y2, 0xffff0000);
        for(int i = MIN(y1, height); i > y2 && i > 0; i--) {
            *pixel(x1, i) = 0xffff0000;
        }
    }
//    if(maxExponent <= 0 || (maxExponent== 1 && iters[1] == 1)) {

//        if(iters[0] == 1) {
//        }
//        drawSmallCircle(pixel, x1, y2, 0xffff0000);
//        for(int i = MIN(y1, height); i > y2 && i > 0; i--) {
//            *pixel(x1, i) = 0xffff0000;
//        }
//    } else {
//        numToPicDI64(1,1, &x1, &y2);
//        drawSmallCircle(pixel, x1, y2, 0xffff0000);

//    }

}
char *appName = "ordinalIteration";

int mainApp()
{//char appName2[] = "ordinalIteration2";
    mainGrid = allocateGrid(50, 50, 5);
    pushGrid(&mainGrid);
    guiStartDrawing();
    guiSetSize(/*rootWindow, */width, height+50);
//    recalculatePicture();
    rawImage = XCreateImage(xdisplay, DefaultVisual(xdisplay, DefaultScreen(xdisplay)), 24,
                            ZPixmap, 0, data, WIDTH_MAX, HEIGHT_MAX, 32,
                                    WIDTH_MAX*4);
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
    Pixmap pixm = XCreatePixmap(xdisplay, rootWindow, 100, 100, xDepth);
    GC pixgc = XCreateGC(xdisplay, pixm, 0, NULL);
    Painter pixpa = {pixm, pixgc};
    guiSetForeground(&pixpa, 0xff1e1e1e);
    guiFillRectangle(&pixpa, 0,0,100,100);
//    XImage *piximage = XGetImage(xdisplay, pixm, 0,0,100,100, AllPlanes, ZPixmap);

//    char* strs[] = {"M", "w", "l", "g", "j", "i"};
//    for(int i = 0; i < ELEMS(strs); i++) {
//        XGlyphInfo extents;
//        XftTextExtentsUtf8( xdisplay, xFont, (XftChar8 *)strs[i], strlen(strs[i]), &extents );
//        fprintf(stderr, "%s: width %d height %d x %d y %d xoff %d yodd %d\n",
//                strs[i], extents.width, extents.height,
//                extents.x, extents.y,
//                extents.xOff, extents.yOff);
//    }
//    return 0;
    while(true) {
        guiNextEvent();
//        if(xEvent.type == ButtonPress ) {
            fprintf(stderr, "received event %d \n", event.type);
//        }
//        {
//            int queued = XEventsQueued(xdisplay, QueuedAlready);
//            if(queued > 1){
//                fprintf(stderr, "%d queued\n", queued);
//            }
//        }
//        int t;
//        if(xEvent.type == Expose)
//            XClearWindow(xdisplay, rootWindow);

        int bottom = getGridBottom(&mainGrid) + 5;
        if(event.type == DestroyNotify) {
            goto exit;
        };
        if(event.type == KeyPress) {
            XKeyEvent e = event.xkey;
            KeySym sym = XLookupKeysym(&e, 0);
            if((sym == XK_M || sym == XK_m) && (e.state & ControlMask)) {
                mode = !mode;
                guiRedraw();
            };
        };
        if(event.type != MotionNotify) loop(&pa);
        if(event.type == ConfigureNotify) {
            if(event.xconfigure.width != width ||
                    event.xconfigure.height-bottom != height) {
                width = MIN(event.xconfigure.width, WIDTH_MAX);
                height = MIN(event.xconfigure.height - bottom, WIDTH_MAX);
                recalculatePicture();
                guiRedraw();
            }
        }
        if(true /*&& mode == selectMode*/) {
            static int prevx, prevy;
            static bool sel = false;
            if(event.type == ButtonPress  && event.xbutton.button == Button1) {
                prevx = startx = event.xbutton.x;
                prevy = starty = event.xbutton.y-bottom;
            }
            if(event.type == ButtonPress && event.xbutton.y > bottom) {
                fprintf(stderr, "\n button %d starty %d\n",event.xbutton.button, starty);
                switch (event.xbutton.button){
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
            } else if(event.type == MotionNotify) {
                DEBUG_PRINT(starty, "%d");
                if(starty < 0) continue;
                XEvent newEvent;
                Bool predicate(Display*d, XEvent* e, XPointer ar) {
                    return e->type == MotionNotify;
                }
                while(XCheckIfEvent(xdisplay, &newEvent,predicate, NULL)) {
                    event = newEvent;
                }

                if(mode == selectMode) {
                    int endx = event.xmotion.x;
                    int endy = event.xmotion.y-bottom;
                    double c = MAX(abs(endx-startx), abs(endy-starty));
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
                    int endx = event.xmotion.x;
                    int endy = event.xmotion.y-bottom;
                    double sx, sy, ex,ey;

                    picToNumDD(prevx, prevy, &sx, &sy);
                    picToNumDD(endx, endy, &ex, &ey);
                    xm += (ex-sx);
                    ym += (ey-sy);
                    prevx  = endx;
                    prevy  = endy;
                    recalculatePicture();

                }                    //                recalculatePicture();
            } if(event.type == ButtonRelease && sel && event.xbutton.y > bottom) {
                double sx, sy;
                picToNumDD(startx, starty,
                         &sx, &sy);
                double ex, ey;
                picToNumDD(event.xbutton.x, event.xbutton.y-bottom,
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
//        Point poppop = {30, 30};

//        XFillRectangle(xdisplay, pixm, pixgc, 0,0,100,100);
//        guiDrawTextZT(&pixpa, "Hi^)", poppop,  0xffffff00);
//        XGetSubImage(xdisplay, pixm, 0,0, 100, 100,
//                     AllPlanes, ZPixmap, res, 100, 0);
        XPutImage(xdisplay, rootWindow, gc, res, 0, 0, 0, bottom, width, height);

        {int w = getGridWidth(&mainGrid);
        if(w > width) {
            fprintf(stderr, "setting size %d %d\n", w, height+bottom);
            guiSetSize(/*rootWindow, */w, height+bottom);
        }}
    }
exit:
    XDestroyWindow(xdisplay,
                   rootWindow);

    XCloseDisplay(xdisplay);
    return 0;
}
double parameterT = 0;
//double (*squishedFunction) (double x) = xplus1;
double squishedFunction(double x) {
    return 1/x;
}
double functionSquish(double x) {
    double functionCI (double x) {
        if(parameterT <= FLT_MIN) {
            return x;
        }
        return (-1/(x*parameterT-1)-1)/parameterT; // =x/(1-x)
    }
    double functionII (double x) {
        if(parameterT <= FLT_MIN) {
            return x;
        }
        return (1-1/(x*parameterT+1))/parameterT; // = x/(x+1)
    }
    return functionII(squishedFunction(functionCI(x)));
}
int mainBatch() {
    guiStartDrawing();
    guiSetSize(700,700);
    width = 700; height = 700;
    rawImage = XCreateImage(xdisplay, DefaultVisual(xdisplay, DefaultScreen(xdisplay)), 24,
                            ZPixmap, 0, data, 700, 700, 32,
                                    700*4);
    function = functionSquish;
    int frames = 10;
    for(int i = 0; i <= frames; i++) {
        parameterT = 1.0*i/frames;
        recalculatePicture();
        saveImageSomewhereNewWrongChannelsZT(rawImage, "frame");
    }
    return 0;
}
int main(int argc) {
    if(argc > 1) return mainBatch();
    else return mainApp();
}
void loop(Painter* pa) {
//    popGrid();
    setCurrentGridPos(0,0);
//    bool res = false;
    bool recalc = persistentNumberEdit(pa, 5, itNum);
    gridNextRow();
    recalc |= guiCheckBox(pa, &drawJumps);


    gridNextColumn();
    if(resourseToolButton(pa, "minus.png")) {
        itNum--;
        SAVE_INT(itNum);
        recalc = /*res = */true;
        guiRedraw();
    }
    gridNextColumn();
    if(resourseToolButton(pa, "plus.png")) {
        itNum++;
        SAVE_INT(itNum);
        recalc = /*res = */true;
        guiRedraw();
    }
//    gridNextColumn();
//    resourseModeToolButton(pa, "select.png");
//        mode = selectMode;
//        res = true;
//    }
    gridNextColumn();
//    if(resourseToolButton(pa, "iters.png")) {
//        recalc = true;
//        mode = itersMode;
////        res = true;
//    }
    gridNextColumn();
    if(resourseToolButton(pa, "100percent.png")) {
        c = 1;
        xm = 0;
        ym = 0;
        recalc/* = res */= true;
    }
    gridNextColumn();
    if(resourseToolButton(pa, "zoomin.png")) {
        zoomin();
    }
    gridNextColumn();
    if(resourseToolButton(pa, "zoomout.png")) {
        zoomout();
    }

    gridNextColumn();
    if(persistentToolButtonGroup(pa, modeEnum, mode)) {
        startx = starty = -1;
    }

    gridNextColumn();
    guiLabelZTWithBackground(pa, text, true);
    gridNextRow();
    STATIC(Grid, nextprevgrid, allocateGrid(2,1,3));
    nextprevgrid.gridStart = getPos();
    pushGrid(&nextprevgrid);
    setCurrentGridPos(0,0);
    static int functionNumber = 0;
    if(guiButtonZT(pa, "Next")) {
        functionNumber = (functionNumber+1)%ELEMS(functions);
        function = functions[functionNumber];
        memset(iters, 0, sizeof(iters));
        recalc = true;
        if(function == frac) drawJumps=true;
        if(function == oneovertwominusx) drawJumps=false;
        if(function == functionC) drawJumps=false;
        if(function == functionInProccess2) drawJumps=true;
        if(function == themainfunction) itNum = 2;
    }
    gridNextColumn();
    if(guiButtonZT(pa, "Prev")) {
        functionNumber = (functionNumber+ELEMS(functions)-1)%ELEMS(functions);
        function = functions[functionNumber];
        recalc = true;
    }
    Size size; size.width = getGridWidth(&nextprevgrid);
               size.height = getGridHeight(&nextprevgrid);

    popGrid();
    feedbackSize(size);

    gridNextColumn();
//    STATIC(Grid, exprGrid, allocateGrid(12, 1, 1));

//    exprGrid.gridStart = getPos();
//    pushGrid(&exprGrid);
//    setCurrentGridPos(0,0);

    char* sups[] = {"​", "", "²","³",  "⁴", "⁵"};
    for(int exponent = 5; exponent > 0; exponent--) {
        char label[30];
        int len = snprintf(label, 30, "ω%s+", sups[exponent]);
        recalc |= persistentNumberEdit_(pa, 2, iters+exponent, label);
        gridNextRow();
        if(resourseToolButton(pa, "plus.png")) {
            iters[exponent]++; recalc = true; guiRedraw();
            for(int i = exponent-1; i >= 0; i--) {
                iters[i] = 0;
            }
        }
        gridNextColumn();
        guiLabelWithBackground(pa, label, len, true);
        gridNextRow();
        if(resourseToolButton(pa, "minus.png")) {
            iters[exponent]--; recalc = true; guiRedraw();
        }
        gridNextColumn();
    }
    recalc |= persistentNumberEdit(pa, 2, iters);
    gridNextRow();
    if(resourseToolButton(pa, "plus.png")) {
        iters[0]++; recalc = true; guiRedraw();
    }
    gridNextColumn();
    gridNextRow();
    if(resourseToolButton(pa, "minus.png")) {
        iters[0]--; recalc = true; guiRedraw();
    }

//    Size size; size.width = getGridWidth(&exprGrid);
//               size.height = getGridHeight(&exprGrid);

//    popGrid();
//    feedbackSize(size);
//    guiLabel(pa, text, true);


    if(recalc) {
        recalculatePicture();
//        guiRedraw();
    }

//    return res;
}
