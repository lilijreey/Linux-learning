/*

psf -- Process Stack Faker (a.k.a. Fucker)
Coded by Stas; (C)opyLeft by SysD Destructive Labs, 1997-2003

Tested on: FreeBSD 4.3, Linux 2.4, NetBSD 1.5, Solaris 2.7

Compile with:
# gcc -O2 -o psf psf.c
# strip psf

Did you ever need to *hide* what are you doing on somewhat like public
server? Like Quake server or maybe John The Ripper? 'Cos when your admin
run "ps auwx" or "top" and sees process like that, it's probable you'll
loose your shell on that server. So, what to do? Rootkit is a good solution
but you need root privilegies to install it and it's a bit overkill for
running an inoffensive eggdrop bot (belive me, I saw user installing rootkit
just to hide eggdrop!). Well, this little proggie does a job for you. It
*will not* erase some entry you wish to hide from process stack. It just
changes a commandline for "ps" entry ;)
This principle is widely used in many security-related programs. Nmap was
the first I saw. How does this technique works? Take a look at execv(3)
system call:

int execv( const char *path, char *const argv[]);

'path' is a path to executable file. And 'argv' array is... Well, it's
just the same 'argv' from:

int main(int argc, char *argv[])

where 'argv[0]' is a commandline and 'argv[1]' and higher are paramenters.
Normally 'argv[0]' receives the same value as 'path' from execv(3). But you
also can use other values! For example, when you run Nmap, it can execv(3)
itself with commandline changed to 'pine'. OK, commandline is gone. But what
to do with paramenters? Nmap uses environment to send paramenters user passed
to 'spoofed' process and ignores other paramenters. If you wish to spoof
'nmap -sS -vv -O -P0 -o lhost.log localhost' as 'pine -i', Nmap "remembers"
it's specific switches and re-execs itself as 'pine' with parameter '-i'.
Fine! But John The Ripper, Quake server & eggdrop can't fake parameters in
this way!!! What's the other way? Sorry, it's *very* dumb and *very* ugly...
What happens if you change commandline to something like:
'pine -i                                                            '
(Ya, 'pine -i' plus many space characters 0x20)? Hahah, "ps", "top" & many
other monitors just shift away *real* parameters! So, you don't hide them,
just shift away from screen. Such a "algorithm" doesn't needs neither rootkits,
neither special privilegies! Any user can do that at any time!!! *That's* "psf"
does. Try this:

# psf -s "pine -i" sleep 30 &
[1] 440
# ps auwx
...
stas        84  0.0  0.6  2012 1232 pts/0    S    19:12   0:00 bash -rcfile .bashrc
stas       440  0.0  0.1  1204  376 tty2     S    20:09   0:00 pine -i

stas       450  0.0  0.4  2544  816 tty2     R    20:12   0:00 ps auwx
...

Hahahaah, that's what we need! Please note that commandline change isn't
immediate, just wait a little before it completes. But... Did you noticed
a white line between processes 440 & 450? Uhm, that's our "shift buffer".
Pray for your admin don't notice that! Anyway, they are many more problems
with parameter shifting. "top" program, for example, shows "command names"
instead of "command lines" by default. You see a file name instead of
'argv[0]' value. "psf" tries to fix that creating symlink with name of
faked commandline to real program (on previous example, it creates symlink
/tmp/.psf-xxxx/pine => /usr/bin/sleep). Note that it doesn't works on *BSD
systems (*BSD kernel (?) follows symlink and shows real filename anyway).
The ways to discover faked processes I know are:

 * kidding with top(1)
 * ps auwx --cols 1024
 * cat /proc/[pidn]/cmdline (Linux only)
 * whatever non-standart process stack monitors
 * looking open files with "lsof" program
 * if you use -d (daemonize) option, be careful!!! As any cool daemon should
   do, "psf" closes std(in,out,err). What your admin will think if he (she)
   sees "pine -i" with no parent and neither allocated TTY?!

Too many, don't you think? So, what's *THE BEST* way to hide processes?
Rootkit sounds well, but it's a bit complex to use, you know... So, IMHO,
you must get source of program you wish to hide and hardcode all parameters
inside executable... After that, rename it in whatever and let it go!
Of course you must program at least C/C++ to do such a trick. Now, if
you're glad with my quick & dirty solution called "psf", happy faking!!!

*/

#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libgen.h>

#define VERSION		"0.03"
#define MAXPATH		256

char *fullpath(char *cmd)
{
	char *p, *q, *filename;
	struct stat st;

	if (*cmd == '/')
		return cmd;

	filename = (char *) malloc(MAXPATH);
	if  (*cmd == '.')
		if (getcwd(filename, MAXPATH - 1) != NULL)
		{
			strncat(filename, "/", MAXPATH - 1);
			strncat(filename, cmd, MAXPATH - 1);
			return filename;
		}
		else
			return NULL;

	for (p = q = (char *) getenv("PATH"); q != NULL; p = ++q)
	{
		if (q = (char *) strchr(q, ':'))
			*q = (char) '\0';

		snprintf(filename, MAXPATH, "%s/%s", p, cmd);

		if (stat(filename, &st) != -1
		    && S_ISREG(st.st_mode)
		    && (st.st_mode&S_IXUSR || st.st_mode&S_IXGRP || st.st_mode&S_IXOTH))
			return filename;

		if (q == NULL)
			break;
	}

	free(filename);
	return NULL;
}

int chownp (char *str)
{
	char user[MAXPATH], *group;
	struct passwd *pwd;
	struct group *grp;
	uid_t uid;
	gid_t gid;

	memset(user, '\0', sizeof(user));
	strncpy(user, str, sizeof(user));

	for (group = user; *group; group++)
		if (*group == ':')
		{
			*group = '\0';
			group++;
			break;
		}
	
	if (pwd = getpwnam(user))
	{
		uid = pwd->pw_uid;
		gid = pwd->pw_gid;
	}
	else
		uid = (uid_t) atoi(user);

	if (*group)
		if (grp = getgrnam(group))
			gid = grp->gr_gid;
		else
			gid = (gid_t) atoi(group);


	if (setgid(gid))
	{
		perror("[err] can't set GID");
		return 0;
	}
	if (setuid(uid))
	{
		perror("[err] can't set UID");
		return 0;
	}

	return 1;
}

char *mytmp(void)
{
	char *tmp, *me = "/.psf-", *ret;
	struct passwd *p;
	
	if ((tmp = (char *) getenv("TMPDIR")) == NULL)
		tmp = P_tmpdir;

	if ((p = getpwuid(getuid())) == NULL)
	{
		perror("[err] username reteival");
		exit(-1);
	}
	
	ret = (char *) malloc (strlen(tmp) + sizeof(me) + strlen(p->pw_name) + 1);
	*ret = '\0';

	strcat(ret, tmp);
	strcat(ret, me);
	strcat(ret, p->pw_name);
	
	return ret;
}

void usage(char *prog)
{
	printf("Process Stack Faker (a.k.a. Fucker) v" VERSION "\n");
	printf("Coded by Stas; (C)opyLeft by SysD Destructive Labs, 1997-2003\n\n");

	printf("Usage: %s [options] command arg1 arg2 ...\n", prog);
	printf("Where options can be:\n");
	printf("-s string\t"	"fake process name\n");
	printf("-p filename\t"	"file to write PID of spawned process - optional\n");
	printf("-d\t\t"		"try to start as daemon (in background, no tty) - optional\n");
	printf("-l\t\t"		"DO NOT exec through link (detectable by 'top'!!!) - optional\n");
	printf("-u uid[:gid]\t"	"(format just like in chown(1)) reset UID/GID - optional\n");
	printf("-n priority\t"	"renice process - optional\n\n");
	
	printf("Example: %s -s \"pine -i\" -d -n 19 ./john -session:websrv\n", prog);
}

int main(int argc, char *argv[])
{
	char buf[MAXPATH], fake[MAXPATH], fakecomm[MAXPATH];
	char *tmp, *fp, *p, *myexec, o;
	char *spoof = NULL, *pidfile = NULL;
	char **newargv;
	int i, j, n, daemon = 0, link = 1;
	FILE *f;
	int null;

	// Check for our parameters
	for (i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
			switch (o = argv[i][1])
			{
				case 's': 
					spoof = argv[++i]; break;
				case 'p':
					pidfile = argv[++i]; break;
				case 'd':
					daemon = 1; break;
				case 'l':
					link = 0; break;
				case 'u':
					chownp(argv[++i]); break;
				case 'n':
					nice(atoi(argv[++i])); break;

				default:
					usage(argv[0]);
					fprintf(stderr, "\n * Don't understood option -%c!\n", o);
					return -1;
			}
		else
			break;

	}

	// Is all OK?
	if (!(n = argc - i) || spoof == NULL)
	{
		usage(argv[0]);
		fprintf(stderr, "\n * Wrong usage!\n");
		return -1;
	}

	// Check for other's parameters
	newargv = (char **) malloc(n * sizeof(char **) + 1);
	for (j = 0; j < n; i++,j++)
		newargv[j] = argv[i];
	newargv[j] = NULL;

	if ((fp = fullpath(newargv[0])) == NULL)
	{
		perror("[err] full path seek");
		return -1;
	}
	
	// Now we'll make top happy linking wierd things...
	if (link)
	{
		tmp = mytmp();

		strncpy(buf, basename(spoof), strlen(spoof) + 1);
		for (p = buf; *p != '\0' && !isspace(*p); p++);
		*p = '\0';
		snprintf(fake, sizeof(fake), "%s/%s", tmp, buf);

		mkdir(tmp, 0700);	// try to create (see later if really created)
		remove(fake);		// reset any existing link
		if (symlink(fp, fake) == -1)
		{
			perror("[err] link creation");
			return -1;
		}
		
		myexec = fake;
	}
	else
		myexec = fp;

	if (n > 1)
	{
		// Build space-padded fake command
		memset(fakecomm, ' ', sizeof(fakecomm) - 1);
		fakecomm[sizeof(fakecomm) - 1] = '\0';
		strncpy(fakecomm, spoof, strlen(spoof));
		newargv[0] = fakecomm;
	}
	else
		newargv[0] = spoof;

	if (daemon)
	{
		if ((null = open("/dev/null", O_RDWR)) == -1)
		{
			perror("[err] /dev/null");
			return -1;
		}

		switch (fork())
		{
			case -1:
				perror("[err] fork1");
				return -1;
			case  0:
				setsid();
				switch (fork())
				{
					case -1:
						perror("[err] fork2");
						return -1;
					case  0:
						// chdir("/");
						umask(0);

						// close standart IO
						close(0);
						close(1);
						close(2);

						// open'em as /dev/null
						dup2(null, 0);
						dup2(null, 1);
						dup2(null, 2);

						break;
					default:
						return 0;
				}
				break;
			default:
				return 0;
		}
	}

	// Save PID if user asked...
	if (pidfile != NULL && (f = fopen(pidfile, "wt")) != NULL)
	{
		fprintf(f, "%d\n", getpid());
		fclose(f);
	}

/****** Some code from UPX 1.20 ;) ******/
	// Fork off a subprocess to clean up.
	// We have to do this double-fork trick to keep a zombie from
	// hanging around if the spawned original program doesn't check for
	// subprocesses (as well as to prevent the real program from getting
	// confused about this subprocess it shouldn't have).
	// Thanks to Adam Ierymenko <api@one.net> for this solution.

	if (link && !fork())
	{
		if (fork() == 0)
		{
			// Sleep 3 seconds, then remove the temp file.
			static const struct timespec ts = { 3, 0 };
			nanosleep(&ts, 0);

			if (unlink(fake) == -1)
				perror("[warn] can't erase symlink");
			if (rmdir(tmp) == -1)
				perror("[warn] can't remove tmpdir");
		}
		exit(0);
	}

	// Wait for the first fork()'d process to die.
	waitpid(-1, (int *)0, 0);
/****** Some code from UPX 1.20 ;) ******/
	
	// And now, execute it!
	execv(myexec, newargv);

	perror("[err] couldn't execute");
	return -1;
}
