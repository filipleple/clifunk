// audio_engine.h

#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include <stddef.h>

// Initialize the audio engine. Returns 0 on success, nonzero on failure.
int audio_engine_init();

// Cleanup the audio engine.
void audio_engine_cleanup();

// Get the current audio level (e.g., RMS).
float audio_engine_get_level();

// Get the FFT data (caller provides buffer, returns number of bins filled).
size_t audio_engine_get_fft(float *fft_buffer, size_t buffer_size);

#endif // AUDIO_ENGINE_H
