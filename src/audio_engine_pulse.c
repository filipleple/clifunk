// audio_engine_pulse.c
#include "audio_engine.h"
#include <pulse/simple.h>
#include <pulse/error.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SAMPLE_RATE 44100
#define FRAGSIZE 512
#define FFT_SIZE 1024

#define BUFSIZE 1024

pa_buffer_attr buffer_attr = {
    .maxlength = (uint32_t)-1,           // Default maximum length
    .tlength   = (uint32_t)-1,           // Default target length for playback
    .prebuf    = (uint32_t)-1,           // Default pre-buffering for playback
    .minreq    = (uint32_t)-1,           // Default minimum request
    .fragsize  = (uint32_t)(1024),       // Smaller fragment size (smaller buffer)
};

static pa_simple *pulse_handle = NULL;
static float fft_buffer[FFT_SIZE];

// Initialize PulseAudio
int audio_engine_init() {
    pa_sample_spec sample_spec = {
        .format = PA_SAMPLE_FLOAT32LE,
        .rate = SAMPLE_RATE,
        .channels = 1
    };
    int error;

    pulse_handle = pa_simple_new(
        NULL,
        "Visualizer",
        PA_STREAM_RECORD,
        "alsa_output.pci-0000_00_1f.3.analog-stereo.monitor",
        "Audio",
        &sample_spec,
        NULL,
        &buffer_attr,
        &error
    );

    if (!pulse_handle) {
        fprintf(stderr, "PulseAudio error: %s\n", pa_strerror(error));
        return -1;
    }
    return 0;
}

// Cleanup PulseAudio
void audio_engine_cleanup() {
    if (pulse_handle) {
        pa_simple_free(pulse_handle);
        pulse_handle = NULL;
    }
}

// Get current audio level
float audio_engine_get_level() {
    if (!pulse_handle) return 0.0f;

    float buffer[BUFSIZE];
    int error;

    if (pa_simple_read(pulse_handle, buffer, sizeof(buffer), &error) < 0) {
        fprintf(stderr, "PulseAudio read error: %s\n", pa_strerror(error));
        return 0.0f;
    }

    float rms = 0.0f;
    for (size_t i = 0; i < BUFSIZE; i++) {
        rms += buffer[i] * buffer[i];
    }
    return sqrtf(rms /  (BUFSIZE / 2));
}

// Get FFT data
size_t audio_engine_get_fft(float *output_buffer, size_t buffer_size) {
    if (!pulse_handle || buffer_size < FFT_SIZE) return 0;

    // Simulated FFT for simplicity (replace with real FFT library).
    memcpy(output_buffer, fft_buffer, sizeof(float) * FFT_SIZE);
    return FFT_SIZE;
}

