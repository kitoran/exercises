#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"
#include "audio.h"
#include "spectrogram.h"
#include "synthesis.h"
#include "globals.h"
#include <sndfile.h>
#include "newfile.h"
#include "graph.h"

void MainWindowon_toolButton_clicked()
{

//    ContMaximaSpectrogram* s = dynamic_cast<ContMaximaSpectrogram*>(spectrogram);
//    resynthesizeMaxima(s, g->rangeStartInArray(), g->rangeEndInArray());
//    alsaPlayBlock(audioOutput.data(), audioOutput.size());

    double* shifted;
    int shiftedH;
    shiftandmulLinear(originalFourierTransform, originalFourierTransformH,
                      originalFourierTransformW, &shifted, &shiftedH);
    int hms;
    continuousHarmonic**  contharms = prepareHarmonicsStbArray(maxesLinearStbArray(shifted, shiftedH, originalFourierTransformW, inpi.samplerate), &hms);
    spectrogram = malloc(sizeof(ContMaximaSpectrogram));
    *spectrogram=ContMaximaSpectrogramVtable;
    ((ContMaximaSpectrogram*)spectrogram)->maxima = contharms;//= new ContMaximaSpectrogram(max,
    ((ContMaximaSpectrogram*)spectrogram)->harmonics = hms;
    guiRedraw();
    alsaPlayBlock(audioOutputStb, arrlen(audioOutputStb));
 }

void MainWindowon_toolButton_2_clicked()
{
    selectRange(&widget);
}

void MainWindowon_pushButton_clicked()
{
    const char* e = "/tmp/imsorrybutifwontdothisproperly.wav";
//    std::string filneavd = ;
    SF_INFO outi = inpi; outi.channels = 1;
    SNDFILE* out = sf_open(
                    e,
                    SFM_WRITE,
                    &outi);
    fprintf(stderr, "FILENAME IS %s", e);
    sf_write_short(out, audioOutputStb, arrlen(audioOutputStb));
    sf_close(out);

    char command[900] = "audacity \0";
    strcat(command, e);
    fprintf(stderr, "command IS %s", command);
    system(command);
}
