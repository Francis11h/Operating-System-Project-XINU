#include <kernel.h>
#include <stdio.h>
#include <proc.h>
#include <lab0.h>

int isTracing;
int proc_syscall_count[NPROC][NSYSCALL];
unsigned long proc_syscall_time[NPROC][NSYSCALL];
int isCalled[NPROC];

void syscallsummary_start() {
	int i, j;
	for (i = 0; i < NPROC; i++) {
		for (j = 0; j < NSYSCALL; j++) {
			proc_syscall_count[i][j] = 0;
			proc_syscall_time[i][j] = 0;
		}
		isCalled[i] = FALSE;
	}
	isTracing = TRUE;
}

void syscallsummary_stop() {
	isTracing = FALSE;
}

char syscall_name[NSYSCALL][SCNMLEN] = {"sys_freemem", "sys_chprio", "sys_getpid", "sys_getprio", "sys_gettime", 
	"sys_kill", "sys_receive", "sys_recvclr", "sys_recvtim", "sys_resume", "sys_scount", "sys_sdelete", "sys_send", 
	"sys_setdev", "setnok", "sys_screate", "sys_signal", "sys_signaln", "sys_sleep", "sys_sleep10", "sys_sleep100", 
	"sys_sleep1000", "sreset", "sys_stacktrace", "sys_suspend", "sys_unsleep", "sys_wait"};


void printsyscallsummary() {

	
	kprintf("\nvoid printsyscallsummary()\n");
	int i, j;
	for (i = 0; i < NPROC; i++) {

		if (!isCalled[i]) continue;
		kprintf("Process [pid:%d]\n", i);

		for (j = 0; j < NSYSCALL; j++) {
			if (proc_syscall_count[i][j] > 0) {
				//sys count
				kprintf("\tSyscall: %s, count: %d, ", syscall_name[j], proc_syscall_count[i][j]);
				//sys averge time
				kprintf("average execution time: %d (ms)\n", proc_syscall_time[i][j] / proc_syscall_count[i][j]);
			}
		}
	}
}