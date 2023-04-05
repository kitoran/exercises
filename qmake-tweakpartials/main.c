//#include <stdio.h>

#include "fft.h"

#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include <clap/clap.h>


#include <time.h>

//extern "C" {
static char const* feature [] = {CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, CLAP_PLUGIN_FEATURE_MONO, NULL};
static const clap_plugin_descriptor_t s_my_plug_desc = {
    .clap_version = CLAP_VERSION_INIT,
    .id = "com.something-offensive.TweakPartials",
    .name = "Tweak Partials",
    //   .vendor = "something-offensive",
    //   .url = "https://your-domain.com/your-plugin",
    //   .manual_url = "https://your-domain.com/your-plugin/manual",
    //   .support_url = "https://your-domain.com/support",
    //   .version = "1.4.2",
    .description = "Tweak Partials.",
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
} my_plug_t;

/////////////////////////////
// clap_plugin_audio_ports //
/////////////////////////////

static uint32_t my_plug_audio_ports_count(const clap_plugin_t *plugin, bool is_input) {
    // We just declare 1 audio input and 1 audio output
    return 1;
}

static bool my_plug_audio_ports_get(const clap_plugin_t    *plugin,
                                    uint32_t                index,
                                    bool                    is_input,
                                    clap_audio_port_info_t *info) {
    if (index > 0)
        return false;
    info->id = 0;
    snprintf(info->name, sizeof(info->name), "%s", "My Port Name");
    info->channel_count = 2;
    info->flags = CLAP_AUDIO_PORT_IS_MAIN;
    info->port_type = CLAP_PORT_STEREO;
    info->in_place_pair = CLAP_INVALID_ID;
    return true;
}

static const clap_plugin_audio_ports_t s_my_plug_audio_ports = {
    .count = my_plug_audio_ports_count,
    .get = my_plug_audio_ports_get,
};

////////////////////////////
// clap_plugin_note_ports //
////////////////////////////

static uint32_t my_plug_note_ports_count(const clap_plugin_t *plugin, bool is_input) {
    // We just declare 1 note input
    return 1;
}

static bool my_plug_note_ports_get(const clap_plugin_t   *plugin,
                                   uint32_t               index,
                                   bool                   is_input,
                                   clap_note_port_info_t *info) {
    if (index > 0)
        return false;
    info->id = 0;
    snprintf(info->name, sizeof(info->name), "%s", "My Port Name");
    info->supported_dialects =
            CLAP_NOTE_DIALECT_CLAP | CLAP_NOTE_DIALECT_MIDI_MPE | CLAP_NOTE_DIALECT_MIDI2;
    info->preferred_dialect = CLAP_NOTE_DIALECT_CLAP;
    return true;
}

static const clap_plugin_note_ports_t s_my_plug_note_ports = {
    .count = my_plug_note_ports_count,
    .get = my_plug_note_ports_get,
};

//////////////////
// clap_latency //
//////////////////

uint32_t my_plug_latency_get(const clap_plugin_t *plugin) {
    const my_plug_t *plug = (const my_plug_t *)plugin->plugin_data;
    return plug->latency;
}

static const clap_plugin_latency_t s_my_plug_latency = {
    .get = my_plug_latency_get,
};

////////////////
// clap_state //
////////////////

bool my_plug_state_save(const clap_plugin_t *plugin, const clap_ostream_t *stream) {
    const my_plug_t *plug = (const my_plug_t *)plugin->plugin_data;
    // TODO: write the state into stream
    return true;
}

bool my_plug_state_load(const clap_plugin_t *plugin, const clap_istream_t *stream) {
    const my_plug_t *plug = (const my_plug_t *)plugin->plugin_data;
    // TODO: read the state from stream
    return true;
}

static const clap_plugin_state_t s_my_plug_state = {
    .save = my_plug_state_save,
    .load = my_plug_state_load,
};

/////////////////
// clap_plugin //
/////////////////
static float (*output1);//[32768];// = {0};
static float (*output2);//[32768];// = {0};

static bool my_plug_init(const struct clap_plugin *plugin) {
    my_plug_t *plug = /*const_cast< my_plug_t *>*/((my_plug_t *)plugin->plugin_data);
//        makeBHWindows(WINDOW_SIZE);
    //        makeHammingWindows(WINDOW_SIZE);
            makeHannWindow(WINDOW_SIZE);
//        makeFlatWindows(WINDOW_SIZE);
    output1 = malloc(32768*sizeof(*output1));
    output2 = malloc(32768*sizeof(*output1));
    memset(output1, 0, 32768*sizeof(*output1));
    memset(output2, 0, 32768*sizeof(*output1));
    // Fetch host's extensions here
    // Make sure to check that the interface functions are not null pointers
    plug->host_log = (const clap_host_log_t *)plug->host->get_extension(plug->host, CLAP_EXT_LOG);
    plug->host_thread_check = (const clap_host_thread_check_t *)plug->host->get_extension(plug->host, CLAP_EXT_THREAD_CHECK);
    plug->host_latency = (const clap_host_latency_t *)plug->host->get_extension(plug->host, CLAP_EXT_LATENCY);
    plug->host_state = (const clap_host_state_t *)plug->host->get_extension(plug->host, CLAP_EXT_STATE);
    return true;
}

static void my_plug_destroy(const struct clap_plugin *plugin) {
    my_plug_t *plug = ( my_plug_t *)plugin->plugin_data;
    free(plug);
}

static bool my_plug_activate(const struct clap_plugin *plugin,
                             double                    sample_rate,
                             uint32_t                  min_frames_count,
                             uint32_t                  max_frames_count) {
    return true;
}

static void my_plug_deactivate(const struct clap_plugin *plugin) {}

static bool my_plug_start_processing(const struct clap_plugin *plugin) { return true; }

static void my_plug_stop_processing(const struct clap_plugin *plugin) {}

static void my_plug_reset(const struct clap_plugin *plugin) {}

static void my_plug_process_event(my_plug_t *plug, const clap_event_header_t *hdr) {
    if (hdr->space_id == CLAP_CORE_EVENT_SPACE_ID) {
        switch (hdr->type) {
        case CLAP_EVENT_NOTE_ON: {
            const clap_event_note_t *ev = (const clap_event_note_t *)hdr;
            // TODO: handle note on
            break;
        }

        case CLAP_EVENT_NOTE_OFF: {
            const clap_event_note_t *ev = (const clap_event_note_t *)hdr;
            // TODO: handle note off
            break;
        }

        case CLAP_EVENT_NOTE_CHOKE: {
            const clap_event_note_t *ev = (const clap_event_note_t *)hdr;
            // TODO: handle note choke
            break;
        }

        case CLAP_EVENT_NOTE_EXPRESSION: {
            const clap_event_note_expression_t *ev = (const clap_event_note_expression_t *)hdr;
            // TODO: handle note expression
            break;
        }

        case CLAP_EVENT_PARAM_VALUE: {
            const clap_event_param_value_t *ev = (const clap_event_param_value_t *)hdr;
            // TODO: handle parameter change
            break;
        }

        case CLAP_EVENT_PARAM_MOD: {
            const clap_event_param_mod_t *ev = (const clap_event_param_mod_t *)hdr;
            // TODO: handle parameter modulation
            break;
        }

        case CLAP_EVENT_TRANSPORT: {
            const clap_event_transport_t *ev = (const clap_event_transport_t *)hdr;
            // TODO: handle transport event
            break;
        }

        case CLAP_EVENT_MIDI: {
            const clap_event_midi_t *ev = (const clap_event_midi_t *)hdr;
            // TODO: handle MIDI event
            break;
        }

        case CLAP_EVENT_MIDI_SYSEX: {
            const clap_event_midi_sysex_t *ev = (const clap_event_midi_sysex_t *)hdr;
            // TODO: handle MIDI Sysex event
            break;
        }

        case CLAP_EVENT_MIDI2: {
            const clap_event_midi2_t *ev = (const clap_event_midi2_t *)hdr;
            // TODO: handle MIDI2 event
            break;
        }
        }
    }
}

char message[3000] = {0};
char* endOfMessage = message;
static float *buffer = NULL;

static float frames[32768] = {0};
//static float frames2[32768] = {0};
static myComplex fft[32768/2];
static int fullness = STEP_SIZE;
static clap_process_status my_plug_process(const struct clap_plugin *plugin,
                                           const clap_process_t     *process) {
    const my_plug_t *plug = (const my_plug_t *)plugin->plugin_data;
    const uint32_t nframes = process->frames_count;
    //   assert(nframes == WINDOW_SIZE);
    const uint32_t nev = process->in_events->size(process->in_events);
    uint32_t       ev_index = 0;
    uint32_t       next_ev_frame = nev > 0 ? 0 : nframes;
    assert(process->audio_outputs_count == 1);
    assert(process->audio_inputs_count == 1);



    //   Peak peaks[WINDOW_SIZE/2] = {{440, {200, 0}}};
    //   resynth(peaks, output );
    //   return CLAP_PROCESS_CONTINUE;

    //   for (uint32_t i = 0; i < nframes;) {
    /* handle every events that happrens at the frame "i" */
    /*while (ev_index < nev && next_ev_frame == i) {
         const clap_event_header_t *hdr = process->in_events->get(process->in_events, ev_index);
         if (hdr->time != i) {
            next_ev_frame = hdr->time;
            break;
         }

         my_plug_process_event(plug, hdr);
         ++ev_index;

         if (ev_index == nev) {
            // we reached the end of the event list
            next_ev_frame = nframes;
            break;
         }
      }*/
    //        for(int j = 0; j < process->audio_inputs[0].channel_count; j++) {

    int j = 0;
#define tau 6.2831853
    assert(nframes+fullness <= WINDOW_SIZE);
    assert(WINDOW_SIZE == STEP_SIZE*2);
    memcpy(frames+fullness, process->audio_inputs[0].data32[j], nframes*sizeof(float));
//    memcpy(process->audio_outputs[0].data32[j], output+fullness/*-WINDOW_SIZE/2+STEP_SIZE*/, nframes*sizeof(float));
    float* windowArray = window();
    float mm = windowArray[0];
//    bool undefined = true;
    static float prev = 0;
    for(int i = 0; i < nframes; i++) {
//        process->audio_outputs[0].data32[j][i] = (output1[i+fullness] * (windowArray[fullness+i]-mm) +
//                output2[fullness+i-STEP_SIZE] * (windowArray[fullness+i-STEP_SIZE]-mm))/(windowArray[fullness+i]+windowArray[fullness+i-STEP_SIZE]-2*mm);
        process->audio_outputs[0].data32[j][i] = (output1[i+fullness] * (windowArray[fullness+i]) +
                output2[fullness+i-STEP_SIZE] * (windowArray[fullness+i-STEP_SIZE]))/(windowArray[fullness+i]+windowArray[fullness+i-STEP_SIZE]);
//        if(!undefined) {
        if(fabs(process->audio_outputs[0].data32[j][i] - prev) > 0.01) {
            volatile int r = 42;
        }
        if(fabs(output1[i+fullness] - output1[i+fullness-1]) > 0.01) {
            volatile int w = 42;
        }
//        }
        prev = process->audio_outputs[0].data32[j][i];
//        undefined = false;
//        float x = (i+fullness)*1.0/STEP_SIZE;
//        float s = -2*x*x+3*x*x;
//        process->audio_outputs[0].data32[j][i] = (output1[i+fullness] * (1-s) +
//                output2[fullness+i-STEP_SIZE] * (s));
    }
    fullness += nframes;

    if(fullness >= WINDOW_SIZE) {//last indx = 4095
        clock_t start = clock();
        float* t = output1;
        output1 = output2;
        output2 = t;
        static double phase = 0;
        float fer = round(447 / SAMPLE_FREQ * WINDOW_SIZE)/WINDOW_SIZE*SAMPLE_FREQ;

        windowedFft(frames, WINDOW_SIZE, fft);
//        for(int i = 0; i < WINDOW_SIZE/4; i++) {
//            myComplex t = fft[i];
//            fft[i] = fft[WINDOW_SIZE/2 - 1 - i];
//            fft[WINDOW_SIZE/2 - 1 - i] = t;
//        }
        Peak peaks[WINDOW_SIZE/2] = {0};
        isolatePeaks(fft, peaks);
        static int mmm = 0; mmm = (mmm+1)%2;//(WINDOW_SIZE/STEP_SIZE);
//        resynth(peaks, output2, mmm);
//        memset(peaks, 0, sizeof(peaks));
//                peaks[0] = (Peak){250*(1+3*mmm), (myComplex){80, 0}};
//                peaks[1] = (Peak){320+300*mmm, (myComplex){80, 0}};

//                peaks[2] = (Peak){410+300*(1-mmm), (myComplex){80, 0}};
        //       alignPeaks(peaks);
        resynth(peaks, output2, mmm);
        //       float smsm[WINDOW_SIZE];
        //       windowedFft
        //       Peak peaks[WINDOW_SIZE/2] = {{fer, {20*cos(phase),20*sin(phase)}}};
        //       for(int i = 0; i < STEP_SIZE; i++) {
        //           output[i] = cos(phase)*0.2;// += fer /STEP_SIZE*tau;
        //           phase;//+= STEP_SIZE*fer /SAMPLE_FREQ*tau;

        //       }
        //       if(PH.
        //
        //       phase=0;
        //       memmove(output, output+STEP_SIZE, WINDOW_SIZE-STEP_SIZE);
        //       for(int i = WINDOW_SIZE-STEP_SIZE; i < WINDOW_SIZE; i++) {
        //           output[i] = cos(/*i*1.0*fre/SAMPLE_FREQ*tau+*/phase)/4;
        //       }
        fullness = WINDOW_SIZE-STEP_SIZE;
        clock_t end = clock();
        //        usle
        volatile double time_spent = (double)(end - start) / CLOCKS_PER_SEC*1000;
//        while(time_spent < 10) time_spent = (double)(end - start) / CLOCKS_PER_SEC*1000;
        return CLAP_PROCESS_CONTINUE;
        //        clock_t begin = clock();
        assert(fullness == WINDOW_SIZE);
        if(frames[1023] != 0) {
            volatile int r = 7;
        }
        //        ifft32(fft, output);
        //       static float phase = 0;
        //       float fre = round(440 / SAMPLE_FREQ.0 * WINDOW_SIZE)/WINDOW_SIZE*SAMPLE_FREQ.0;
        //       phase += fre*STEP_SIZE*tau/SAMPLE_FREQ;
        //       peaks[0]=(Peak){fre,{300*cos(phase),300*sin(phase)}};
        //       peaks[1]=(Peak){880,{200,0}};
        //       resynth(peaks, output  );
        //       static float phase = 0;
//        for(int i = 0; i < WINDOW_SIZE; i++) {
//            output[i] = cos(/*i*1.0*fre/SAMPLE_FREQ*tau+*/phase)/4;
//            phase += 440.0 /SAMPLE_FREQ * tau;
//        }

//        fullness = WINDOW_SIZE-STEP_SIZE;
//        memmove(frames, frames+STEP_SIZE, WINDOW_SIZE-STEP_SIZE);
//        memmove(output, output+STEP_SIZE, WINDOW_SIZE-STEP_SIZE);
        //        time_spent+=1;
    }
    //        }
    /* process every samples until the next event */
    /*for (; i < next_ev_frame; ++i) {
         // fetch input samples
         const float in_l = process->audio_inputs[0].data32[0][i];
         const float in_r = process->audio_inputs[0].data32[1][i];

         /* TODO: process samples, here we simply swap left and right channels * /
         const float out_l = in_r;
         const float out_r = in_l;

         // store output samples
         process->audio_outputs[0].data32[0][i] = out_l;
         process->audio_outputs[0].data32[1][i] = out_r;
      }*/
    //   }

    return CLAP_PROCESS_CONTINUE;
}

static const void *my_plug_get_extension(const struct clap_plugin *plugin, const char *id) {
    if (!strcmp(id, CLAP_EXT_LATENCY))
        return &s_my_plug_latency;
    if (!strcmp(id, CLAP_EXT_AUDIO_PORTS))
        return &s_my_plug_audio_ports;
    if (!strcmp(id, CLAP_EXT_NOTE_PORTS))
        return &s_my_plug_note_ports;
    if (!strcmp(id, CLAP_EXT_STATE))
        return &s_my_plug_state;
    // TODO: add support to CLAP_EXT_PARAMS
    return NULL;
}

static void my_plug_on_main_thread(const struct clap_plugin *plugin) {}

clap_plugin_t *my_plug_create(const clap_host_t *host) {
    my_plug_t *p = ( my_plug_t *)calloc(1, sizeof(*p));
    p->host = host;
    p->plugin.desc = &s_my_plug_desc;
    p->plugin.plugin_data = p;
    p->plugin.init = my_plug_init;
    p->plugin.destroy = my_plug_destroy;
    p->plugin.activate = my_plug_activate;
    p->plugin.deactivate = my_plug_deactivate;
    p->plugin.start_processing = my_plug_start_processing;
    p->plugin.stop_processing = my_plug_stop_processing;
    p->plugin.reset = my_plug_reset;
    p->plugin.process = my_plug_process;
    p->plugin.get_extension = my_plug_get_extension;
    p->plugin.on_main_thread = my_plug_on_main_thread;

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
    .desc = &s_my_plug_desc,
    .create = my_plug_create,
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
