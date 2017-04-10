all:

OBJECTS=signal mask

signal: signal.c
	gcc -Wall signal.c -o $@ 

mask: sigpromask.c 
	gcc -Wall sigpromask.c -o $@ 
clean:
	@rm -rf *.o ${OBJECTS}
