#include "widget.h"
#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
//    QDir d = QDir::current();
//    for(QString name : d.entryList()) {
//        bool succ;
//        int dot = name.indexOf('.');
//        QString numS = name.left(dot);
//        numS = numS.right(numS.size() - 7);
//        int num = numS.toInt(&succ);
//        if(succ ) {
//            QFile f(name);
//            f.rename(QString("alliterswithcut/frame%0.png").arg(num/2));
////            d.rename(name, QString("alliters/frame%0").arg(num/2));
//        }
////        frameq0.000000000000000000000000000000000000000000000000


//    }



    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
