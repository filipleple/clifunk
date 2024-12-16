#include "canvas.h"
#include "visualization.h"
#include <math.h>

void draw_rect_from_level(float level, Canvas* canvas);
void draw_rects_from_fft(const float *fft_data, size_t fft_size, Canvas* canvas);

void draw_rect_from_level(float level, Canvas* canvas){
    // Draw a square at the center, 25% of terminal size
    float h_min = 0.02, h_max = 0.25;

    float h = level * (h_max - h_min);
    draw_rect(canvas, 0.375, 0.375, 0.25, h, '#');
}

#include <math.h>

void draw_rects_from_fft(const float *fft_data, size_t fft_size, Canvas* canvas) {
    // Define frequency ranges
    float freq_per_bin = SAMPLE_RATE / (float)FFT_SIZE;
    size_t bass_end = (size_t)(300 / freq_per_bin);
    size_t mid_start = bass_end, mid_end = (size_t)(5000 / freq_per_bin);
    size_t high_start = mid_end, high_end = (size_t)(16000 / freq_per_bin);

    // Calculate log-scaled intensity for each range
    float bass_intensity = 0.0f;
    for (size_t i = 0; i < bass_end; i++) {
        bass_intensity += log1p(fft_data[i]); // Logarithmic scale
    }
    bass_intensity /= (bass_end - 0);

    float mid_intensity = 0.0f;
    for (size_t i = mid_start; i < mid_end; i++) {
        mid_intensity += log1p(fft_data[i]);
    }
    mid_intensity /= (mid_end - mid_start);

    float high_intensity = 0.0f;
    for (size_t i = high_start; i < high_end; i++) {
        high_intensity += log1p(fft_data[i]);
    }
    high_intensity /= (high_end - high_start);

    // Apply weighting to balance the bands visually
    const float bass_weight = 0.5f;  // Reduce bass impact
    const float mid_weight = 1.0f;   // Leave mids as-is
    const float high_weight = 2.0f;  // Boost highs
    bass_intensity *= bass_weight;
    mid_intensity *= mid_weight;
    high_intensity *= high_weight;

    // Find the maximum intensity across all bands
    float max_intensity = fmaxf(fmaxf(bass_intensity, mid_intensity), high_intensity);

    // Scale each band to the visualization range
    float h_max = 0.5;  // Maximum bar height
    float h_min = 0.02; // Minimum bar height
    if (max_intensity > 0) {
        bass_intensity = (bass_intensity / max_intensity) * (h_max - h_min) + h_min;
        mid_intensity = (mid_intensity / max_intensity) * (h_max - h_min) + h_min;
        high_intensity = (high_intensity / max_intensity) * (h_max - h_min) + h_min;
    } else {
        bass_intensity = h_min;
        mid_intensity = h_min;
        high_intensity = h_min;
    }

    // Draw rectangles for each frequency range
    draw_rect(canvas, 0.25, 0.1, 0.125, bass_intensity, '#');  // Left: Bass
    draw_rect(canvas, 0.4375, 0.1, 0.125, mid_intensity, '#'); // Center: Midrange
    draw_rect(canvas, 0.625, 0.1, 0.125, high_intensity, '#'); // Right: High-End
}

void calculate_and_draw(float level, const float *fft_data, size_t fft_size, Canvas *canvas){
    //draw_rect_from_level(level, canvas);
    draw_rects_from_fft(fft_data, fft_size, canvas);
}
