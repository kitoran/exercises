#include "spectrogram.h"
#include "globals.h"
#include "mathext.h"
#include "audio.h"
#include <QPainter>
#include <QDebug>

//draw() {
//p.setPen(Qt::NoPen);
//double cutoff = 10000;//samplerate;//10000.0;//*heights/samplerate;
//double cutoffind = mode == spectrogram_mode::linear?(cutoff*heights/44100)
//            :log(cutoff/freqMin)/log(frequencyMultiplent);//samplerate;

//for(int i = 0; i < widths; i++) {
//    for(int j = 0; j < cutoffind; j++) {
//        double left = (width()-20)/double(widths)*i;
//        double right = (width()-20)/double(widths)*(i+1);
//        double bottom = (height()-20)/double(cutoffind)*j;
//        double top = (height()-20)/double(cutoffind)*(j+1);
//        double value = complex?std::abs(data.c(i*heights+j))
//                             :data.d(i*heights+j);
//        int g = value/max*255;
//        p.setBrush(QColor(g,g,g));
//        if(g < 0 || g > 255) {
//            qDebug() << "i" << i << "j" << j << "amplitude" << value // data.c(i*heights+j)
//                     << "max" << max <<
//                        "color" << g;
//        }
//        p.drawRect(QRectF(left+10, height()-10-top, right-left, top-bottom));
//    }
//}
//}

void Spectrogram::drawAxes(QPainter *p, int w, int h) {
    p->setPen(Qt::darkMagenta);
//    double sortabase = log(freqMax/freqMin)/(height()-20);
    for(int y = 0; y < h-20; y += 100) {
        double freq;
//        if(mode == spectrogram_mode::logarithmic) {
//             freq = 30 * exp(sortabase*y);
//        } else {
//             freq = double(y) / (height()-20) * cutoff;
//        }
        freq = frequencyAtProportion(double(y) / (h-20));
        p->drawLine(QPointF{10., h-10.-y},
                   QPointF{w-10., h-10.-y});
        p->drawText(QPointF{10., h-10.-y},
                   QString::number(freq, ' ', 2));
    }
}




//prepareFrequenciesAndIntSpectrum(){
//logWindowSize = intLog2(spectrumSize);
//int size = spectr.data.sized;
//if(spectr.complex) size /= 2;
//if(size > frequencies.size() && spectr.mode == spectrogram_mode::logarithmic) {
//    int i = frequencies.size();
//    for(; i < size; i++) {
//        double mul = pow(frequencyMultiplent, i);
//        double prod = freqMin * mul;
//        frequencies.push_back(prod);
//    }
//}
//spectrumInt.resize(spectrumSize);
//for(int i = 0; i < spectrumSize; i++) {
//    spectrumInt[i] = spectr.data.d(i)*(INT16_MAX/16)/max;
//}
//}


//fillBuffer() {

//    if(spectr.mode == spectrogram_mode::logarithmic) {
//        for(int j = 0; j < framesPerPeriod; j++) {
//            int v = 0;
//            for(int i = 0; i < spectrumSize; i++) {
//                v += spectrumInt[i]*lookup[int((frequencies[i]/alsaSampleRate
//                                        *phase+i)*LOOKUP_TABLE_SIZE)%LOOKUP_TABLE_SIZE];
//            }
//            phase++;
//            buffer[j] = v/INT16_MAX;
//        }
//    }  else  {

//        for(int j = 0; j < framesPerPeriod; j++) {
//            int v = 0;
////                    static float r[] = {220, 261.6, 329.6};
//            for(int i = 0; i < 3/*spectrumSize*/; i++) {
//                v += /*spectrumInt*/r[i]*lookup[int((1.0/spectrumSize*i
//                                        *phase+i)*LOOKUP_TABLE_SIZE)%LOOKUP_TABLE_SIZE]/*/max*3*/;
//            }
//            phase++;
//            buffer[j] = v/*/*400*/   /INT16_MAX;



//        }
////                int written = 0;
////                while(spectr.windowSize > 0 && written < framesPerPeriod) {
////                    int amount = std::min(framesPerPeriod-written,
////                                          spectr.windowSize);
////                    memcpy(buffer, &unFft[unfftIndex], amount);
////                    for(int i = 0; i < amount; i++) {
////                        buffer[i] = unFft[unfftIndex + i];
////                    }
////                    written+=amount;
////                    unfftIndex += amount;
////                    unfftIndex &= (spectr.windowSize-1);
////                }
//    } /**/

//}

void MaximaSpectrogram::draw(QPainter *p, int w, int h)
{
    p->setPen(Qt::NoPen);
    p->setBrush(Qt::black);
    p->drawRect(10, 10, w-20, h-20);
    int spectrWidth = maxima.size();
    for(int i = 0; i < spectrWidth; i++) {
        for(int j = 0; j < maxima[i].size(); j++) {
            harmonic har = maxima[i][j];
            double left = (w-20)/double(spectrWidth)*i;
            double right = (w-20)/double(spectrWidth)*(i+1);
            double bottom = (h-20)/double(cutoff)*har.freq;
            double top = bottom+1;/*(h-20)/double(cutoff)*(j+1);*/
            double value = maxima[i][j].amp;
            int g = value/max*255;
//            if(g > 100) {
//                qDebug() << g;
//            }
            p->setBrush(QColor(g,g,g));
            if(g < 0 || g > 255) {
                qDebug() << "i" << i << "j" << j << "amplitude" << value // data.c(i*heights+j)
                         << "max" << max <<
                            "color" << g;
            }
            p->drawRect(QRectF(left+10, h-10-top, right-left, top-bottom));
        }
    }
    drawAxes(p,w,h);
}

void MaximaSpectrogram::fillBuffer(int16_t *buffer, int bufferSize, int pos, unsigned int phase)
{
    harmonic* hs = maxima[pos].data();
    int size = maxima[pos].size();
//    double ph  = phase;
    for(int j = 0; j < bufferSize; j++) {
        double v = 0;
        for(int i = 0; i < size; i++) {
            auto deb32 = hs[i];
            int64_t deb = ((hs[i].freq
                        *phase+i)*LOOKUP_TABLE_SIZE);
            int64_t deb15 = int64_t((hs[i].freq
                        *phase+i)*LOOKUP_TABLE_SIZE)%LOOKUP_TABLE_SIZE;
            int64_t deb2 = lookup[int64_t((hs[i].freq
                                   *phase+i)*LOOKUP_TABLE_SIZE)
                   %LOOKUP_TABLE_SIZE];
            double deb3 = hs[i].amp;
            v += hs[i].amp*lookup[int64_t((hs[i].freq
                                    *phase/alsaSampleRate+i)*LOOKUP_TABLE_SIZE)
                    %LOOKUP_TABLE_SIZE]/max/*3*/;
        }
        phase++;
        buffer[j] = v/10/*/*400   /INT16_MAX*/;
    }
}

int* MaximaSpectrogram::lookup = sinLookupTableInt();

double MaximaSpectrogram::frequencyAtProportion(double proportion)
{
    return proportion*cutoff;
}
