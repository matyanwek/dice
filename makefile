dice: main.c
	cc -o dice main.c

install: dice dice.1
	mkdir -p /usr/local/bin
	cp dice /usr/local/bin
	mkdir -p /usr/local/share/man/man1
	cp dice.1 /usr/local/share/man/man1

clean: dice
	rm dice
