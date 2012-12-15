OBJECTS=mmap

all: ${OBJECTS}

mmap: mmap.c
	@gcc -Wall -std=gnu99 -o $@ $<

clean:
	rm -rf *.o ${OBJECTS}
