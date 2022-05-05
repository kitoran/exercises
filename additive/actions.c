#include "actions.h"
#include "ui.h"
#include "math.h"
#include "audio.h"
#include "spectrogram.h"
#include "synthesis.h"
#include "globals.h"
#include <sndfile.h>
#include "newFile.h"
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

void calculateSpectrogram()
{
    if(originalFourierTransform) {
        free(originalFourierTransform);
    }
    stfft(samplsStbArray, arrlen(samplsStbArray),
          windowSize, stepSize, /*inpi.samplerate, */
          &originalFourierTransform, /*&h, */
          &originalFourierTransformW);
    originalFourierTransformH = windowSize;
    fprintf(stderr, "%d, %d, %lu, %lu", originalFourierTransformW,
                    originalFourierTransformH,
            sizeof(*originalFourierTransform),
            (originalFourierTransformW)*(originalFourierTransformH)*sizeof(*originalFourierTransform));
    harmonic** msstb = maxesLinearStbArray(originalFourierTransform, originalFourierTransformH,
                                           originalFourierTransformW, alsaSampleRate);

    free(spectrogram);
    struct MaximaSpectrogram mspectrogram = {
        MaximaSpectrogramVtable,
        msstb,
        max};


    spectrogram = malloc(sizeof(MaximaSpectrogram));
    *((MaximaSpectrogram *)spectrogram) = mspectrogram;
}

void openFile(char *path)
{

    inpi.format = 0;
    SNDFILE* inp = sf_open(path, SFM_READ, &inpi);


    if(inpi.channels  > 2) {
        fprintf(stderr, "%d channels ! ! !", inpi.channels);
        exit(543);
    }

    sf_count_t end = sf_seek(inp, 0, SEEK_END);
    arrsetlen(samplsStbArray, end);
//    sf_seek(inp, 0, SEEK_SET);
//    for(int i = 0; i < end; i++) {
//        int16_t sam[2];
//        sf_read_short(&inpi, sam, 1);
//        samplsStbArray[i] = sam[0];
//    }

//    fprintf(stderr, "%ld", end);
    sf_seek(inp, 0, SEEK_SET);
//    int16_t
    sf_read_short(inp, samplsStbArray, end);
    fprintf(stderr, "end of openFile %ld\n", end);

}
