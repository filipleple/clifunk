#ifndef CANVAS_H
#define CANVAS_H

#include <ncurses.h>

typedef struct {
    int width;
    int height;
    char **buffer; // Holds current frame content
    char **prev_buffer; // Holds previous frame content
} Canvas;

Canvas *create_canvas();
void destroy_canvas(Canvas *canvas);
void resize_canvas(Canvas *canvas);
void clear_canvas(Canvas *canvas);
void render_canvas(Canvas *canvas);

// Drawing API
void draw_rect(Canvas *canvas, float x, float y, float w, float h, char fill);

#endif // CANVAS_H

