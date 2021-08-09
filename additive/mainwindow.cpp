#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"
#include "audio.h"
#include <QDebug>
#include "spectrogram.h"
#include "synthesis.h"
#include "globals.h"
#include <sndfile.h>
#include "newfile.h"

MainWindow::MainWindow(/*QWidget *parent,*/
/*                       double *data_,
                       int size_,
                       double max_*/) :
//    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    g=ui->widget;

//    g->setData(data_, size_, max_);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toolButton_clicked()
{

//    ContMaximaSpectrogram* s = dynamic_cast<ContMaximaSpectrogram*>(spectrogram);
//    resynthesizeMaxima(s, g->rangeStartInArray(), g->rangeEndInArray());
//    alsaPlayBlock(audioOutput.data(), audioOutput.size());

    double* shifted;
    int shiftedH;
    shiftandmulLinear(originalFourierTransform, originalFourierTransformH,
                      originalFourierTransformW, &shifted, &shiftedH);
    int hms;
    std::vector<std::vector<continuousHarmonic> >  contharms = prepareHarmonics(maxesLinear(shifted, shiftedH, originalFourierTransformW, inpi.samplerate), &hms);
    spectrogram = new ContMaximaSpectrogram(max,
            contharms, hms);
    g->repaint();
    alsaPlayBlock(audioOutput.data(), audioOutput.size());
 }

void MainWindow::on_denominator_textChanged(const QString &arg1)
{
    denominator = arg1.toDouble();
}

void MainWindow::on_toolButton_2_clicked()
{
    g->selectRange();
}

void MainWindow::on_pushButton_clicked()
{
    const char* e = "/tmp/imsorrybutifwontdothisproperly.wav";
//    std::string filneavd = ;
    SF_INFO outi = inpi; outi.channels = 1;
    SNDFILE* out = sf_open(
                    e,
                    SFM_WRITE,
                    &outi);
    qDebug() << "FILENAME IS" << e;
    sf_write_short(out, audioOutput.data(), audioOutput.size());
    sf_close(out);

    char command[900] = "audacity \0";
    strcat(command, e);
    qDebug() << "command IS" << command;
    system(command);
}

void MainWindow::on_numberOfHarmonicsLineEdit_textChanged(const QString &arg1)
{
    numberOfHarmonics = arg1.toInt();
}
