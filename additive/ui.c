#include "ui.h"

#include "graph.h"
#include "audio.h"
#include "gui.h"
#include "gridlayout.h"
#include "globals.h"
#include "actions.h"
#include "persistent.h"
#include "loadImage.h"
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
    gridNextRow();
    persistentNumberEdit(p, 6, &maxx, NULL);


    gridNextColumn();
    guiLabelZT(p, "method");
    gridNextRow();
    INTROSPECT_ENUM(method, original, multiply, divide);
    static int cmethod = 2;
    persistentEnumComboBox(method, p, &cmethod);

    gridNextColumn();
    guiLabelZT(p, "action");
    gridNextRow();
    persistentEnumComboBox(MouseAction, p, &action);

    gridNextColumn();
    guiLabelZT(p, "treshold");
    gridNextRow();
    int dummy = 0;
    persistentNumberEdit(p, 6, &dummy, NULL);


    gridNextColumn();
    guiLabelZT(p, "frequency cadr");
    gridNextRow();
//    int dummy2 = 44100;
    persistentNumberEdit(p, 7, &cutoff, NULL);

    gridNextColumn();
    guiLabelZT(p,  "number of harmonics");
    gridNextRow();
    persistentNumberEdit(p, 6, &numberOfHarmonics, NULL);


    gridNextColumn();
    char label_2[] = "window (sampls)";
    guiLabel(p, label_2, sizeof(label_2)-1);
    gridNextRow();
    int ws = windowSize;
    persistentNumberEdit(p,  5, &ws, NULL);

    gridNextColumn();
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
    gridNextRow();
    if(guiButtonZT(p, "<|>")) {
        MainWindowon_toolButton_2_clicked();
    }

    gridNextColumn();
    char toolButton[] = ">";
    if(guiButton(p, toolButton, sizeof(toolButton)-1)) {
        MainWindowon_toolButton_clicked();
    }

//    static XImage* recIcon = loadLocalImageZT("rec.png");
    gridNextRow();
    if(recordingInAThread) {
        if(resourseToolButton(p, "stop.png", 0)) {
            stopRecordingInAThread();
            calculateSpectrogram();
            redrawSpectrogram(&widget);
        }
    } else {
        if(resourseToolButton(p, "rec.png", 0)) {
            startRecordingInAThread();
            guiRedraw();
        }
    }

    gridNextColumn();
    char label_3[] = "windowFrequency (sampls)";
    guiLabel(p, label_3, sizeof(label_3)-1);
    gridNextRow();
    int step = stepSize;
    persistentNumberEdit(p,  5, &step, NULL);


    gridNextColumn();
    guiLabelZT(p, "denominstor");
    gridNextRow();
    guiDoubleEdit(p, 6, &denominator);

    gridNextColumn();
    guiLabelZT(p, "num of samples");
    int s = arrlen(samplsStbArray);
    gridNextRow();
    guiNumberEdit(p, 6, &s, NULL);


//    setCurrentGridPos(0, 12);
//    char* reeee[] = {"hi", "bestie", NULL};
//    guiComboBoxZT(p, reeee, 1);



    setCurrentGridPos(3,0);
    if(guiButtonZT(p, "audacity")) {
        MainWindowon_pushButton_clicked();
    }

}
