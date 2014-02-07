all: t.c t.h

t.c: t.x
	rpcgen -c -o $@ $<

t.h: t.x
	rpcgen -h -o $@ $<
