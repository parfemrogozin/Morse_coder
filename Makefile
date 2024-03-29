CC = gcc
CFLAGS = -std=gnu18 -Wno-unused-parameter -Wall -Wextra `sdl2-config --cflags`
LIBS = -lm `sdl2-config --libs` -lreadline
SRCS = $(wildcard *.c)
EXECUTABLE = morse

all: CFLAGS += -g
all: $(SRCS)
	$(CC) -o $(EXECUTABLE) $(CFLAGS) $(LIBS) $^

release: CFLAGS += -march=native -O2 -s
release: $(SRCS)
	$(CC) -o $(EXECUTABLE) $(CFLAGS) $(LIBS) $^

clean:
	rm $(EXECUTABLE)

