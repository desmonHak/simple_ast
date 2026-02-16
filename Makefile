CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11 -D_GNU_SOURCE
EXTENSION = elf

main.$(EXTENSION): main.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f main.$(EXTENSION)
