CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11 -D_GNU_SOURCE
EXTENSION = elf

main.$(EXTENSION): main.c
	$(CC) 	-I./DebugLibC 								\
			-I./DebugLibC/colors-C-C-plus-plus 			\
			-DDEBUG_ENABLE								\
			./DebugLibC/src/*.c							\
			./DebugLibC/colors-C-C-plus-plus/src/*.c	\
			$(CFLAGS) $^ -o $@

clean:
	rm -f main.$(EXTENSION)
