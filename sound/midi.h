#ifndef MIDI_H
#define MIDI_H
#include <alsa/asoundlib.h>

void midi_open(void);
snd_seq_event_t *midi_read(void);
void subscribe();

#endif // MIDI_H
