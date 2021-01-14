#include <alsa/asoundlib.h>

static snd_seq_t *seq_handle;
static int in_port;

void midi_open(void)
{
    int err = snd_seq_open(&seq_handle, "default", SND_SEQ_OPEN_INPUT, 0);
    fprintf(stderr, "%d\n", err);
    snd_seq_nonblock(seq_handle, true);
    err = snd_seq_set_client_name(seq_handle, "Midi Listener");
    fprintf(stderr, "%d\n", err);
    in_port = snd_seq_create_simple_port(seq_handle, "listen:in",
                      SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
                      SND_SEQ_PORT_TYPE_APPLICATION);
    fprintf(stderr, "%d\n", err);
 }
void subscribe() {
//    snd_seq_port_subscribe_t *portSub = 0;
//    snd_seq_addr_t src{129,0};
//    snd_seq_addr_t dest{128,0};
//    int err = snd_seq_port_subscribe_malloc(&portSub);
//    fprintf(stderr, "snd_seq_port_subscribe_malloc: %d\n", err);
//    snd_seq_port_subscribe_set_dest(portSub, &dest);
////    fprintf(stderr, "snd_seq_port_subscribe_set_dest: %d\n", err);
//    snd_seq_port_subscribe_set_sender(portSub, &src);
////    fprintf(stderr, "snd_seq_port_subscribe_set_sender: %d\n", err);
//    snd_seq_port_subscribe_set_queue(portSub, 0);
//    snd_seq_port_subscribe_set_exclusive(portSub, 0);
//    snd_seq_port_subscribe_set_time_update(portSub, 0);
//    snd_seq_port_subscribe_set_time_real(portSub, 0);

//    err = snd_seq_subscribe_port(seq_handle, portSub);
//    fprintf(stderr, "snd_seq_subscribe_port %d\n", err);

    system("aconnect 128 129");
}
snd_seq_event_t *midi_read()
{
    snd_seq_event_t *ev = NULL;
    snd_seq_event_input(seq_handle, &ev);
    return ev;
}
