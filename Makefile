CC = gcc
CFLAGS = -Wall -Wextra -pedantic -lncurses
OBJS = main.o canvas.o

all: render_engine

render_engine: $(OBJS)
	$(CC) -o render_engine $(OBJS) $(CFLAGS)

main.o: main.c canvas.h
	$(CC) -c main.c $(CFLAGS)

canvas.o: canvas.c canvas.h
	$(CC) -c canvas.c $(CFLAGS)

clean:
	rm -f $(OBJS) render_engine

