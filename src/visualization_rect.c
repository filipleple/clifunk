#include "canvas.h"
#include "visualization.h"

void draw_rect_from_level(float level, Canvas* canvas);

void draw_rect_from_level(float level, Canvas* canvas){
    // Draw a square at the center, 25% of terminal size
    float h_min = 0.02, h_max = 0.25;

    float h = level * (h_max - h_min);
    draw_rect(canvas, 0.375, 0.375, 0.25, h, '#');
}

void calculate_and_draw(float level, Canvas* canvas){
    draw_rect_from_level(level, canvas);
}
