all:

OBJS=cyg

cyg: cyg_profile_func.c
	gcc -finstrument-functions $^ -o $@

test: test.c
	gcc -Wall -Wformat=2 $^ -o $@

clean:
	@rm -f *.o ${OBJS}
