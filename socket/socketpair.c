/**
 * @file     socketpair.c
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     03/10/2014 03:39:37 PM
 *
 */
#include <stdlib.h>

#include <string.h>
#include <sys/types.h>	       /* See NOTES */
       #include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>


/* EE
 * 自己实现socketpair
 * 使用tcp
 *  
 * int pair[2];
 *  1. listen INET_LOOPBACK
 *  2. connet INET_LOOPBACK ->pair[1]
 *  3. pair[0] = accept
 *  4. close listen
 *  5. fork
 *  6.1 parent
 *      close pair[1]
 *      ok
 *  6.2 client
 *      close pair[0]
 *      dup2(pair[1], STDIN)
 *      dup2(pair[1], STDOUT)
 *      system(prog)
 * see XX 
 */

/* Run a program on a local tcp socket, so that we can talk to it's stdin and
 * stdout.  This is used to fake a connection to a daemon for testing -- not
 * for the normal case of running SSH.
 *
 * Retruns a socket which is attached to a subprocess running "prog". stdin and
 * stdout are attached. stderr is left attached to the original stderr. */
static int sock_exec(const char *prog)
{
	pid_t pid;
	int fd[2];

	if (socketpair_tcp(fd) != 0) {
		rsyserr(FERROR, errno, "socketpair_tcp failed");
		return -1;
	}
	if (verbose >= 2)
		rprintf(FINFO, "Running socket program: \"%s\"\n", prog);

	pid = fork();
	if (pid < 0) {
		rsyserr(FERROR, errno, "fork");
		exit_cleanup(RERR_IPC);
	}

	if (pid == 0) {
		close(fd[0]);
		if (dup2(fd[1], STDIN_FILENO) < 0
		 || dup2(fd[1], STDOUT_FILENO) < 0) {
			fprintf(stderr, "Failed to run \"%s\"\n", prog);
			exit(1);
		}
		exit(system(prog));
	}

	close(fd[1]);
	return fd[0];
}

/* This is like socketpair but uses tcp.  The function guarantees that nobody
 * else can attach to the socket, or if they do that this function fails and
 * the socket gets closed.  Returns 0 on success, -1 on failure.  The resulting
 * file descriptors are symmetrical.  Currently only for RSYNC_CONNECT_PROG. */
static int socketpair_tcp(int fd[2])
{
	int listener;
	struct sockaddr_in sock;
	struct sockaddr_in sock2;
	socklen_t socklen = sizeof sock;
	int connect_done = 0;

	fd[0] = fd[1] = listener = -1;

	memset(&sock, 0, sizeof sock);

	if ((listener = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		goto failed;

	memset(&sock2, 0, sizeof sock2);
#ifdef HAVE_SOCKADDR_IN_LEN
	sock2.sin_len = sizeof sock2;
#endif
	sock2.sin_family = PF_INET;
	sock2.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if (bind(listener, (struct sockaddr *)&sock2, sizeof sock2) != 0
	 || listen(listener, 1) != 0
	 || getsockname(listener, (struct sockaddr *)&sock, &socklen) != 0
	 || (fd[1] = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		goto failed;

	set_nonblocking(fd[1]);

	sock.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if (connect(fd[1], (struct sockaddr *)&sock, sizeof sock) == -1) {
		if (errno != EINPROGRESS)
			goto failed;
	} else
		connect_done = 1;

	if ((fd[0] = accept(listener, (struct sockaddr *)&sock2, &socklen)) == -1)
		goto failed;

	close(listener);
	listener = -1;

	set_blocking(fd[1]);

	if (connect_done == 0) {
		if (connect(fd[1], (struct sockaddr *)&sock, sizeof sock) != 0
		    && errno != EISCONN)
			goto failed;
	}

	/* all OK! */
	return 0;

 failed:
	if (fd[0] != -1)
		close(fd[0]);
	if (fd[1] != -1)
		close(fd[1]);
	if (listener != -1)
		close(listener);
	return -1;
}

int
main(int argc, char *argv[])
{
    int so[2];
    char buf[32]={0};
    if (-1 == socketpair(AF_UNIX, SOCK_STREAM, 0, so)) {
        perror("socketpair");
        exit(1);
    }

    write(so[0], "xxx", 3);
    read(so[1], buf, 3);

    printf("so 0 buf:%s\n", buf);

    write(so[1], "yyy", 3);
    read(so[0], buf, 3);
    printf("so 0 buf:%s\n", buf);

    return 0;
}				/* ----------  end of function main  ---------- */
