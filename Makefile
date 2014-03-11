PREFIX=/usr/local
CC=gcc
LIBS=-ldl
CFLAGS=-Wall -Wextra -Werror -pedantic -std=c99 -O2

.PHONY: all clean install uninstall

all: dltest

dltest: dltest.c
	$(CC) $(CFLAGS) $(LIBS) $< -o $@

install: $(PREFIX)/bin/dltest

$(PREFIX)/bin/dltest: dltest
	install $< $@

uninstall:
	rm $(PREFIX)/bin/dltest

clean:
	rm dltest
