// audio_engine_pulse.c
#include "audio_engine.h"
#include <pulse/simple.h>
#include <pulse/error.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "kiss_fftr.h"
#include "config.h"

pa_buffer_attr buffer_attr = {
    .maxlength = (uint32_t)-1,           // Default maximum length
    .tlength   = (uint32_t)-1,           // Default target length for playback
    .prebuf    = (uint32_t)-1,           // Default pre-buffering for playback
    .minreq    = (uint32_t)-1,           // Default minimum request
    .fragsize  = (uint32_t)(FRAGSIZE),       // Smaller fragment size (smaller buffer)
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

static kiss_fftr_cfg fft_cfg = NULL;
static kiss_fft_scalar input_buffer[FFT_SIZE];
static kiss_fft_cpx output_buffer[FFT_SIZE / 2 + 1];

// Get FFT data
size_t audio_engine_get_fft(float *output_buffer, size_t buffer_size) {
    if (!pulse_handle || buffer_size < FFT_SIZE / 2) {
        fprintf(stderr, "Invalid FFT configuration or buffer size.\n");
        return 0;
    }

    // Allocate FFT configuration if not already done
    if (!fft_cfg) {
        fft_cfg = kiss_fftr_alloc(FFT_SIZE, 0, NULL, NULL);
        if (!fft_cfg) {
            fprintf(stderr, "Failed to allocate FFT configuration.\n");
            return 0;
        }
    }

    float input_buffer[FFT_SIZE];
    kiss_fft_cpx freq_buffer[FFT_SIZE / 2 + 1];
    int error;

    // Read audio data into input_buffer
    if (pa_simple_read(pulse_handle, input_buffer, sizeof(input_buffer), &error) < 0) {
        fprintf(stderr, "PulseAudio read error: %s\n", pa_strerror(error));
        return 0;
    }

    // Perform FFT
    kiss_fftr(fft_cfg, input_buffer, freq_buffer);

    // Compute magnitudes from the complex output
    for (size_t i = 0; i < FFT_SIZE / 2; i++) {
        output_buffer[i] = sqrtf(freq_buffer[i].r * freq_buffer[i].r +
                                 freq_buffer[i].i * freq_buffer[i].i);
    }

    return FFT_SIZE / 2; // Return the size of the output buffer
}
