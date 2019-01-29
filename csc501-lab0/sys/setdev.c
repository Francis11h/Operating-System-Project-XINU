/* setdev.c - setdev */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <lab0.h>
/*------------------------------------------------------------------------
 *  setdev  -  set the two device entries in the process table entry
 *------------------------------------------------------------------------
 */
SYSCALL	setdev(int pid, int dev1, int dev2)
{
	unsigned long start = ctr1000;
	if(isTracing) {
		isCalled[currpid] = TRUE;
		proc_syscall_count[currpid][SETDEV]++;
	}

	short	*nxtdev;

	if (isbadpid(pid)) {

		if(isTracing) {
			proc_syscall_time[currpid][SETDEV] = ctr1000 - start + proc_syscall_time[currpid][SETDEV];
		}

		return(SYSERR);
	}

	nxtdev = (short *) proctab[pid].pdevs;
	*nxtdev++ = dev1;
	*nxtdev = dev2;

	if(isTracing) {
		proc_syscall_time[currpid][SETDEV] = ctr1000 - start + proc_syscall_time[currpid][SETDEV];
	}
	
	return(OK);
}
