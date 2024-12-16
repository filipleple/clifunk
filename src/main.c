#include "audio_engine.h"
#include "canvas.h"
#include "visualization.h"
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

    Canvas* canvas = create_canvas();
    canvas_init();

    float level = 0;

    // Visualizer loop
    while (1) {
        int ch = getch();
        if (ch == 'q') break;

        // Update canvas size if terminal is resized
        update_canvas_size(canvas);
        clear_canvas(canvas);

        // Get current sound level from audio engine
        level = audio_engine_get_level();

        // Calculate the visualization based on current level
        calculate_and_draw(level, canvas);

        render_canvas(canvas);
        usleep(sleep_duration_us);
    }

    destroy_canvas(canvas);
    audio_engine_cleanup();
    return 0;
}

