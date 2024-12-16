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

// Constants for falloff and peak timing
#define FALLOFF_RATE 0.05f   // How fast the bars fall off
#define PEAK_FALLOFF_RATE 0.01f // How fast the peaks fall off

// Arrays to track falloff and peaks
static float previous_heights[6] = {0.0f};
static float peak_heights[6] = {0.0f};

void draw_rects_from_fft(const float *fft_data, size_t fft_size, Canvas* canvas) {
    // Define frequency ranges
    float freq_per_bin = SAMPLE_RATE / (float)FFT_SIZE;
    size_t ranges[7] = {
        0,                                // Start of sub-bass
        (size_t)(60 / freq_per_bin),      // End of sub-bass
        (size_t)(300 / freq_per_bin),     // End of bass
        (size_t)(1000 / freq_per_bin),    // End of lower midrange
        (size_t)(5000 / freq_per_bin),    // End of upper midrange
        (size_t)(10000 / freq_per_bin),   // End of presence
        (size_t)(16000 / freq_per_bin)    // End of brilliance
    };

    // Calculate intensities for each range
    float intensities[6] = {0.0f};
    for (int band = 0; band < 6; band++) {
        for (size_t i = ranges[band]; i < ranges[band + 1]; i++) {
            intensities[band] += log1p(fft_data[i]); // Logarithmic scaling
        }
        intensities[band] /= (ranges[band + 1] - ranges[band]);
    }

    // Apply weighting to balance the bands visually
    const float weights[6] = {0.3f, 0.25f, 0.3f, 1.5f, 1.2f, 1.5f};
    float max_intensity = 0.0f;
    for (int band = 0; band < 6; band++) {
        intensities[band] *= weights[band];
        if (intensities[band] > max_intensity) {
            max_intensity = intensities[band];
        }
    }

    // Normalize and scale intensities to fit within visualization range
    float h_max = 0.3, h_min = 0.02;
    for (int band = 0; band < 6; band++) {
        if (max_intensity > 0) {
            intensities[band] = (intensities[band] / max_intensity) * (h_max - h_min) + h_min;
        } else {
            intensities[band] = h_min;
        }
    }

    // Update falloff and peaks
    for (int band = 0; band < 6; band++) {
        // Update bar falloff
        if (intensities[band] < previous_heights[band]) {
            previous_heights[band] -= FALLOFF_RATE;
            if (previous_heights[band] < intensities[band]) {
                previous_heights[band] = intensities[band];
            }
        } else {
            previous_heights[band] = intensities[band];
        }

        // Update peak heights
        if (intensities[band] > peak_heights[band]) {
            peak_heights[band] = intensities[band];
        } else {
            peak_heights[band] -= PEAK_FALLOFF_RATE;
            if (peak_heights[band] < previous_heights[band]) {
                peak_heights[band] = previous_heights[band];
            }
        }
    }

    // Draw bars and peaks
    float bar_width = 0.125; // Width of each bar
    float gap = 0.02;        // Gap between bars
    float x_start = 0.125;   // Initial x-coordinate
    for (int band = 0; band < 6; band++) {
        // Draw main bar
        draw_rect(canvas, x_start + band * (bar_width + gap), 0.1, bar_width, previous_heights[band], '#');

        // Draw peak indicator as a single character
        float peak_y = 0.1 + peak_heights[band];
        draw_rect(canvas, x_start + band * (bar_width + gap), peak_y, bar_width, 0.01, '*');
    }
}

void calculate_and_draw(float level, const float *fft_data, size_t fft_size, Canvas *canvas){
    //draw_rect_from_level(level, canvas);
    draw_rects_from_fft(fft_data, fft_size, canvas);
}
