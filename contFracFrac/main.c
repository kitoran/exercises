#include <stdio.h>
#include <X11/Xlib.h>
#include "gui.h"
#include "gridlayout.h"
#include "persistent.h"
#include "color.h"
#include "complex.h"
#define tau 6.283185307179586
XEvent xEvent;
char appName[] = "contfracfrac";
//int getColor(int blue, int red) {
//    // or = rb*b + rr*r + ofr
//    // og = gb*b + gr*g + ofg
//    // ob = bb*b + br*r + ogb

//    // 255 = rb*
//}
void iter( double* restrict x, double* restrict  y) {
//    *x = *x - floor(*x);
//    *y = *y - floor(*y);
    double denom = *x**x+*y**y;
    *x = *x/denom;
    *y = -*y/denom;
    *x = *x - floor(*x);
    *y = *y - floor(*y);////
    if(*x**x+*y**y > 1) {
        if((*x-1)*(*x-1) +*y**y >1) {
            *y -= 1;
        } else {
            *x -= 1;
        }
    }
//    _Complex double r = *x + I * *y;
//    r = 1/r;
//    double abs = cabs(r);
//    double frac = abs - floor(abs);
//    r = r / abs * frac;
//    *x = creal(r); *y = cimag(r);
}
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
enum { itersMode, selectMode, zoominMode} mode;
void loop(Painter* pa, bool* consume) {

    setCurrentGridPos(0,0);
//    bool res = false;
    bool recalc = guiNumberEdit(pa, 5, &itNum, consume);

    setCurrentGridPos(0,1);
    if(resourseToolButton(pa, "minus.png", consume)) {
        itNum--;
        recalc = /*res = */true;
    }
    setCurrentGridPos(0,2);
    if(resourseToolButton(pa, "plus.png", consume)) {
        itNum++;
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

    if(recalc) {
        recalculatePicture();
    }
//    return res;
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
//                bool lowC = (xn-0.5)*(xn-0.5)+yn*yn>1.0/4 ;
//                if(xn > 0.5 && lowC && (xn-0.5)*(xn-0.5)+(yn-1)*(yn-1)>1.0/4) {
//                    in = true;
//                    fprintf(stderr, "%d\n", cn);
//                    break;
//                }
//                if(xw > 0 && xw < 1
//                        && yw > 0 && yw < 1) {
//                    in = true;
//                }
//                if(xn

            }
#define CAP(x) ((x)>255?0:(x)<0?0:(x))
//            ((int*)data)[j*600+i] = CAP((int)xn*255/size) << 16 | CAP((int)yn*255/size) << 8;
            ((int*)data)[j*600+i] = hsvd2bgr((carg(xn + I*yn)+tau/2)*360/tau, 1, cabs(xn + I*yn));
//            if(in) {
//                    ((int*)data)[j*600+i] = hsvd2bgr((itNum-cn)*1.0/itNum*360, 1, (itNum-cn)*1.0/itNum);
//            ((int*)data)[j*600+i] = hsvd2bgr(((double)(j))/size*360, 1, 0.5);
//                data[()*4] = (itNum-cn)*1.0/itNum*255;//b
//                data[(j*600+i)*4+1] = 0;//CAP((xn+xm)/c*255);//0;//g
//                data[(j*600+i)*4+2] = 0;//CAP((yn+ym)/c*255);//0;//r
//                data[(j*600+i)*4+3] = 0;..nothing
//            } else {
//                data[(j*600+i)*4] = 0;
//                data[(j*600+i)*4+1] = 0;//
//                data[(j*600+i)*4+2] = 0;//CAP((yn+ym)/c*255);
//                data[(j*600+i)*4+3] = 0;//CAP((xn+xm)/c*255);;
//            }
        }
    }
//    numToPic(1.618033988, 0, &xp, &yp);
//    fprintf(stderr, "%d\n", xp);
//    *((int*)(data+ (599*600+xp)*4)) = 0x000000ff;
//    *((int*)(data+ (599*600+xp-1)*4)) = 0x000000ff;
//    *((int*)(data+ (599*600+xp+1)*4)) = 0x000000ff;
//    *((int*)(data+ (598*600+xp+1)*4)) = 0x000000ff;
//    *((int*)(data+ (598*600+xp-1)*4)) = 0x000000ff;
//    *((int*)(data+ (598*600+xp)*4)) = 0x000000ff;
    int xp, yp;
    numToPic(0.618033988, 0, &xp, &yp);
    if(xp+1 < size && xp - 1 >= 0 &&
            yp < size && yp - 1 >= 0 ) {

        fprintf(stderr, "%d\n", xp);
        *((int*)(data+ (yp*600+xp)*4)) = 0xffffffff;
        *((int*)(data+ (yp*600+xp-1)*4)) = 0xffffffff;
        *((int*)(data+ (yp*600+xp+1)*4)) = 0xffffffff;
        *((int*)(data+ (yp*600+xp+1)*4)) = 0xffffffff;
        *((int*)(data+ (yp*600+xp-1)*4)) = 0xffffffff;
        *((int*)(data+ (yp*600+xp)*4)) = 0xffffffff;
    }
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
        if(!consume && mode == itersMode) {
            if(xEvent.type == ButtonPress || xEvent.type == MotionNotify) {
                double x, y;
                double xn, yn;
                picToNum(xEvent.xbutton.x, xEvent.xbutton.y, &x, &y);
                for(int i = 0; i < 10; i++) {
                    xn = x; yn = y;
                    iter(&x, &y);

                    int xl, yl, xc, yc;
                    numToPic(x, y, &xc, &yc);
                    numToPic(xn, yn, &xl, &yl);

                    guiDrawLine(&pa,xl,yl,xc,yc);
                }
            };
        } else if(!consume && mode == selectMode) {
            static int startx, starty;
            static bool sel = false;
            if(xEvent.type == ButtonPress) {
                startx = xEvent.xbutton.x;
                starty = xEvent.xbutton.y;
                sel = true;
            } else if(xEvent.type == ButtonRelease) {
                double sx, sy;
                picToNum(startx, starty,
                         &sx, &sy);
                double ex, ey;
                picToNum(xEvent.xbutton.x, xEvent.xbutton.y,
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
                double mouseY = xEvent.xbutton.y;
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
