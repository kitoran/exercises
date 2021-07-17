#ifndef SOUNDEXT_H
#define SOUNDEXT_H
//#include <alsa/asoundlib.h>
#include <inttypes.h>

int freqToFftIndex(double freq, int sampleRate);
void alsaPlayBlock(int16_t *buffer, int size);

//template<typename T>
//struct frameType {
//    using type = T;
//    static const int alsaType = SND_PCM_FORMAT_UNKNOWN;
//    static const int  = SND_PCM_FORMAT_UNKNOWN;
//};

//template<>
//struct frameType<double>
//{
//    static const int alsaType = SND_PCM_FORMAT_FLOAT64_LE;
//};

//template<>
//struct frameType<double>
//{
//    static const int alsaType = SND_PCM_FORMAT_S16_LE;
//};




#endif // SOUNDEXT_H
