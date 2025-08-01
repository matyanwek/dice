.PHONY: all

all:
	cc -o dice dice.c

.PHONY: install

install:
	mkdir -p /usr/local/bin /usr/local/share/man/man1
	cc -o dice dice.c
	cp dice /usr/local/bin
	cp dice.1 /usr/local/share/man/man1
