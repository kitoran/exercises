#include "ui.h"

#include "graph.h"
#include "audio.h"
#include "gui.h"
#include "gridlayout.h"
#include "globals.h"
#include "actions.h"
#include "persistent.h"
#include "loadImage.h""
#include "alsathread.h"

void ui(Painter *p)
{
    Size size = guiGetSize();
//    fprintf(stderr, "got size %d x %d", size.width, size.height);
    int gh = getGridBottom() + 5;
    graphProcessEvent(&widget/*, p*/, 0, gh, size.width, size.height-gh);
    //    gtk_grid_attach(layout, widget, 4, 0, 1, 17);

    setCurrentGridPos(0, 0);
    guiLabelZT(p, "max");
    int maxx = max;
    setCurrentGridPos(1, 0);
    persistentNumberEdit(p, 6, &maxx, NULL);


    setCurrentGridPos(0, 1);
    guiLabelZT(p, "method");
    setCurrentGridPos(1, 1);
    char* strs[] = {"original", "multiply", "divide", NULL};
    guiComboBoxZT(p, strs, 2);


    setCurrentGridPos(0, 2);
    guiLabelZT(p, "treshold");
    setCurrentGridPos(1, 2);
    int dummy = 0;
    persistentNumberEdit(p, 6, &dummy, NULL);


    setCurrentGridPos(0, 3);
    guiLabelZT(p, "frequency cadr");
    setCurrentGridPos(1, 3);
//    int dummy2 = 44100;
    persistentNumberEdit(p, 7, &cutoff, NULL);

    setCurrentGridPos(0, 4);
    guiLabelZT(p,  "number of harmonics");
    setCurrentGridPos(1, 4);
    persistentNumberEdit(p, 6, &numberOfHarmonics, NULL);


    setCurrentGridPos(0, 5);
    char label_2[] = "window (sampls)";
    guiLabel(p, label_2, sizeof(label_2)-1);    
    setCurrentGridPos(1, 5);
    int ws = windowSize;
    persistentNumberEdit(p,  5, &ws, NULL);

    setCurrentGridPos(0,6);
    if(guiButtonZT(p, "redraw")) {
        Painter p = {
            widget.spectrogramDrawing,
            XCreateGC(xdisplay, widget.spectrogramDrawing, 0, NULL)
        };
        guiFillRectangle(&p, 0, 0, widget.width, widget.height);
        spectrogram->draw(spectrogram, &p, widget.width, widget.height);
        XFreeGC(xdisplay, p.gc);

//        spectrogram->draw(spectrogram, &p, widget.width, widget.height);
        guiRedraw();
    }
    setCurrentGridPos(1, 6);
    if(guiButtonZT(p, "<|>")) {
        MainWindowon_toolButton_2_clicked();
    }

    setCurrentGridPos(0, 7);
    char toolButton[] = ">";
    if(guiButton(p, toolButton, sizeof(toolButton)-1)) {
        MainWindowon_toolButton_clicked();
    }

//    static XImage* recIcon = loadLocalImageZT("rec.png");
    setCurrentGridPos(1, 7);
    if(recordingInAThread) {
        if(resourseToolButton(p, "stop.png", 0)) {
            stopRecordingInAThread();
            calculateSpectrogram();
            redrawSpectrogram(&widget);
        }
    } else {
        if(resourseToolButton(p, "rec.png", 0)) {
            startRecordingInAThread();
        }
    }

    setCurrentGridPos(0, 9);
    char label_3[] = "windowFrequency (sampls)";
    guiLabel(p, label_3, sizeof(label_3)-1);
    setCurrentGridPos(1, 9);
    int step = stepSize;
    persistentNumberEdit(p,  5, &step, NULL);


    setCurrentGridPos(0, 10);
    guiLabelZT(p, "denominstor");
    setCurrentGridPos(1,10);
    guiDoubleEdit(p, 6, &denominator);

    setCurrentGridPos(0, 11);
    guiLabelZT(p, "num of samples");
    int s = arrlen(samplsStbArray);
    setCurrentGridPos(1, 11);
    guiNumberEdit(p, 6, &s, NULL);





    setCurrentGridPos(3,0);
    if(guiButtonZT(p, "audacity")) {
        MainWindowon_pushButton_clicked();
    }

}
