#include <stdio.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <linux/user.h>
#include <errno.h>

int main(int argc, char **argv) {
	pid_t pid;
	struct user_regs_struct regs;
	int i=0;
	char **args=("arg1", "arg2", "arg3", "arg4", NULL);
	long int addr=0;
	addr=strtoul(*(argv+2), (char **) NULL, 16);

	pid=fork();
	
	if(pid == -1) {
		return -1;
	} else if(pid == 0) {
		ptrace(PTRACE_TRACEME, NULL, NULL, NULL);
		execv(*(argv+1), args);
	} else {
		wait();
		printf("%08x\n", ptrace(PTRACE_PEEKDATA, pid, addr, NULL));
		printf("%08x\n", addr);
		printf("%s\n", strerror(errno));
	}

	return 1;
}
