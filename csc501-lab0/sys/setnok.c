/* setnok.c - setnok */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include <lab0.h>
/*------------------------------------------------------------------------
 *  setnok  -  set next-of-kin (notified at death) for a given process
 *------------------------------------------------------------------------
 */
SYSCALL	setnok(int nok, int pid)
{
	unsigned long start = ctr1000;
	if(isTracing) {
		isCalled[currpid] = TRUE;
		proc_syscall_count[currpid][SETNOK]++;
	}

	STATWORD ps;    
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid)) {
		restore(ps);

		if(isTracing) {
			proc_syscall_time[currpid][SETNOK] = ctr1000 - start + proc_syscall_time[currpid][SETNOK];
		}

		return(SYSERR);
	}
	pptr = &proctab[pid];
	pptr->pnxtkin = nok;
	restore(ps);

	if(isTracing) {
		proc_syscall_time[currpid][SETNOK] = ctr1000 - start + proc_syscall_time[currpid][SETNOK];
	}

	return(OK);
}
