﻿#include <stdio.h>
#include <X11/Xlib.h>
extern char appName[] = "ordinalIteration";
#include "gui.h"
#include "gridlayout.h"
#include "math.h"
#include "persistent.h"
#include "color.h"
#include "complex.h"
#define tau 6.283185307179586
XEvent xEvent;
int size = 600;
unsigned char data[600*600*4];
int itNum = 0;
double c = 8;
double xm = 4;
double ym = 4;
void picToNum (double x, double y, double* restrict rx, double* restrict ry) {
    *rx = x*c/size - xm;
    *ry = c - y*c/size - ym;
}
void numToPic (double x, double y,double * restrict rx, double * restrict ry) {
    *rx = (x+xm)/c*size;
    *ry = -(y +ym - c)/c*size;
}
enum { itersMode, selectMode, zoominMode} mode;
void loop(Painter* pa, bool* consume) {

//    setCurrentGridPos(0,0);
////    bool res = false;
//    bool recalc = guiNumberEdit(pa, 5, &itNum, consume);

//    setCurrentGridPos(0,1);
//    if(resourseToolButton(pa, "minus.png", consume)) {
//        itNum--;
//        recalc = /*res = */true;
//    }
//    setCurrentGridPos(0,2);
//    if(resourseToolButton(pa, "plus.png", consume)) {
//        itNum++;
//        recalc = /*res = */true;
//    }
//    setCurrentGridPos(0,3);
//    if(resourseToolButton(pa, "select.png", consume)) {
//        mode = selectMode;
////        res = true;
//    }
//    setCurrentGridPos(0,4);
//    if(resourseToolButton(pa, "iters.png", consume)) {
//        mode = itersMode;
////        res = true;
//    }
//    setCurrentGridPos(0,5);
//    if(resourseToolButton(pa, "zoomin.png", consume)) {
//        mode = zoominMode;
////        res = true;
//    }
////    setCurrentGridPos(0,6);
////    if(resourseToolButton(pa, "zoomout.png", consume)) {
////        mode = zoominMode;
//////        res = true;
////    }
//    setCurrentGridPos(0,7);
//    if(resourseToolButton(pa, "100percent.png", consume)) {
//        c = 1;
//        xm = 0;
//        ym = 0;
//        recalc/* = res */= true;
//    }

//    if(recalc) {
//        recalculatePicture();
//    }
//    return res;
}
int grey(int v) {
    int vv = v | v << 8;
    return vv | vv<<8;
}
void recalculatePicture() {
    double dummy;
    double x;
    picToNum(0,0,&x,&dummy);
    double lasty ;
    numToPic(x, x*x, &dummy, &lasty);
    for(int i = 0; i < 600 - 1; i++) {
        double xp, yp;
        picToNum(i,0,&x,&dummy);
        numToPic(x, x*x, &xp, &yp);
        if(!(xp+10 < size && xp - 10 >= 0 &&
                                        yp+10 < size && yp - 10 >= 0)) {
            goto end;
        }
        if(fabs(yp-lasty) > 1) {
            int sign = yp > lasty? 1 : -1;
            for(double i = lasty/*+sign*/; abs(i-lasty) <= abs(yp-lasty); i+=sign) {
                double greyRight = fabs(i*1.0-yp)/fabs(lasty-yp);
                *((int*)(data+ ((int)round(i)*600+(int)round(xp))*4)) = grey((greyRight)*255);
                *((int*)(data+ ((int)round(i)*600+(int)round(xp)+1)*4)) = grey((1-greyRight)*255);
            }

        } else {
            double intgr, fr = modf(yp, &intgr);
            *((int*)(data+ (((int)(intgr))*600+(int)round(xp))*4)) = grey(fr*255);
            *((int*)(data+ ((int)(intgr-1)*600+(int)round(xp))*4)) = grey((1-fr)*255);
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
        end:
        lasty = yp;
    }
}

extern char appName1[] = "ordinalIteration1";
int main()
{char appName2[] = "ordinalIteration2";
    guiStartDrawing(appName);
    guiSetSize(rootWindow, size, size);
    recalculatePicture();
    XImage *res = XCreateImage(xdisplay, DefaultVisual(xdisplay, DefaultScreen(xdisplay)), 24,
                 ZPixmap, 0, data, size, size, 32,
                         size*4);
    GC gc = XCreateGC(xdisplay, rootWindow, 0, NULL);
    XPutImage(xdisplay, rootWindow, gc, res, 0, 0, 0,0, 600, 600);

    Painter pa = {rootWindow, gc};
    getPos = gridGetPos;
    feedbackSize = gridFeedbackSize;
    gridStart.x = 5;
    gridStart.y = 5;

    while(true) {
        guiNextEvent();
        XPutImage(xdisplay, rootWindow, gc, res, 0, 0, 0,0, 600, 600);
        volatile int t;
        if(xEvent.type == ButtonPress) {
           fprintf(stderr, "hi%d", t);
        }
        bool consume = false;
        loop(&pa, &consume);
        printf("%d", consume);
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
//        } else if(!consume && mode == selectMode) {
//            static int startx, starty;
//            static bool sel = false;
//            if(xEvent.type == ButtonPress) {
//                startx = xEvent.xbutton.x;
//                starty = xEvent.xbutton.y;
//                sel = true;
//            } else if(xEvent.type == ButtonRelease) {
//                double sx, sy;
//                picToNum(startx, starty,
//                         &sx, &sy);
//                double ex, ey;
//                picToNum(xEvent.xbutton.x, xEvent.xbutton.y,
//                         &ex, &ey);
//#define MAX(x,y) ((x)>(y)?(x):(y))
//#define MIN(x,y) ((x)<(y)?(x):(y))
//                c = MAX(fabs(ex-sx), fabs(ey-sy));
//                xm = -MIN(sx,ex);
//                ym = -MIN(sy,ey);
//                sel = false;
//                recalculatePicture();
//            }

//        } else if(!consume && mode == zoominMode) {
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
    }
exit:
    XDestroyWindow(xdisplay,
                   rootWindow);

    XCloseDisplay(xdisplay);
    return 0;
}
