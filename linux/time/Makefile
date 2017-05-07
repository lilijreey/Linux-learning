all:

objects= posix_clock alarm a.out 
CC=gcc
FALGE= -Wall -std=gnu99

#Link with -lrt.
posix_clock: posix_clock.c
	$(CC) $(FALGE)  $^ -lrt -o $@

alarm:	alarm.c
	$(CC) $(FALGE)  $^ -lrt -o $@

clean:
	rm -rf $(objects)




