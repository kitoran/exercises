//#include "mainwindow.h"

//#include <QApplication>
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();
    const double phi =  /3;;//1+1.41421356237;
    double rem = phi;//fmod(phi, 1);

    for(int i = 0; i < 10; i++) {
        int r = round(rem);
        if(r < rem) {
            fprintf(stderr, "%d + 1/(", r);
            rem = 1/(rem-r);
        } else {
            fprintf(stderr, "%d - 1/(", r);
            rem = 1/(r-rem);
        }
    }
    double a = 1;
    for(int i = 0; i < 300; i++) {
        a = 2 + 1/a;
        a = 2 - 1/a;
    }
    fprintf(stderr, "\n%.20lf\n", a);

//    fprintf(stderr, "");
}
