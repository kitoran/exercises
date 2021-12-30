#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"
#include "audio.h"
#include "spectrogram.h"
#include "synthesis.h"
#include "globals.h"
#include <sndfile.h>
#include "newfile.h"

//MainWindow::MainWindow(/*QWidget *parent,*/
/*                       double *data_,
                       int size_,
                       double max_*///) :
//    QMainWindow(parent),
//    ui(new Ui::MainWindow)

void createMainWindow(GtkGrid* grid){
    setupUi(grid);
//    g=widget;

    gtk_signal_connect(GTK_OBJECT (toolButton), "clicked",
                     GTK_SIGNAL_FUNC(MainWindowon_toolButton_clicked), 0);
    gtk_signal_connect(GTK_OBJECT (denominatorEntry), "changed",
                     GTK_SIGNAL_FUNC(MainWindowon_denominator_textChanged), 0);
    gtk_signal_connect(GTK_OBJECT (toolButton_2), "clicked",
                     GTK_SIGNAL_FUNC(MainWindowon_toolButton_2_clicked), 0);
    gtk_signal_connect(GTK_OBJECT (pushButton), "clicked",
                     GTK_SIGNAL_FUNC(MainWindowon_toolButton_2_clicked), 0);
    gtk_signal_connect(GTK_OBJECT (numberOfHarmonicsLineEdit), "changed",
                     GTK_SIGNAL_FUNC(MainWindowon_numberOfHarmonicsLineEdit_textChanged), 0);
//    g->setData(data_, size_, max_);
}

void MainWindowon_toolButton_clicked(GtkWidget*a, void*data)
{

//    ContMaximaSpectrogram* s = dynamic_cast<ContMaximaSpectrogram*>(spectrogram);
//    resynthesizeMaxima(s, g->rangeStartInArray(), g->rangeEndInArray());
//    alsaPlayBlock(audioOutput.data(), audioOutput.size());

    double* shifted;
    int shiftedH;
    shiftandmulLinear(originalFourierTransform, originalFourierTransformH,
                      originalFourierTransformW, &shifted, &shiftedH);
    int hms;
    continuousHarmonic**  contharms = prepareHarmonicsStbArray(maxesLinear(shifted, shiftedH, originalFourierTransformW, inpi.samplerate), &hms);
    spectrogram = malloc(sizeof(ContMaximaSpectrogram));
    *spectrogram=contMaximaSpectrogramVtable;
    ((ContMaximaSpectrogram*)spectrogram)->maxima = contharms;//= new ContMaximaSpectrogram(max,
    ((ContMaximaSpectrogram*)spectrogram)->harmonics = hms;
    gtk_widget_queue_draw(widget);
    alsaPlayBlock(audioOutputStb, arrlen(audioOutputStb));
 }

void MainWindowon_denominator_textChanged(GtkWidget*d, void* c)
{
    double r;
    sscanf(c, "%lf", &r);
    denominator = r;
}

void MainWindowon_toolButton_2_clicked(GtkWidget*a, void*b)
{
    selectRange(widget);
}

void MainWindowon_pushButton_clicked(GtkWidget*a, void*b)
{
    const char* e = "/tmp/imsorrybutifwontdothisproperly.wav";
//    std::string filneavd = ;
    SF_INFO outi = inpi; outi.channels = 1;
    SNDFILE* out = sf_open(
                    e,
                    SFM_WRITE,
                    &outi);
    fprintf(stderr, "FILENAME IS %s", e);
    sf_write_short(out, audioOutputStb.data(), audioOutputStb.size());
    sf_close(out);

    char command[900] = "audacity \0";
    strcat(command, e);
    fprintf(stderr, "command IS %s", command);
    system(command);
}

void MainWindowon_numberOfHarmonicsLineEdit_textChanged(GtkWidget*f, void* e)
{
//    numberOfHarmonics = arg1.toInt();
    sscanf(s, "%d", &numberOfHarmonics );
}
