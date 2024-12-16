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

#define PEAK_FALLOFF 0.1f // Controls peak cap fall-off speed
#define SMOOTHING_FACTOR 0.05f

static float smoothed_intensities[NUM_BANDS] = {0};
static float peak_values[NUM_BANDS] = {0};

// Smooth and normalize FFT intensities
void calculate_band_intensities(const float *fft_data, size_t fft_size, float *intensities) {
    float freq_per_bin = SAMPLE_RATE / (float)FFT_SIZE;

    // Define frequency ranges
    size_t ranges[NUM_BANDS + 1] = {
        0,                                // 20 Hz
        (size_t)(60 / freq_per_bin),      // 60 Hz
        (size_t)(250 / freq_per_bin),     // 250 Hz
        (size_t)(500 / freq_per_bin),     // 500 Hz
        (size_t)(2000 / freq_per_bin),    // 2000 Hz
        (size_t)(6000 / freq_per_bin),    // 6000 Hz
        (size_t)(16000 / freq_per_bin)    // 16000 Hz
    };

    // Calculate band magnitudes
    for (int band = 0; band < NUM_BANDS; band++) {
        float sum = 0.0f;
        for (size_t i = ranges[band]; i < ranges[band + 1]; i++) {
            sum += log1p(fft_data[i]); // Logarithmic scaling
        }
        float magnitude = sum / (ranges[band + 1] - ranges[band]);

        // Smooth intensities using EMA
        smoothed_intensities[band] =
            SMOOTHING_FACTOR * magnitude + (1.0f - SMOOTHING_FACTOR) * smoothed_intensities[band];

        // Update peaks
        if (smoothed_intensities[band] > peak_values[band]) {
            peak_values[band] = smoothed_intensities[band];
        } else {
            peak_values[band] -= PEAK_FALLOFF; // Gradual fall-off
        }

        intensities[band] = smoothed_intensities[band];
    }
}

void draw_bars_from_intensities(const float *intensities, Canvas *canvas) {
    float h_max = 0.3f, h_min = 0.02f; // Bar height range
    float bar_width = 0.125f;          // Width of each bar
    float gap = 0.02f;                 // Gap between bars
    float x_start = 0.125f;            // Initial x-coordinate

    for (int band = 0; band < NUM_BANDS; band++) {
        // Scale bar height
        float bar_height = intensities[band] * (h_max - h_min) + h_min;

        // Scale peak height
        float peak_height = peak_values[band] * (h_max - h_min) + h_min;

        // Draw the bar
        draw_rect(canvas, x_start + band * (bar_width + gap), 0.1, bar_width, bar_height, '#');

        // Draw the peak cap as a single character above the bar
        float peak_y = 0.1 + peak_height;
        draw_rect(canvas, x_start + band * (bar_width + gap), peak_y, bar_width, 0.01, '-');
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
