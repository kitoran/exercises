#include <stdio.h>
#include "audio.h"
#include "extmath.h"
//#include "midi.h"
//#include "gui.h"
int main()
{
    int16_t *buffer;

    initAudio(1, SND_PCM_FORMAT_S16_LE);
    buffer = (int16_t *) malloc(sizeof(int16_t)*framesPerBuffer);



//    while
    writeFrames(buffer, framesPerBuffer);
    printf("Hello World!\n");
    return 0;
}
