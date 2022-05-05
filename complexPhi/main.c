#include <stdio.h>
#include <math.h>
#include <string.h>
#include <X11/Xlib.h>
#include "gui.h"
#include "time.h"
#include "loadImage.h"
#include "gridlayout.h"
#include "persistent.h"
#include "color.h"
#include "complex.h"
#define tau 6.283185307179586
XEvent xEvent;
char *appName = "complexPhi";
int size = 600;
unsigned char data[600*600*4];
int itNum = 0;
double c = 8;
double xm = 4;
double ym = 4;
void picToNum (int x, int y, double* restrict rx, double* restrict ry) {
    *rx = x*c/size - xm;
    *ry = c - y*c/size - ym;
}
void numToPic (double x, double y,int * restrict rx, int * restrict ry) {
    *rx = (x+xm)/c*size;
    *ry = -(y +ym - c)/c*size;
}

bool eisen= false;
double maxDenom = 30;//1;//12;
double sqrt2 = 1.414213563;
void recalculatePicture() {


    /* here, do your time-consuming job */
    volatile bool condition = true;
    memset(data, 0, size*size*4);
    double _Complex w = -1.0/2 + I*sqrt(3)/2;
    double _Complex w2 = -1.0/2 - I*sqrt(3)/2;
    clock_t begin = clock();
    for(double reNum = ceil(-maxDenom*sqrt2); reNum < floor(maxDenom*sqrt2); reNum++)
    for(double imNum = ceil(-maxDenom*sqrt2); imNum < floor(maxDenom*sqrt2); imNum++)
//    for(double rimNum = ceil(-maxDenom*sqrt2); rimNum < floor(maxDenom*sqrt2); rimNum++)
    for(double reDen = ceil(-maxDenom); reDen < floor(maxDenom); reDen++)
    for(double imDen = ceil(-maxDenom); imDen < floor(maxDenom); imDen++)
//    for(double rimDen = ceil(-maxDenom); rimDen < floor(maxDenom); rimDen++)
    {
//        double imDen = 0;
//        if(reDen*reDen+imDen*imDen>maxDenom*maxDenom) {
//        if(reDen*imDen>maxDenom*maxDenom) {
//            if(fabs(reDen) > maxDenom || fabs(imDen)>maxDenom|| fabs(rimDen)>maxDenom) {
        //        if(reDen*reDen+imDen*imDen - reDen*imDen > maxDenom*maxDenom) {
        if(reDen > maxDenom || imDen > maxDenom) {
            continue;
        }
        double _Complex rel;
        if(false)
            rel = (reNum/reDen) +  w*(imNum/imDen);
            //                double _Complex rel = (reNum/reDen) +  w*(imNum/imDen) + w2*(rimNum/rimDen);
        else
                rel = (reNum/reDen) +  I*(imNum/imDen);
//      double _Complex rel = (reNum+I*imNum)/(reDen+I*imDen);
//      double _Complex rel = (reNum+w*imNum)/(reDen+w*imDen);
        int x, y; numToPic(creal(rel), cimag(rel), &x, &y); y--;
        if(x >= 0 && x < size &&
            y >= 0 && y < size) {
            ((int*)data)[y*size+x] = 0xffffffff;
        }
    }


    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    fprintf(stderr, "\n%lf secs\n", time_spent);
    int xp, yp;
    numToPic(0.618033988, 0, &xp, &yp); yp--;
    if(xp+1 < size && xp - 1 >= 0 &&
            yp < size && yp >= 0 ) {

        fprintf(stderr, "%d\n", xp);
        *((int*)(data+ (yp*600+xp)*4)) = 0xffff00ff;
        *((int*)(data+ (yp*600+xp-1)*4)) = 0xffff00ff;
        *((int*)(data+ (yp*600+xp+1)*4)) = 0xfffff00ff;
        *((int*)(data+ (yp*600+xp+1)*4)) = 0xffff00ff;
        *((int*)(data+ (yp*600+xp-1)*4)) = 0xffff00ff;
        *((int*)(data+ (yp*600+xp)*4)) = 0xffff00ff;
    }
}
XImage *res;
enum { itersMode, selectMode, zoominMode} mode;
void loop(Painter* pa, bool* consume) {

    setCurrentGridPos(0,0);
//    bool res = false;
    int e = maxDenom;
    bool recalc = /*persistent*/guiNumberEdit(pa, 5, &e, consume);

    setCurrentGridPos(0,1);
    if(resourseToolButton(pa, "minus.png", consume)) {
        maxDenom--;
        recalc = /*res = */true;
    }
    setCurrentGridPos(0,2);
    if(resourseToolButton(pa, "plus.png", consume)) {
        maxDenom++;
        recalc = /*res = */true;
    }
    setCurrentGridPos(0,3);
    if(resourseToolButton(pa, "select.png", consume)) {
        mode = selectMode;
//        res = true;
    }
    setCurrentGridPos(0,4);
    if(resourseToolButton(pa, "iters.png", consume)) {
        mode = itersMode;
//        res = true;
    }
    setCurrentGridPos(0,5);
    if(resourseToolButton(pa, "zoomin.png", consume)) {
        mode = zoominMode;
//        res = true;
        eisen = true;
    }
//    setCurrentGridPos(0,6);
//    if(resourseToolButton(pa, "zoomout.png", consume)) {
//        mode = zoominMode;
////        res = true;
//    }
    setCurrentGridPos(0,7);
    if(resourseToolButton(pa, "100percent.png", consume)) {
        c = 1;
        xm = 0;
        ym = 0;
        recalc/* = res */= true;
    }

    gridNextColumn();
    if(resourseToolButton(pa, "save.png", consume)) {
        saveImageSomewhereNewWrongChannelsZT(res, "gauss");
    }


    if(recalc) {
        recalculatePicture();
    }
//    return res;
}
int main(int argc)
{
    guiStartDrawing();
    guiSetSize(rootWindow, size, size+30);
    recalculatePicture();
    res = XCreateImage(xdisplay, DefaultVisual(xdisplay, DefaultScreen(xdisplay)), 24,
                 ZPixmap, 0, data, size, size, 32,
                         size*4);
    GC gc = XCreateGC(xdisplay, rootWindow, 0, NULL);
//    XPutImage(xdisplay, rootWindow, gc, res, 0, 0, 0,0, 600, 600);
    XSetWindowBackground(xdisplay, rootWindow, 0x205020);
    Painter pa = {rootWindow, gc};
    getPos = gridGetPos;
    feedbackSize = gridFeedbackSize;
    gridStart.x = 5;
    gridStart.y = 5;

    while(true) {
        guiNextEvent();
        volatile Size wsize = guiGetSize();bool resize = false;
        if(wsize.height < getGridBottom()+size) {
            wsize.height = getGridBottom()+size;
            resize = true;
        }
        if(wsize.width < size) {
            wsize.width = size;
            resize = true;
        }
        if(resize) guiSetSize(rootWindow, wsize.width, wsize.height);
        XPutImage(xdisplay, rootWindow, gc, res, 0, 0, 0,getGridBottom(), size, size);
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
        if(!consume && mode == selectMode) {
            static int startx, starty;
            static bool sel = false;
            if(xEvent.type == ButtonPress) {
                startx = xEvent.xbutton.x;
                starty = xEvent.xbutton.y-getGridBottom();
                sel = true;
            } else if(xEvent.type == ButtonRelease) {
                double sx, sy;
                picToNum(startx, starty,
                         &sx, &sy);
                double ex, ey;
                picToNum(xEvent.xbutton.x, xEvent.xbutton.y-getGridBottom(),
                         &ex, &ey);
#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)<(y)?(x):(y))
                c = MAX(fabs(ex-sx), fabs(ey-sy));
                xm = -MIN(sx,ex);
                ym = -MIN(sy,ey);
                sel = false;
                recalculatePicture();
            }

        } else if(!consume && mode == zoominMode) {
            if(xEvent.type == ButtonRelease) {
                double mouseX = xEvent.xbutton.x;
                double mouseY = xEvent.xbutton.y-getGridBottom();
                /*
                 * mouseX * c / 2 - xmnew = mouseX * c - xmold
                 * mouseY * c / 2 - ymnew = mouseY * c - ymold
                 *
                 * xmnew = xmold - mouseX * c *1.5
                 *
                 * size/2*c/2/size - xmnew = mouseX * c/size - xmold
                 * c/2 - size/2*c/2/size - ymnew = c - mouseY * c/size - ymold
                 * ymnew - c/2 + c/4 = ymold - c + mouseY*c/size
                 * ymnew = ymold - c + mouseY*c/size + c/2 - c/4
                 *
                 * xmnew = xmold - mouseX*c + c/2
                 */
                double cxcx = mouseX * c/size - xm;
                double newxm, newym;
                double cxcxoldx, d; picToNum((int)mouseX, (int)mouseY, &cxcxoldx, &d);
//                picToNum(mouseX - size/4,
//                         mouseY - size/4,
//                         &newxm,
//                         &newym);
//                picToNum(
                fprintf(stderr, "xm bef %lf \n", xm);
                xm = xm-mouseX*c/size + c/4;
                fprintf(stderr, "xm aft %lf \n", xm);
                double cxcxnew = size/2*c/size - xm;
                ym = ym - c + mouseY*c/size + c/2 - c/4;//ym-mouseY*c/size + c/4;
                c/=2;

                double cxcxnewx; picToNum(size/2, size/2,&cxcxnewx, &d);
                fprintf(stderr, "cxcx %lf, cxcxoldx %lf, cxcxnew %lf, "
                        "cxcxnewx %lf\n", cxcx, cxcxoldx, cxcxnew, cxcxnewx);
                recalculatePicture();
            }
        }
    }
exit:
    XDestroyWindow(xdisplay,
                   rootWindow);

    XCloseDisplay(xdisplay);
    return 0;
}
