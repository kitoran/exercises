#include "midi.h"

int main() {
    midi_open();

    while(true) {
//        time();
        snd_seq_event_t* ev = midi_read();
        if(ev) {
            fprintf(stderr, "%c %d\n", ev->data.note.note, (int)(ev->data.note.note));
            if(ev->type == SND_SEQ_EVENT_NOTEON) {
                frequency = 440 * pow(2, ((double)(ev->data.note.note-69))/12);
            }
        }
//        fprintf(stderr, ".");
//        int remainingSize = framesPerPeriod;
//        if(frameNumber + framesPerPeriod >= totalFrames) {
//            remainingSize = totalFrames - frameNumber;
//        }
        for(int j = 0; j < framesPerPeriod; j++, frameNumber++) {
//            buffer[j].l = 0;
            buffer[j].r = buffer[j].l = valueTable[sample%framesPerOscNormal];
            sample++;
//            phase += M_PI*2/sampleRate*frequency;
        }

        writeFrames(buffer, framesPerPeriod);
//        iter(valueTable, framesPerOscNormal);
    }
    drain();
//  free(buffer); // there is no need to free

  return 0;
}
