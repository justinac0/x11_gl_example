CFLAGS=-Wall -pedantic -Wno-unused-function  -Wno-unused-function -Wno-undefined-internal -std=c99 -DWINDOW_XLIB
LDFLAGS=-lEGL -lX11 -lGL

all: build run

format:
	./format.sh

build:
	clang $(CFLAGS) -c build.c -I./ -I./thirdparty/glad/include
	clang -fsanitize=address build.o -o app -I./ $(LDFLAGS)

run:
	./app

clean:
	rm *.o ./app

.PHONY: run clean format
