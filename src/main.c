#include "audio_engine.h"
#include <stdio.h>
#include <unistd.h>

#define SAMPLE_RATE 44100
#define FRAGSIZE 512

int main() {
    float sleep_duration_sec = (float)FRAGSIZE / SAMPLE_RATE; // Duration of one buffer
    int sleep_duration_us = (int)(sleep_duration_sec * 1e6);  // Convert to microseconds

    if (audio_engine_init() != 0) {
        fprintf(stderr, "Failed to initialize audio engine.\n");
        return 1;
    }

    // Visualizer loop
    while (1) {
        float level = audio_engine_get_level();
        printf("Audio Level: %f\n", level);

        float fft_data[1024];
        size_t fft_size = audio_engine_get_fft(fft_data, 1024);

        // Visualization logic here...

        usleep(sleep_duration_us); // Adjust as needed
    }

    audio_engine_cleanup();
    return 0;
}

