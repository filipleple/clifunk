#include "canvas.h"
#include "visualization.h"
#include <math.h>

void draw_rect_from_level(float level, Canvas* canvas);
void draw_rects_from_fft(const float *fft_data, size_t fft_size, Canvas* canvas);
void calculate_logarithmic_ranges(size_t *ranges, size_t num_bands, float freq_per_bin, float start_freq, float end_freq);

void draw_rect_from_level(float level, Canvas* canvas){
    // Draw a square at the center, 25% of terminal size
    float h_min = 0.02, h_max = 0.25;

    float h = level * (h_max - h_min);
    draw_rect(canvas, 0.375, 0.375, 0.25, h, '#');
}

// Function to calculate logarithmic frequency ranges
void calculate_logarithmic_ranges(size_t *ranges, size_t num_bands, float freq_per_bin, float start_freq, float end_freq) {
    for (size_t i = 0; i <= num_bands; i++) {
        float freq = start_freq * powf((end_freq / start_freq), i / (float)num_bands);
        ranges[i] = (size_t)(freq / freq_per_bin);
    }
}

void draw_rects_from_fft(const float *fft_data, size_t fft_size, Canvas* canvas) {
    // Constants for frequency range
    float start_freq = 20.0f;    // Starting frequency (20 Hz)
    float end_freq = 16000.0f;   // Ending frequency (16 kHz)
    size_t num_bands = 6;        // Number of frequency bands
    float freq_per_bin = SAMPLE_RATE / (float)FFT_SIZE;

    // Calculate logarithmic frequency ranges
    size_t ranges[7]; // num_bands + 1 for start and end
    calculate_logarithmic_ranges(ranges, num_bands, freq_per_bin, start_freq, end_freq);

    // Calculate intensities for each band
    float intensities[6] = {0.0f};
    for (size_t band = 0; band < num_bands; band++) {
        for (size_t i = ranges[band]; i < ranges[band + 1] && i < fft_size; i++) {
            intensities[band] += log1p(fft_data[i]); // Logarithmic scaling
        }
        if (ranges[band + 1] > ranges[band]) { // Avoid division by zero
            intensities[band] /= (ranges[band + 1] - ranges[band]);
        }
    }

    // Apply weighting to balance the bands visually
    const float weights[6] = {1.0f, 1.0f, 1.2f, 1.5f, 1.8f, 2.0f};
    float max_intensity = 0.0f;
    for (size_t band = 0; band < num_bands; band++) {
        intensities[band] *= weights[band];
        if (intensities[band] > max_intensity) {
            max_intensity = intensities[band];
        }
    }

    // Normalize and scale intensities to fit within visualization range
    float h_max = 0.3, h_min = 0.02;
    for (size_t band = 0; band < num_bands; band++) {
        if (max_intensity > 0) {
            intensities[band] = (intensities[band] / max_intensity) * (h_max - h_min) + h_min;
        } else {
            intensities[band] = h_min;
        }
    }

    // Draw rectangles for each band
    float bar_width = 0.125; // Width of each bar
    float gap = 0.02;        // Gap between bars
    float x_start = 0.125;   // Initial x-coordinate
    for (size_t band = 0; band < num_bands; band++) {
        draw_rect(canvas, x_start + band * (bar_width + gap), 0.1, bar_width, intensities[band], '#');
    }
}

void calculate_and_draw(float level, const float *fft_data, size_t fft_size, Canvas *canvas){
    //draw_rect_from_level(level, canvas);
    draw_rects_from_fft(fft_data, fft_size, canvas);
}
