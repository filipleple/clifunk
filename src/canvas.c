#include "canvas.h"
#include <stdlib.h>
#include <string.h>

Canvas *create_canvas() {
    Canvas *canvas = malloc(sizeof(Canvas));
    resize_canvas(canvas);
    return canvas;
}

void canvas_init(){
    initscr();
    noecho();
    curs_set(FALSE);
    timeout(0);
}

void destroy_canvas(Canvas *canvas) {
    for (int i = 0; i < canvas->height; i++) {
        free(canvas->buffer[i]);
        free(canvas->prev_buffer[i]);
    }
    free(canvas->buffer);
    free(canvas->prev_buffer);
    free(canvas);
}

void resize_canvas(Canvas *canvas) {
    int new_width, new_height;
    getmaxyx(stdscr, new_height, new_width);

    if (canvas->buffer) {
        for (int i = 0; i < canvas->height; i++) {
            free(canvas->buffer[i]);
            free(canvas->prev_buffer[i]);
        }
        free(canvas->buffer);
        free(canvas->prev_buffer);
    }

    canvas->width = new_width;
    canvas->height = new_height;
    canvas->buffer = malloc(new_height * sizeof(char *));
    canvas->prev_buffer = malloc(new_height * sizeof(char *));

    for (int i = 0; i < new_height; i++) {
        canvas->buffer[i] = calloc(new_width, sizeof(char));
        canvas->prev_buffer[i] = calloc(new_width, sizeof(char));
    }
}

void update_canvas_size(Canvas *canvas){
    if (LINES != canvas->height || COLS != canvas->width) {
        resize_canvas(canvas);
    }
}

void clear_canvas(Canvas *canvas) {
    for (int y = 0; y < canvas->height; y++) {
        memset(canvas->buffer[y], ' ', canvas->width);
    }
}

void render_canvas(Canvas *canvas) {
    for (int y = 0; y < canvas->height; y++) {
        for (int x = 0; x < canvas->width; x++) {
            if (canvas->buffer[y][x] != canvas->prev_buffer[y][x]) {
                mvaddch(y, x, canvas->buffer[y][x]);
                canvas->prev_buffer[y][x] = canvas->buffer[y][x];
            }
        }
    }
    refresh();
}

void draw_rect(Canvas *canvas, float x, float y, float w, float h, char fill) {
    int start_x = (int)(x * canvas->width);
    int start_y = (int)((1.0 - y - h) * canvas->height);
    int rect_width = (int)(w * canvas->width);
    int rect_height = (int)(h * canvas->height);

    for (int j = 0; j < rect_height; j++) {
        int y_coord = start_y + j;
        if (y_coord < 0 || y_coord >= canvas->height) continue;
        for (int i = 0; i < rect_width; i++) {
            int x_coord = start_x + i;
            if (x_coord < 0 || x_coord >= canvas->width) continue;
            canvas->buffer[y_coord][x_coord] = fill;
        }
    }
}
