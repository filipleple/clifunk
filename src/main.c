#include <pulse/simple.h>
#include <pulse/error.h>
#include <stdio.h>
#include <math.h>

#define BUFSIZE 1024

pa_buffer_attr buffer_attr = {
    .maxlength = (uint32_t)-1,           // Default maximum length
    .tlength   = (uint32_t)-1,           // Default target length for playback
    .prebuf    = (uint32_t)-1,           // Default pre-buffering for playback
    .minreq    = (uint32_t)-1,           // Default minimum request
    .fragsize  = (uint32_t)(1024),       // Smaller fragment size (smaller buffer)
};

int main() {
    pa_simple *s = NULL;
    int error;

    // Create a new PulseAudio recording stream
    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 44100,
        .channels = 1
    };

    if (!(s = pa_simple_new(NULL, "Visualizer", PA_STREAM_RECORD, NULL, "Audio Stream", &ss, NULL, &buffer_attr, &error))) {
        fprintf(stderr, "pa_simple_new() failed: %s\n", pa_strerror(error));
        return 1;
    }

    uint8_t buf[BUFSIZE];
    while (1) {
        // Read audio data
        if (pa_simple_read(s, buf, sizeof(buf), &error) < 0) {
            fprintf(stderr, "pa_simple_read() failed: %s\n", pa_strerror(error));
            break;
        }

        // Process audio (e.g., calculate RMS or FFT)
        double rms = 0.0;
        for (int i = 0; i < BUFSIZE; i += 2) {
            int16_t sample = buf[i] | (buf[i+1] << 8);
            rms += sample * sample;
        }
        rms = sqrt(rms / (BUFSIZE / 2));

        // Print RMS value as a visual bar
        int bars = (int)(rms / 1000);
        printf("\r");
        for (int i = 0; i < bars; ++i) printf("#");
        fflush(stdout);
    }

    // Cleanup
    if (s) pa_simple_free(s);
    return 0;
}

