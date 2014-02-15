all: t.c t.h w r

t.c: t.x
	rpcgen -c -o $@ $<

t.h: t.x
	rpcgen -h -o $@ $<

w: writer.c
	gcc -std=gnu99 -g -Wall -o $@ $^

r: reader.c
	gcc -std=gnu99 -g -Wall -o $@ $^
