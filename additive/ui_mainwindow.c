#include "ui_mainwindow.h"

#include "graph.h"
#include "gui.h"
#include "gridlayout.h"
#include "globals.h"
#include "mainwindow.h"

void setupUi(Painter *p)
{
//    int deb1, deb2;
//    if(xEvent.type == ResizeRequest) {
//        deb1 = xEvent.xresizerequest.width;
//        deb2 = xEvent.xresizerequest.height;
//        guiSetSize(rootWindow, deb1, deb2);
//    }
//    Size size = guiGetSize();
//    fprintf(stderr, "got size %d x %d", size.width, size.height);
//    int gh = getGridBottom() + 5;
//    graphProcessEvent(&widget/*, p*/, 0, gh, size.width, size.height-gh);
//    //    gtk_grid_attach(layout, widget, 4, 0, 1, 17);

    setCurrentGridPos(0, 8);
    char label_2[] = "window (sampls)";
    guiLabel(p, label_2, sizeof(label_2)-1);


    setCurrentGridPos(0, 12);
    char label_3[] = "windowFrequency (sampls)";
    guiLabel(p, label_3, sizeof(label_3)-1);

    setCurrentGridPos(2,11);
    char toolButton[] = ">";
    if(guiButton(p, toolButton, sizeof(toolButton)-1)) {
        MainWindowon_toolButton_clicked();
    }

    setCurrentGridPos(2, 1);
    char* strs[] = {"original", "multiply", "divide", NULL};
    guiComboBoxZT(p, strs, 2);

    setCurrentGridPos(2,16);
    guiDoubleEdit(p, 6, &denominator);

    setCurrentGridPos(2,2);
    int dummy;
    guiNumberEdit(p, 6, &dummy);

    setCurrentGridPos(2, 0);
    guiNumberEdit(p, 6, &dummy);

    setCurrentGridPos(0,2);
    guiLabelZT(p, "treshold");

    setCurrentGridPos(0,1);
    guiLabelZT(p, "method");

    setCurrentGridPos(2,5);
    int dummy2 = 44100;
    guiNumberEdit(p, 7, &dummy2);

    setCurrentGridPos(0,16);
    guiLabelZT(p, "denominstor");

    setCurrentGridPos(2,10);
    if(guiButtonZT(p, "<|>")) {
        MainWindowon_toolButton_2_clicked();
    }

    setCurrentGridPos(2, 12);
    guiNumberEdit(p,  5, &dummy);

    setCurrentGridPos(2, 8);
    guiNumberEdit(p,  5, &dummy);

    setCurrentGridPos(0,0);
    guiLabelZT(p, "max");

    setCurrentGridPos(0, 5);
    guiLabelZT(p, "frequency cadr");

    setCurrentGridPos(3,0);
    if(guiButtonZT(p, "audacity")) {
        MainWindowon_pushButton_clicked();
    }

    setCurrentGridPos(0, 6);
    guiLabelZT(p,  "number of harmonics");

    setCurrentGridPos(2, 6);
    guiNumberEdit(p, 6, &numberOfHarmonics);
}
