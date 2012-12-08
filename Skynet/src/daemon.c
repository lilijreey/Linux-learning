#include <stdarg.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <linux/unistd.h>

#include "log.h"
#include "conf_parser/config.h"

#include "shmq.h"
#include "daemon.h"

int max_fd_num;

volatile int	stop        = 0;
volatile int	restart     = 0;
char** saved_argv = NULL;
static char*	arg_start;
static char*	arg_end;
static char*	env_start;
static int		backgd_mode = 0;
static int		status;

const char* const signame[] = {
	"SIG_0",
	"SIGHUP",	"SIGINT",	"SIGQUIT",	"SIGILL",
	"SIGTRAP",	"SIGABRT",	"SIGBUS",	"SIGFPE",
	"SIGKILL",	"SIGUSR1",	"SIGSEGV",	"SIGUSR2",
	"SIGPIPE",	"SIGALRM",	"SIGTERM",	"SIGSTKFLT",
	"SIGCHLD",	"SIGCONT",	"SIGSTOP",	"SIGTSTP",
	"SIGTTIN",	"SIGTTOU",	"SIGURG",	"SIGXCPU",
	"SIGXFSZ",	"SIGVTALRM",	"SIGPROF",	"SIGWINCH",
	"SIGIO",	"SIGPWR",	"SIGSYS",
	"SIGRT_0",	"SIGRT_1",	"SIGRT_2",	"SIGRT_3",
	"SIGRT_4",	"SIGRT_5",	"SIGRT_6",	"SIGRT_7",
	"SIGRT_8",	"SIGRT_9",	"SIGRT_10",	"SIGRT_11",
	"SIGRT_12",	"SIGRT_13",	"SIGRT_14",	"SIGRT_15",
	"SIGRT_16",	"SIGRT_17",	"SIGRT_18",	"SIGRT_19",
	"SIGRT_20",	"SIGRT_21",	"SIGRT_22",	"SIGRT_23",
	"SIGRT_24",	"SIGRT_25",	"SIGRT_26",	"SIGRT_27",
	"SIGRT_28",	"SIGRT_29",	"SIGRT_30",	"SIGRT_31",
	"SIGRT_32"
};

static void sigterm_handler(int signo) 
{
	stop     = 1;
	restart  = 0;
	DEBUG_LOG("SIG_TERM from pid=%d", getpid());
}

static void sighup_handler(int signo) 
{
	restart  = 1;
	stop     = 1;
}

static void sigchld_handler(int signo, siginfo_t *si, void * p) 
{
	while (waitpid (-1, &status, WNOHANG|__WALL) > 0);
#if 0
	char *corename;

	switch (si->si_code) {
		case SI_USER:
		case SI_TKILL:
			DEBUG_LOG("SIGCHLD from pid=%d uid=%d, IGNORED",
					si->si_pid, si->si_uid);
			return; /* someone send use fake SIGCHLD */
		case CLD_KILLED:
			DEBUG_LOG("child %d killed by signal %s",
					si->si_pid, signame[WTERMSIG(si->si_status)]);
			stop = 1;
			if (WTERMSIG(si->si_status) == SIGABRT)
				restart = 1;
			break;
		case CLD_TRAPPED:
			DEBUG_LOG("child %d trapped", si->si_pid);
			return;
		case CLD_STOPPED:
			DEBUG_LOG("child %d stopped", si->si_pid);
			if(si->si_pid > 1) kill(si->si_pid, SIGCONT);
			return;
		case CLD_CONTINUED:
			DEBUG_LOG("child %d continued", si->si_pid);
			return;
		case CLD_DUMPED:
			DEBUG_LOG("child %d coredumped by signal %s",
					si->si_pid, signame[WTERMSIG(si->si_status)]);
			chmod("core", 700);
			chown("core", 0, 0);
			corename = alloca(40);
			sprintf(corename, "core.%d", si->si_pid);
			rename("core", corename);
			restart = 1;
			stop = 1;
			break;
	}
#endif
}

static inline void
dup_argv(int argc, char** argv)
{
	saved_argv = malloc(sizeof(char*) * (argc + 1));
	if (!saved_argv) {
		return;
	}
	saved_argv[argc] = NULL;
	while (--argc >= 0) {
		saved_argv[argc] = strdup(argv[argc]);
	}
}

static inline void
rlimit_reset()
{
	struct rlimit rlim;

	max_fd_num = config_get_intval("max_open_fd", 100);

	/* raise open files */
	rlim.rlim_cur = max_fd_num;
	rlim.rlim_max = max_fd_num;
	if (setrlimit(RLIMIT_NOFILE, &rlim) == -1) {
		ALERT_LOG("INIT FD RESOURCE FAILED");
	}

	/* allow core dump */
	rlim.rlim_cur = 1 << 30;
	rlim.rlim_max = 1 << 30;
	if (setrlimit(RLIMIT_CORE, &rlim) == -1) {
		ALERT_LOG("INIT CORE FILE RESOURCE FAILED");
	}
}

int daemon_start(int argc, char** argv)
{
	struct sigaction sa;
	sigset_t sset;
	const char *style;

	rlimit_reset ();

	memset(&sa, 0, sizeof(sa));
	signal(SIGPIPE,SIG_IGN);	

	sa.sa_handler = sigterm_handler;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	
	sa.sa_handler = sighup_handler;
	sigaction(SIGHUP, &sa, NULL);

	sa.sa_flags = SA_RESTART|SA_SIGINFO;
	sa.sa_sigaction = sigchld_handler;
	sigaction(SIGCHLD, &sa, NULL);
/*	sigaction(SIGSEGV, &sa, NULL);
	sigaction(SIGBUS, &sa, NULL);
	sigaction(SIGILL, &sa, NULL);
	sigaction(SIGABRT, &sa, NULL);
	sigaction(SIGFPE, &sa, NULL);
*/	
	sigemptyset(&sset);
	sigaddset(&sset, SIGSEGV);
	sigaddset(&sset, SIGBUS);
	sigaddset(&sset, SIGABRT);
	sigaddset(&sset, SIGILL);
	sigaddset(&sset, SIGCHLD);
	sigaddset(&sset, SIGFPE);
	sigprocmask(SIG_UNBLOCK, &sset, &sset);

	arg_start = argv[0];
	arg_end = argv[argc-1] + strlen (argv[argc - 1]) + 1;
	env_start = environ[0];
	dup_argv(argc, argv);

	style = config_get_strval ("run_mode");
	if (!style || !strcasecmp ("background", style)) {
		if (-1 == daemon (1, 1))
            return 1;
		backgd_mode = 1;
		BOOT_LOG (0, "switch to daemon mode");
	}
	return 0;
}

void daemon_stop(void) 
{
	if (!backgd_mode) {
		printf("Server stopping...\n");
	}

	if (restart && prog_name && saved_argv) {
		WARN_LOG("%s", "Server restarting...");
		if (-1 != chdir(current_dir))
		    execv(prog_name, saved_argv);
		WARN_LOG("%s", "Restart Failed...");
	}

	free_argv();
	free(prog_name);
	free(current_dir);
}

void daemon_set_title(const char* fmt, ...)
{
	char title [64];
	int i, tlen;
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(title, sizeof(title) - 1, fmt, ap);
	va_end(ap);

	tlen = strlen(title) + 1;
	if (arg_end - arg_start < tlen && env_start == arg_end) {
		char *env_end = env_start;
		for (i = 0; environ[i]; i++) {
			if(env_end == environ[i]) {
				env_end = environ[i] + strlen (environ[i]) + 1;
				environ[i] = strdup(environ[i]);
			} else
				break;
		}
		arg_end = env_end;
		env_start = NULL;
	}

	i = arg_end - arg_start;
	if (tlen == i) {
		strcpy (arg_start, title);
	} else if (tlen < i) {
		strcpy (arg_start, title);
		memset (arg_start + tlen, 0, i - tlen);
	} else {
		stpncpy(arg_start, title, i - 1)[0] = '\0';
	}
}
