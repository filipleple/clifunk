#include "canvas.h"
#include "visualization.h"

void draw_rect_from_level(float level, Canvas* canvas);
void draw_rects_from_fft(const float *fft_data, size_t fft_size, Canvas* canvas);

void draw_rect_from_level(float level, Canvas* canvas){
    // Draw a square at the center, 25% of terminal size
    float h_min = 0.02, h_max = 0.25;

    float h = level * (h_max - h_min);
    draw_rect(canvas, 0.375, 0.375, 0.25, h, '#');
}

void draw_rects_from_fft(const float *fft_data, size_t fft_size, Canvas* canvas) {
    // Calculate bin indices for each frequency range
    size_t bass_start = 0, bass_end = 7;       // Bass: 0 - 300 Hz
    size_t mid_start = bass_end, mid_end = 116; // Midrange: 300 - 5000 Hz
    size_t high_start = mid_end, high_end = 371; // High-End: 5000 - 16000 Hz

    // Calculate average intensity for each range
    float bass_intensity = 0.0f;
    for (size_t i = bass_start; i < bass_end; i++) {
        bass_intensity += fft_data[i];
    }
    bass_intensity /= (bass_end - bass_start);

    float mid_intensity = 0.0f;
    for (size_t i = mid_start; i < mid_end; i++) {
        mid_intensity += fft_data[i];
    }
    mid_intensity /= (mid_end - mid_start);

    float high_intensity = 0.0f;
    for (size_t i = high_start; i < high_end; i++) {
        high_intensity += fft_data[i];
    }
    high_intensity /= (high_end - high_start);

    // Normalize intensities to fit the visualization range
    float h_min = 0.02, h_max = 0.25;
    float bass_height = bass_intensity * (h_max - h_min) + h_min;
    float mid_height = mid_intensity * (h_max - h_min) + h_min;
    float high_height = high_intensity * (h_max - h_min) + h_min;

    // Draw rectangles representing bass, midrange, and high-end intensities
    draw_rect(canvas, 0.25, 0.1, 0.125, bass_height, '#');  // Left: Bass
    draw_rect(canvas, 0.4375, 0.1, 0.125, mid_height, '#'); // Center: Midrange
    draw_rect(canvas, 0.625, 0.1, 0.125, high_height, '#'); // Right: High-End
}

void calculate_and_draw(float level, const float *fft_data, size_t fft_size, Canvas *canvas){
    //draw_rect_from_level(level, canvas);
    draw_rects_from_fft(fft_data, fft_size, canvas);
}
