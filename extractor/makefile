CC=gxx
CFLAGS=-I.
DEPS = extract.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

extract: extract.c extract.c
	gcc -o extract extract.c extractfunc.c -I.
