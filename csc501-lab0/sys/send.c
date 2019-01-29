/* send.c - send */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 *  send  --  send a message to another process
 *------------------------------------------------------------------------
 */
SYSCALL	send(int pid, WORD msg)
{
	unsigned long start = ctr1000;
	if(isTracing) {
		isCalled[currpid] = TRUE;
		proc_syscall_count[currpid][SEND]++;
	}

	STATWORD ps;    
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid) || ( (pptr= &proctab[pid])->pstate == PRFREE)
	   || pptr->phasmsg != 0) {
		restore(ps);

		if(isTracing) {
			proc_syscall_time[currpid][SEND] = ctr1000 - start + proc_syscall_time[currpid][SEND];
		}

		return(SYSERR);
	}
	pptr->pmsg = msg;
	pptr->phasmsg = TRUE;
	if (pptr->pstate == PRRECV)	/* if receiver waits, start it	*/
		ready(pid, RESCHYES);
	else if (pptr->pstate == PRTRECV) {
		unsleep(pid);
		ready(pid, RESCHYES);
	}
	restore(ps);

	if(isTracing) {
		proc_syscall_time[currpid][SEND] = ctr1000 - start + proc_syscall_time[currpid][SEND];
	}

	return(OK);
}
