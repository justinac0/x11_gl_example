CFLAGS=-Wall -pedantic -Wno-unused-function -Wno-gnu-zero-variadic-macro-arguments -Wno-unused-function -Wno-undefined-internal -fsanitize=address -std=c99 -DWINDOW_XCB
LDFLAGS=-lEGL -lxcb -lGL -lm

all: build run

format:
	./format.sh

build:
	mkdir -p bin/
	clang $(CFLAGS) -c src/build.c -I./ -I./src -I./thirdparty/glad/include -I./thirdparty
	clang -fsanitize=address build.o -o app -I./ $(LDFLAGS)

run:
	./app

clean:
	rm *.o ./app

.PHONY: run clean format
