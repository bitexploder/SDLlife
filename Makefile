CC=gcc
CFLAGS=-c -g
#LIBS=-lSDL -lSDL_gfx -lmingw32 -mwindows
LIBS=`sdl-config --libs` -lSDL_gfx -lSDL_ttf

all: life

life: main.o life.o draw.o ui.o bench.o stack.o
	$(CC)	ui.o main.o life.o draw.o bench.o stack.o -o bin/sdllife	$(LIBS)

main.o: main.c
	$(CC) $(CFLAGS) main.c

life.o: life.c
	$(CC) $(CFLAGS) life.c

draw.o: draw.c
	$(CC) $(CFLAGS) draw.c

ui.o: ui.c
	$(CC) $(CFLAGS) ui.c

bench.o: bench.c
	$(CC) $(CFLAGS) bench.c

stack.o: stack.c
	$(CC) $(CFLAGS) stack.c

clean:
	rm *.o bin/sdllife bin/sdllife.exe

win: all
	mv bin/sdllife bin/sdllife.exe
