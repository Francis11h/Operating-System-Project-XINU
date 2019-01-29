/* getprio.c - getprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 * getprio -- return the scheduling priority of a given process
 *------------------------------------------------------------------------
 */
SYSCALL getprio(int pid)
{
	//counts and start time;
	unsigned long start = ctr1000;
	if (isTracing) {
		isCalled[currpid] = TRUE;
		proc_syscall_count[currpid][GETPRIO]++;
	}


	STATWORD ps;    
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid) || (pptr = &proctab[pid])->pstate == PRFREE) {
		restore(ps);
		return(SYSERR);
	}
	restore(ps);

	//end time
	if (isTracing) {
		proc_syscall_time[currpid][GETPRIO] = ctr1000 - start + proc_syscall_time[currpid][GETPRIO];
	}
	
	return(pptr->pprio);
}
