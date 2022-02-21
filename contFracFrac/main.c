#include <stdio.h>
#include <X11/Xlib.h>
#include "gui.h"
#include "gridlayout.h"
#include "persistent.h"
XEvent xEvent;
char appName[] = "contfracfrac";
//int getColor(int blue, int red) {
//    // or = rb*b + rr*r + ofr
//    // og = gb*b + gr*g + ofg
//    // ob = bb*b + br*r + ogb

//    // 255 = rb*
//}
void iter( double* restrict x, double* restrict  y) {
    *x = *x - floor(*x);
    *y = *y - floor(*y);
    double denom = *x**x+*y**y;
    *x = *x/denom;
    *y = -*y/denom;
    *x = *x - floor(*x);
    *y = *y - floor(*y);

}
int size = 600;
unsigned char data[600*600*4];
int itNum = 0;
double c = 1;
double xm = 0;
double ym = 0;
void picToNum (int x, int y, double* restrict rx, double* restrict ry) {
    *rx = x*c/size - xm;
    *ry = c - y*c/size - ym;
}
void numToPic (double x, double y,int * restrict rx, int * restrict ry) {
    *rx = (x+xm)/c*size;
    *ry = -(y +ym - c)/c*size;
}
void loop(Painter* pa) {

    setCurrentGridPos(0,0);
    bool recalc = guiNumberEdit(pa, 5, &itNum);

    setCurrentGridPos(0,1);
    if(resourseToolButton(pa, "minus.png")) {
        itNum--;
        recalc = true;
    }
    setCurrentGridPos(0,2);
    if(resourseToolButton(pa, "plus.png")) {
        itNum++;
        recalc = true;
    }

    if(recalc) {
        recalculatePicture();
    }
}
void recalculatePicture() {
    for(int i = 0; i < 600; i++) {
        for(int j = 0; j < 600; j++) {
            bool in = false;
//            ((int*)data)[i*600+j] =
            double xn,yn;
            picToNum(i, j, &xn, &yn);
            int cn = 0;
            for(; cn < itNum; cn ++) {
//                double xw = xn,yw = yn;

//                iter(&xw, &yw);
                iter(&xn, &yn);
                bool lowC = (xn-0.5)*(xn-0.5)+yn*yn>1.0/4 ;
                if(xn > 0.5 && lowC && (xn-0.5)*(xn-0.5)+(yn-1)*(yn-1)>1.0/4) {
                    in = true;
//                    fprintf(stderr, "%d\n", cn);
                    break;
                }
//                if(xw > 0 && xw < 1
//                        && yw > 0 && yw < 1) {
//                    in = true;
//                }
//                if(xn

            }
#define CAP(x) ((x)>255?0:(x)<0?0:(x))
            if(in) {
                data[(j*600+i)*4] = (itNum-cn)*1.0/itNum*255;
                data[(j*600+i)*4+1] = 0;//CAP((xn+xm)/c*255);//0;
                data[(j*600+i)*4+2] = 0;//CAP((yn+ym)/c*255);//0;
                data[(j*600+i)*4+3] = cn*1.0/itNum*255;
            } else {
                data[(j*600+i)*4] = 0;
                data[(j*600+i)*4+1] = 0;//
                data[(j*600+i)*4+2] = 0;//CAP((yn+ym)/c*255);
                data[(j*600+i)*4+3] = 0;//CAP((xn+xm)/c*255);;
            }
        }
    }
    int xp, yp;
    numToPic(1.618033988, 0, &xp, &yp);
    fprintf(stderr, "%d\n", xp);
    *((int*)(data+ (599*600+xp)*4)) = 0x000000ff;
    *((int*)(data+ (599*600+xp-1)*4)) = 0x000000ff;
    *((int*)(data+ (599*600+xp+1)*4)) = 0x000000ff;
    *((int*)(data+ (598*600+xp+1)*4)) = 0x000000ff;
    *((int*)(data+ (598*600+xp-1)*4)) = 0x000000ff;
    *((int*)(data+ (598*600+xp)*4)) = 0x000000ff;
    numToPic(0.618033988, 0, &xp, &yp);
    fprintf(stderr, "%d\n", xp);
    *((int*)(data+ (599*600+xp)*4)) = 0x000000ff;
    *((int*)(data+ (599*600+xp-1)*4)) = 0x000000ff;
    *((int*)(data+ (599*600+xp+1)*4)) = 0x000000ff;
    *((int*)(data+ (598*600+xp+1)*4)) = 0x000000ff;
    *((int*)(data+ (598*600+xp-1)*4)) = 0x000000ff;
    *((int*)(data+ (598*600+xp)*4)) = 0x000000ff;

}

int main()
{
    guiStartDrawing();
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
        loop(&pa);
        if(xEvent.type == DestroyNotify) {
            goto exit;
        };
    }
exit:
    XDestroyWindow(xdisplay,
                   rootWindow);

    XCloseDisplay(xdisplay);
    return 0;
}
