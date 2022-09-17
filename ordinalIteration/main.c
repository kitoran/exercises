#include <stdio.h>
#include <string.h>
#include <float.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xdbe.h>
extern char appName[] = "ordinalIteration";
#include "gui.h"
#include <time.h>
#include "gridlayout.h"
//#include "timeAFunction.h"
#include "math.h"
#include "persistent.h"
#include "color.h"
#include "complex.h"
#define tau 6.283185307179586
struct Ordinal {
    int val[5];
} iters;

void ordinalEdit(Painter*p/*, Ordinal max*/) {
    Point pos = getPos();
    Size size = {500,
                30};


}

XEvent xEvent;
int size = 600;
unsigned char data[600*600*4];
unsigned char dataWithSelection[600*600*4];
int itNum = 0;
double c = 8;
double xm = 4;
double ym = 4;
void picToNumDD (double x, double y, double* restrict rx, double* restrict ry) {
    *rx = x*c/size - xm;
    *ry = c - y*c/size - ym;
}
void numToPicDD (double x, double y,double * restrict rx, double * restrict ry) {
    *rx = (x+xm)/c*size;
    *ry = -(y +ym - c)/c*size;
}
void numToPicDI64 (double x, double y,long long int* restrict rx, long long int* restrict ry) {
    *rx = round((x+xm)/c*size);
    *ry = round(-(y +ym - c)/c*size);
}

char text[30];
enum { itersMode, selectMode} mode;
void loop(Painter* pa, bool* consume) {
    setCurrentGridPos(0,0);
//    bool res = false;
    bool recalc = guiNumberEdit(pa, 5, &itNum, consume);

    gridNextColumn();
    if(resourseToolButton(pa, "minus.png", consume)) {
        itNum--;
        recalc = /*res = */true;
    }
    gridNextColumn();
    if(resourseToolButton(pa, "plus.png", consume)) {
        itNum++;
        recalc = /*res = */true;
    }
//    gridNextColumn();
//    resourseModeToolButton(pa, "select.png", consume);
//        mode = selectMode;
//        res = true;
//    }
    gridNextColumn();
    if(resourseToolButton(pa, "iters.png", consume)) {
        recalc = true;
        mode = itersMode;
//        res = true;
    }
    gridNextColumn();
    if(resourseToolButton(pa, "100percent.png", consume)) {
        c = 1;
        xm = 0;
        ym = 0;
        recalc/* = res */= true;
    }
    gridNextColumn();
    if(resourseToolButton(pa, "zoomin.png", consume)) {
        c /= 2;
        xm = xm - c/2;
        ym = ym - c/2;
        recalc/* = res */= true;
    }
    gridNextColumn();
    if(resourseToolButton(pa, "zoomout.png", consume)) {
        xm = xm + c/2;
        ym = ym + c/2;
        c *= 2;
        //cent = (-xm + -xm+c)/2 = -xm + c/2
        // -xm + c/2 = -xnm + c
        // xnm = c - c/2 + xm = xm + c/2
        recalc/* = res */= true;
    }
    gridNextColumn();
    guiLabelZT(pa, text);

    if(recalc) {
        recalculatePicture();
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
    return -1/(x-1);
}
double functionI (double x) {
    return 1-1/x;
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
    return nthfunction(3,x);
}
const double asin13 = 0.3398369094;
const double sqrt2 = 1.4142135623730951;
int* pixel(int x, int y) {
    static int oob;
    if(x >= 0 && x < 600&& y >= 0 && y < 600) {
        return (int*)(data+ (y*600+x)*4);
    } else {
        return &oob;
    }
}
int* pixelWithSelection(int x, int y) {
    static int oob;
    if(x >= 0 && x < 600 && y >= 0 && y < 600) {
        return (int*)(dataWithSelection+ (y*600+x)*4);
    } else {
        return &oob;
    }
}
#define SMALLFLOAT 0.001
void recalculatePicture() {
    int start = clock();

    memset(data, 30, 600*600*4);
    const double side = asin13*2*1.5/sqrt2;
    const double corner = (tau/4-asin13*2)*1.5/sqrt2;
    double dummy;
    double x;
    for(int i = 0; i < 600; i++) {
        double xp, yp;
        picToNumDD(i,0,&x,&dummy);
        numToPicDD(x, x, &xp, &yp);
//        if(!(xp+10 < size && xp - 10 >= 0 &&
//                                        yp+10 < size && yp - 10 >= 0)) {
//            continue;
////            goto end;
//        }
        *pixel(round(xp),round(yp)) = 0xffff0000;
    }

    picToNumDD(0,0,&x,&dummy);
    double lasty = NAN;
//    numToPic(x, x*x, &dummy, &lasty);
    for(int i = 0; i < 600 - 1; i++) {
        double xp, yp;
        picToNumDD(i,0,&x,&dummy);
        numToPicDD(x, function(x), &xp, &yp);
//        bool insideScope = (xp+10 < size && xp - 10 >= 0 &&
//                            yp+10 < size && yp - 10 >= 0);
        bool jump = fabs(yp-lasty) > 1+SMALLFLOAT;
        if(jump) {
//            if(insideScope) {
                *pixel(xp+1, yp-1) = *pixel(xp+1, yp)= *pixel(xp, yp+1)=*pixel(xp, yp-2) = //greyf(side);
                *pixel(xp-1, yp+1) = *pixel(xp-1, yp-2)=*pixel(xp-2, yp-1)=*pixel(xp-2, yp) = greyf(corner);
                *pixel(xp, yp-1) = *pixel(xp, yp)= *pixel(xp-1, yp-1)=*pixel(xp-1, yp) = 0xffffffff;
//            }
//            bool lastInsideScope = (xp+10 < size && xp - 10 >= 0 &&
//                                lasty+10 < size && lasty - 10 >= 0);
//            if(lastInsideScope) {
                *pixel(xp+1, lasty-1) = *pixel(xp+1, lasty)= *pixel(xp, lasty+1)=*pixel(xp, lasty-2) = //greyf(side);
                *pixel(xp-1, lasty+1) = *pixel(xp-1, lasty-2)=*pixel(xp-2, lasty-1)=*pixel(xp-2, lasty) = greyf(corner);
                *pixel(xp, lasty-1) = *pixel(xp, lasty)= *pixel(xp-1, lasty-1)=*pixel(xp-1, lasty) = 0;
//            }
        } else /*if(insideScope)*/ {
            double intgr, fr = modf(yp, &intgr);
            *pixel(round(xp), intgr+1) =grey(fr*255);
            *pixel(round(xp), intgr) =grey(255);
            *pixel(round(xp), intgr-1) =grey((1-fr)*255);
//            *((int*)(data+ (((int)(intgr+1))*600+(int)round(xp))*4)) = grey(fr*255);
//            *((int*)(data+ (((int)(intgr))*600+(int)round(xp))*4)) = grey(255);
//            *((int*)(data+ ((int)(intgr-1)*600+(int)round(xp))*4)) = grey((1-fr)*255);
        }
//        if(xp+1 < size && xp - 1 >= 0 &&
//                yp < size && yp - 1 >= 0 ) {
////            fprintf(stderr, "%d\n", xp);

//            for(double i = lasty+sign; abs(i-lasty) < abs(yp-lasty); i+=sign) {
//                double greyRight = fabs(i*1.0-yp)/fabs(lasty-yp);
//                *((int*)(data+ (((int)(intgr)+1)*600+(int)(xp)+1)*4)) = grey(fr*(1-greyRight)*255);
//                *((int*)(data+ ((int)(intgr)*600+(int)xp+1)*4)) = grey((1-fr)*(1-greyRight)*255);

//            }
//        }
//        end:
        lasty = yp;
    }
    x = 0;
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
    while(true) {
        volatile double y = function(x);
        long long int y1, y2, x1, x2;
        numToPicDI64(x, x, &x1, &y1);
        numToPicDI64(y, y, &x2, &y2);
        if(x2 < 0 || y2 > 600) {
            goto nextIter;
        }
        if(x1 > 600 || y1 < 0) {
            break;
        }
        for(int i = y1; i > y2; i--) {
            *pixel(x1, i) = 0xffff0000;
        }
        for(int i = x1; i < x2; i++) {
            *pixel(i, y2) = 0xff00ff00;
        }
        nextIter:
        if(x1 == x2) {
            fprintf(stderr, "hi^))");
            break;
        }
        x = y;
    }
    {
        long long int xi;
        numToPicDI64(x, 0, &xi, &dummy);
        volatile double y = function(x);
        volatile long long int lastyi;
        numToPicDI64(0, y, &dummy, &lastyi);
        xi++;
        while(true) {
            picToNumDD(xi, 0, &x, &dummy);
            y = function(x);
            long long int yi;
            numToPicDI64(0, y, &dummy, &yi);
            if(yi - lastyi < -2) break;
            lastyi = yi;
            xi++;
        }

        picToNumDD(xi+1, 0, &x, &dummy);
    }
    while(true) {
        volatile double y = function(x);
        long long int y1, y2, x1, x2;
        numToPicDI64(x, x, &x1, &y1);
        numToPicDI64(y, y, &x2, &y2);
        if(x2 < 0 || y2 > 600) {
            goto nextIter2;
        }
        if(x1 > 600 || y1 < 0) {
            break;
        }
        for(int i = y1; i > y2; i--) {
            *pixel(x1, i) = 0xffff0000;
        }
        for(int i = x1; i < x2; i++) {
            *pixel(i, y2) = 0xff00ff00;
        }
        nextIter2:
        if(x1 == x2) {
            break;
        }
        x = y;
    }
    memcpy(dataWithSelection, data, 600*600*4);
    int end = clock();
    snprintf(text,sizeof(text), "%lf ms", (double)(end - start) / CLOCKS_PER_SEC*1000);
    guiRedraw();
}
extern char appName1[] = "ordinalIteration1";
int main()
{char appName2[] = "ordinalIteration2";
    guiStartDrawing(appName);
    guiSetSize(rootWindow, size, size+50);
    recalculatePicture();
    XImage *res = XCreateImage(xdisplay, DefaultVisual(xdisplay, DefaultScreen(xdisplay)), 24,
                 ZPixmap, 0, dataWithSelection, size, size, 32,
                         size*4);
//    XdbeBackBuffer bb = XdbeAllocateBackBufferName(xdisplay, rootWindow, XdbeUndefined);
    Window bb = rootWindow;
    GC gc = XCreateGC(xdisplay, bb, 0, NULL);
    XPutImage(xdisplay, rootWindow, gc, res, 0, 0, 0,50, 600, 600);

    Painter pa = {bb, gc};
    getPos = gridGetPos;
    feedbackSize = gridFeedbackSize;
    gridStart.x = 5;
    gridStart.y = 5;
//    Pixmap pixm = XCreatePixmap(xdisplay, bb, 600, 600, xDepth);
//    GC pixgc = XCreateGC(xdisplay, pixm, 0, NULL);
    while(true) {
        guiNextEvent();

        volatile int t;
        bool consume = false;
        if(xEvent.type == Expose)
            XClearWindow(xdisplay, rootWindow);

        loop(&pa, &consume);

//        printf("%d", consume);
        if(xEvent.type == DestroyNotify) {
            goto exit;
        };
//        if(!consume && mode == itersMode) {
//            if(xEvent.type == ButtonPress || xEvent.type == MotionNotify) {
//                double x, y;
//                double xn, yn;
//                picToNum(xEvent.xbutton.x, xEvent.xbutton.y, &x, &y);
//                for(int i = 0; i < 10; i++) {
//                    xn = x; yn = y;
//                    iter(&x, &y);

//                    int xl, yl, xc, yc;
//                    numToPic(x, y, &xc, &yc);
//                    numToPic(xn, yn, &xl, &yl);

//                    guiDrawLine(&pa,xl,yl,xc,yc);
//                }
//            };
//        } else
        if(!consume /*&& mode == selectMode*/) {
            static int startx, starty;
            static bool sel = false;
            if(xEvent.type == ButtonPress) {
                startx = xEvent.xbutton.x;
                starty = xEvent.xbutton.y-50;
                sel = true;
            } else if(xEvent.type == MotionNotify) {
                int endx = xEvent.xbutton.x;
                int endy = xEvent.xbutton.y-50;
#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)<(y)?(x):(y))
                int c = MAX(abs(endx-startx), abs(endy-starty));
                int xm = endx>startx?startx:startx-c;
                int ym = endy>starty?starty:starty-c;
//                int ym = MIN(starty,endy);
//                XSetForeground(xdisplay, pixgc, 0xffffffff);
//                XDrawRectangle(xdisplay, pixm, pixgc,
//                                 xm, ym+50, c, c);
                memcpy(dataWithSelection, data, 600*600*4);
                for(int i = 0; i < c; i++) {
                    *pixelWithSelection(xm+i, ym) =
                    *pixelWithSelection(xm+i, ym+c) =
                    *pixelWithSelection(xm, ym+i) =
                    *pixelWithSelection(xm+c, ym+i) = 0xffffffff;

                }
//                recalculatePicture();
            } if(xEvent.type == ButtonRelease) {
                double sx, sy;
                picToNumDD(startx, starty,
                         &sx, &sy);
                double ex, ey;
                picToNumDD(xEvent.xbutton.x, xEvent.xbutton.y-50,
                         &ex, &ey);
#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)<(y)?(x):(y))
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
        XPutImage(xdisplay, rootWindow, gc, res, 0, 0, 0,50, 600, 600);
//        XPutImage(xdisplay, pixm, pixgc, res, 0, 0, 0,0, 600, 600);
//        XCopyArea(xdisplay, pixm, bb, gc, 0, 0, 600, 600, 0, 50);
//        else if(!consume && mode == zoominMode) {
//            if(xEvent.type == ButtonRelease) {
//                double mouseX = xEvent.xbutton.x;
//                double mouseY = xEvent.xbutton.y;
//                /*
//                 * mouseX * c / 2 - xmnew = mouseX * c - xmold
//                 * mouseY * c / 2 - ymnew = mouseY * c - ymold
//                 *
//                 * xmnew = xmold - mouseX * c *1.5
//                 *
//                 * size/2*c/2/size - xmnew = mouseX * c/size - xmold
//                 * c/2 - size/2*c/2/size - ymnew = c - mouseY * c/size - ymold
//                 * ymnew - c/2 + c/4 = ymold - c + mouseY*c/size
//                 * ymnew = ymold - c + mouseY*c/size + c/2 - c/4
//                 *
//                 * xmnew = xmold - mouseX*c + c/2
//                 */
//                double cxcx = mouseX * c/size - xm;
//                double newxm, newym;
//                double cxcxoldx, d; picToNum((int)mouseX, (int)mouseY, &cxcxoldx, &d);
////                picToNum(mouseX - size/4,
////                         mouseY - size/4,
////                         &newxm,
////                         &newym);
////                picToNum(
//                fprintf(stderr, "xm bef %lf \n", xm);
//                xm = xm-mouseX*c/size + c/4;
//                fprintf(stderr, "xm aft %lf \n", xm);
//                double cxcxnew = size/2*c/size - xm;
//                ym = ym - c + mouseY*c/size + c/2 - c/4;//ym-mouseY*c/size + c/4;
//                c/=2;

//                double cxcxnewx; picToNum(size/2, size/2,&cxcxnewx, &d);
//                fprintf(stderr, "cxcx %lf, cxcxoldx %lf, cxcxnew %lf, "
//                        "cxcxnewx %lf\n", cxcx, cxcxoldx, cxcxnew, cxcxnewx);
//                recalculatePicture();
//            }
//        }
//        XdbeSwapInfo si = {rootWindow, XdbeUndefined};
//        XdbeSwapBuffers(xdisplay, &si, 1);
//        XFlush(xdisplay);
    }
exit:
    XDestroyWindow(xdisplay,
                   rootWindow);

    XCloseDisplay(xdisplay);
    return 0;
}
