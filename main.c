#include <ncurses.h>
#include <unistd.h>
#include "canvas.h"

int main() {
    initscr();
    noecho();
    curs_set(FALSE);
    timeout(0);

    Canvas *canvas = create_canvas();

    while (1) {
        int ch = getch();
        if (ch == 'q') break;

        // Update canvas size if terminal is resized
        if (LINES != canvas->height || COLS != canvas->width) {
            resize_canvas(canvas);
        }

        clear_canvas(canvas);

        // Draw a square at the center, 25% of terminal size
        draw_rect(canvas, 0.375, 0.375, 0.25, 0.25, '#');

        render_canvas(canvas);
        usleep(50000); // 50ms delay for smooth animation
    }

    destroy_canvas(canvas);
    endwin();
    return 0;
}

