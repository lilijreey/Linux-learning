/**
 * @file     fcntl.c
 *          ; 
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     03/07/2014 04:59:46 PM
 *
 */
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>
int set_read_pipe(int fd) 
{
	int var = (O_NOATIME| fcntl(fd, F_GETFL));
  return fcntl(fd, F_SETFL, var);
}


int
main(int argc, char *argv[])
{
  int pip[2];
  pipe(pip);
  set_read_pipe(pip[0]);
  return 0;
}				/* ----------  end of function main  ---------- */
