
all:

CFLAGS=`pkg-config --libs libssl`

push: ssl.c
	gcc -Wall -g -o $@ $^ ${CFLAGS}

bio: bio.c
	gcc -Wall -g -o $@ $^ ${CFLAGS}

tt: tt.c
	gcc -Wall -g -o $@ $^ ${CFLAGS}

clean:
	@rm -f push bio *.o
