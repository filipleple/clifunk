#include "canvas.h"
#include "visualization.h"
#include <math.h>


#define NUM_BANDS 6

void draw_rect_from_level(float level, Canvas* canvas);
void draw_rects_from_fft(const float *fft_data, size_t fft_size, Canvas* canvas);
void calculate_logarithmic_ranges(size_t *ranges, size_t num_bands, float freq_per_bin, float start_freq, float end_freq);
void calculate_band_intensities(const float *fft_data, size_t fft_size, float *intensities);
void draw_bars_from_intensities(const float *intensities, Canvas *canvas);

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


void calculate_band_intensities(const float *fft_data, size_t fft_size, float *intensities) {
    // Define frequency ranges
    float freq_per_bin = SAMPLE_RATE / (float)FFT_SIZE;
    size_t ranges[NUM_BANDS + 1] = {
        0,                                // Start of sub-bass
        (size_t)(60 / freq_per_bin),      // End of sub-bass
        (size_t)(300 / freq_per_bin),     // End of bass
        (size_t)(1000 / freq_per_bin),    // End of lower midrange
        (size_t)(5000 / freq_per_bin),    // End of upper midrange
        (size_t)(10000 / freq_per_bin),   // End of presence
        (size_t)(16000 / freq_per_bin)    // End of brilliance
    };

    // Weights to balance the frequency bands visually
    const float weights[NUM_BANDS] = {0.3f, 0.25f, 0.3f, 1.5f, 1.2f, 1.5f};

    // Calculate intensities for each band
    float max_intensity = 0.0f;
    for (int band = 0; band < NUM_BANDS; band++) {
        intensities[band] = 0.0f;
        for (size_t i = ranges[band]; i < ranges[band + 1]; i++) {
            intensities[band] += log1p(fft_data[i]); // Logarithmic scaling
        }
        intensities[band] /= (ranges[band + 1] - ranges[band]); // Average intensity
        intensities[band] *= weights[band];                    // Apply band weight

        if (intensities[band] > max_intensity) {
            max_intensity = intensities[band];
        }
    }

    // Normalize to ensure consistent scaling
    float normalization_factor = (max_intensity > 0) ? 1.0f / max_intensity : 1.0f;
    for (int band = 0; band < NUM_BANDS; band++) {
        intensities[band] *= normalization_factor;
    }
}

void draw_bars_from_intensities(const float *intensities, Canvas *canvas) {
    float h_max = 0.3f, h_min = 0.02f; // Visualization range for bar heights
    float bar_width = 0.125f;          // Width of each bar
    float gap = 0.02f;                 // Gap between bars
    float x_start = 0.125f;            // Initial x-coordinate

    for (int band = 0; band < NUM_BANDS; band++) {
        // Scale the intensity to visualization range
        float bar_height = intensities[band] * (h_max - h_min) + h_min;

        // Draw the bar
        draw_rect(canvas, x_start + band * (bar_width + gap), 0.1, bar_width, bar_height, '#');
    }
}

void draw_rects_from_fft(const float *fft_data, size_t fft_size, Canvas *canvas) {
    float intensities[NUM_BANDS] = {0.0f};
    calculate_band_intensities(fft_data, fft_size, intensities);
    draw_bars_from_intensities(intensities, canvas);
}

void calculate_and_draw(float level, const float *fft_data, size_t fft_size, Canvas *canvas){
    //draw_rect_from_level(level, canvas);
    draw_rects_from_fft(fft_data, fft_size, canvas);
}
