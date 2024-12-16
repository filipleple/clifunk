#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include "canvas.h"
#include "config.h"

void calculate_and_draw(float level, const float *fft_data, size_t fft_size, Canvas *canvas);

#endif // VISUALIZATION_H
