all:

CC := gcc
LIB := -D_REENTRANT -lpthread
OPTION := -Wall -std=gnu99
# -O 优化
O:=-O$(o) #immediately assigment


define compile-and-run
	$(CC) $(OPTION) $(O)  $< $(LIB)  -o $@
endef

OBJECTS = create_thread  rwlock join

create_thread: create_thread.c
	$(compile-and-run)

rwlock: rwlock.c
	$(compile-and-run)

join: join.c
	$(compile-and-run)

buf: buf.c
	$(compile-and-run)

clean:
	@rm -rf *.o ${OBJECTS}

sig: sigwait_t.c
	$(compile-and-run)

.PHONY: all clean

