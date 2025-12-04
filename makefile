dice: main.c
	cc -o dice main.c

install: dice
	mkdir -p /usr/local/bin
	cp dice /usr/local/bin
	mkdir -p /usr/local/share/man/man1
	cp dice.1 /usr/local/share/man/man1

.PHONY: clean

clean:
	rm dice
