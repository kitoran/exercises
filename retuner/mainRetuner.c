//#include <stdio.h>

#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "midiprot.h"

#include <clap/clap.h>


#include <time.h>

//extern "C" {
static char const* feature [] = {CLAP_PLUGIN_FEATURE_NOTE_EFFECT, NULL};
static const clap_plugin_descriptor_t s_retune_plug_desc = {
    .clap_version = CLAP_VERSION_INIT,
    .id = "com.something-offensive.Retuner",
    .name = "Retuner",
    //   .vendor = "something-offensive",
    //   .url = "https://your-domain.com/your-plugin",
    //   .manual_url = "https://your-domain.com/your-plugin/manual",
    //   .support_url = "https://your-domain.com/support",
    //   .version = "1.4.2",
    .description = "Retune the notes to 16edo.",
    .features = feature,
};

typedef struct {
    clap_plugin_t                   plugin;
    const clap_host_t              *host;
    const clap_host_latency_t      *host_latency;
    const clap_host_log_t          *host_log;
    const clap_host_thread_check_t *host_thread_check;
    const clap_host_state_t        *host_state;

    uint32_t latency;
} retune_plug_t;

////////////////////////////
// clap_plugin_note_ports //
////////////////////////////

static uint32_t retune_plug_note_ports_count(const clap_plugin_t *plugin, bool is_input) {
    // We just declare 1 note input and output
    return 1;
}

static bool retune_plug_note_ports_get(const clap_plugin_t   *plugin,
                                   uint32_t               index,
                                   bool                   is_input,
                                   clap_note_port_info_t *info) {
    if (index > 0)
        return false;
    info->id = 0;
    snprintf(info->name, sizeof(info->name), "%s", "My Port Name");
    info->supported_dialects =
            CLAP_NOTE_DIALECT_CLAP | CLAP_NOTE_DIALECT_MIDI | CLAP_NOTE_DIALECT_MIDI_MPE | CLAP_NOTE_DIALECT_MIDI2;
    info->preferred_dialect = CLAP_NOTE_DIALECT_MIDI;
    return true;
}

static const clap_plugin_note_ports_t s_retune_plug_note_ports = {
    .count = retune_plug_note_ports_count,
    .get = retune_plug_note_ports_get,
};

//////////////////
// clap_latency //
//////////////////

static uint32_t retune_plug_latency_get(const clap_plugin_t *plugin) {
    const retune_plug_t *plug = (const retune_plug_t *)plugin->plugin_data;
    return plug->latency;
}

static const clap_plugin_latency_t s_retune_plug_latency = {
    .get = retune_plug_latency_get,
};

////////////////
// clap_state //
////////////////

static bool retune_plug_state_save(const clap_plugin_t *plugin, const clap_ostream_t *stream) {
    const retune_plug_t *plug = (const retune_plug_t *)plugin->plugin_data;
    // TODO: write the state into stream
    return true;
}

static bool retune_plug_state_load(const clap_plugin_t *plugin, const clap_istream_t *stream) {
    const retune_plug_t *plug = (const retune_plug_t *)plugin->plugin_data;
    // TODO: read the state from stream
    return true;
}

static const clap_plugin_state_t s_retune_plug_state = {
    .save = retune_plug_state_save,
    .load = retune_plug_state_load,
};

/////////////////
// clap_plugin //
/////////////////

static bool retune_plug_init(const struct clap_plugin *plugin) {
    retune_plug_t *plug = /*const_cast< retune_plug_t *>*/((retune_plug_t *)plugin->plugin_data);

//    fprintf(stdout, "printing to stdour\n");
//    fprintf(stderr, "printing to stderr\n");
    // Fetch host's extensions here
    // Make sure to check that the interface functions are not null pointers
    plug->host_log = (const clap_host_log_t *)plug->host->get_extension(plug->host, CLAP_EXT_LOG);
    plug->host_thread_check = (const clap_host_thread_check_t *)plug->host->get_extension(plug->host, CLAP_EXT_THREAD_CHECK);
    plug->host_latency = (const clap_host_latency_t *)plug->host->get_extension(plug->host, CLAP_EXT_LATENCY);
    plug->host_state = (const clap_host_state_t *)plug->host->get_extension(plug->host, CLAP_EXT_STATE);
    return true;
}

static void retune_plug_destroy(const struct clap_plugin *plugin) {
    retune_plug_t *plug = ( retune_plug_t *)plugin->plugin_data;
    free(plug);
}

static bool retune_plug_activate(const struct clap_plugin *plugin,
                             double                    sample_rate,
                             uint32_t                  min_frames_count,
                             uint32_t                  max_frames_count) {
    return true;
}

static void retune_plug_deactivate(const struct clap_plugin *plugin) {}

static bool retune_plug_start_processing(const struct clap_plugin *plugin) { return true; }

static void retune_plug_stop_processing(const struct clap_plugin *plugin) {}

static void retune_plug_reset(const struct clap_plugin *plugin) {}

static void retune_plug_process_event(retune_plug_t *plug, const clap_event_header_t *hdr) {
    if (hdr->space_id == CLAP_CORE_EVENT_SPACE_ID) {
        switch (hdr->type) {
        case CLAP_EVENT_MIDI: {
            fprintf(stderr, "CLAP_EVENT_MIDI\n");
            const clap_event_midi_t *ev = (const clap_event_midi_t *)hdr;
            int channel = msg[0] & MIDI_CHANNEL_MASK;
            if((msg[0] & MIDI_COMMAND_MASK) == pitchWheelEvent) {
               i16 pitchWheel =
                       (msg[2] << 7) |
                       (msg[1]&MIDI_7HB_MASK);
               double differenceInTones =
                       double(pitchWheel-0x2000)/0x2000 * currentItemConfig->value.pitchRange / 2.0;
               double ratio = pow(2, differenceInTones/6);
               channelPitches[channel] = ratio;
            }
            //TODO: delete selected notes on "delete"
            //TODO: store velocity value too
//          res = MIDI_GetNote(take, i++, 0, 0, , &endppqpos, 0, &pitch, &vel);

            // TODO: indicate when we changed the take
    // we think that all the simultaneous notes are on different channels
    // so to get note's key we only need to read it from onteOff event
            double pos = MIDI_GetProjTimeFromPPQPos(take, ppqpos);
            if((msg[0] & MIDI_COMMAND_MASK) == noteOn) {
                channelNoteStarts[channel] = pos;
            }
            if((msg[0] & MIDI_COMMAND_MASK) == noteOff) {
                int key = msg[1];
                int vel = msg[2];
                double freq = (440.0 / 32) * pow(2, ((key - 9) / 12.0));
                freq *= channelPitches[channel];
                message("st %lf end %lf pitch %d vel %d\n"
                        "start %lf  freq %lf", pos, channelNoteStarts[channel] , key, vel
                        , start, freq);
                appendRealNote({.note = {.freq = freq,
                                         .start = channelNoteStarts[channel]  - itemStart,
                                         .length = pos-channelNoteStarts[channel]},
                                .midiChannel = channel});
            }

            volatile int rer = 42;
            break;
        }

        case CLAP_EVENT_MIDI_SYSEX: {
            const clap_event_midi_sysex_t *ev = (const clap_event_midi_sysex_t *)hdr;
            // TODO: handle MIDI Sysex event
            break;
        }
        }
    }
}

static char message[3000] = {0};
static char* endOfMessage = message;
static clap_process_status retune_plug_process(const struct clap_plugin *plugin,
                                           const clap_process_t     *process) {
    const retune_plug_t *plug = (const retune_plug_t *)plugin->plugin_data;
    const uint32_t nframes = process->frames_count;
    const uint32_t nev = process->in_events->size(process->in_events);
    uint32_t       ev_index = 0;
    uint32_t       next_ev_frame = nev > 0 ? 0 : nframes;

    for (uint32_t i = 0; i < nframes;i++) {//TODO remove this loop
          /* handle every events that happrens at the frame "i" */
          while (ev_index < nev && next_ev_frame == i) {
             const clap_event_header_t *hdr = process->in_events->get(process->in_events, ev_index);

             if (hdr->time != i) {
                next_ev_frame = hdr->time;
                break;
             }
             volatile uint16_t sid = hdr->space_id;
             volatile uint16_t type = hdr->type;

             retune_plug_process_event(plug, hdr);
             ++ev_index;

             if (ev_index == nev) {
                // we reached the end of the event list
                next_ev_frame = nframes;
                break;
             }
          }
    }
    return CLAP_PROCESS_CONTINUE;
}

static const void *retune_plug_get_extension(const struct clap_plugin *plugin, const char *id) {
    if (!strcmp(id, CLAP_EXT_LATENCY))
        return &s_retune_plug_latency;
    if (!strcmp(id, CLAP_EXT_AUDIO_PORTS))
        return NULL;
    if (!strcmp(id, CLAP_EXT_NOTE_PORTS))
        return &s_retune_plug_note_ports;
    if (!strcmp(id, CLAP_EXT_STATE))
        return &s_retune_plug_state;
    // TODO: add support to CLAP_EXT_PARAMS
    return NULL;
}

static void retune_plug_on_main_thread(const struct clap_plugin *plugin) {}

static clap_plugin_t *retune_plug_create(const clap_host_t *host) {
    retune_plug_t *p = ( retune_plug_t *)calloc(1, sizeof(*p));
    p->host = host;
    p->plugin.desc = &s_retune_plug_desc;
    p->plugin.plugin_data = p;
    p->plugin.init = retune_plug_init;
    p->plugin.destroy = retune_plug_destroy;
    p->plugin.activate = retune_plug_activate;
    p->plugin.deactivate = retune_plug_deactivate;
    p->plugin.start_processing = retune_plug_start_processing;
    p->plugin.stop_processing = retune_plug_stop_processing;
    p->plugin.reset = retune_plug_reset;
    p->plugin.process = retune_plug_process;
    p->plugin.get_extension = retune_plug_get_extension;
    p->plugin.on_main_thread = retune_plug_on_main_thread;

    // Don't call into the host here

    return &p->plugin;
}

/////////////////////////
// clap_plugin_factory //
/////////////////////////

static struct {
    const clap_plugin_descriptor_t *desc;
    clap_plugin_t *(CLAP_ABI *create)(const clap_host_t *host);
} s_plugins[] = {
{
    .desc = &s_retune_plug_desc,
    .create = retune_plug_create,
},
};

static uint32_t plugin_factory_get_plugin_count(const struct clap_plugin_factory *factory) {
    return sizeof(s_plugins) / sizeof(s_plugins[0]);
}

static const clap_plugin_descriptor_t *
plugin_factory_get_plugin_descriptor(const struct clap_plugin_factory *factory, uint32_t index) {
    return s_plugins[index].desc;
}

static const clap_plugin_t *plugin_factory_create_plugin(const struct clap_plugin_factory *factory,
                                                         const clap_host_t                *host,
                                                         const char *plugin_id) {
    if (!clap_version_is_compatible(host->clap_version)) {
        return NULL;
    }

    const int N = sizeof(s_plugins) / sizeof(s_plugins[0]);
    for (int i = 0; i < N; ++i)
        if (!strcmp(plugin_id, s_plugins[i].desc->id))
            return s_plugins[i].create(host);

    return NULL;
}

static const clap_plugin_factory_t s_plugin_factory = {
    .get_plugin_count = plugin_factory_get_plugin_count,
    .get_plugin_descriptor = plugin_factory_get_plugin_descriptor,
    .create_plugin = plugin_factory_create_plugin,
};

////////////////
// clap_entry //
////////////////

static bool entry_init(const char *plugin_path) {
    // called only once, and very first
    return true;
}

static void entry_deinit(void) {
    // called before unloading the DSO
}

static const void *entry_get_factory(const char *factory_id) {
    if (!strcmp(factory_id, CLAP_PLUGIN_FACTORY_ID))
        return &s_plugin_factory;
    return NULL;
}

// This symbol will be resolved by the host
CLAP_EXPORT const clap_plugin_entry_t clap_entry = {
    .clap_version = CLAP_VERSION_INIT,
    .init = entry_init,
    .deinit = entry_deinit,
    .get_factory = entry_get_factory,
};
//}
