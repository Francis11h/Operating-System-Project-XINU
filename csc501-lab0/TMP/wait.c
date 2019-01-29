/* wait.c - wait */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 * wait  --  make current process wait on a semaphore
 *------------------------------------------------------------------------
 */
SYSCALL	wait(int sem)
{
	unsigned long start = ctr1000;
	if(isTracing) {
		isCalled[currpid] = TRUE;
		proc_syscall_count[currpid][WAIT]++;
	}

	STATWORD ps;    
	struct	sentry	*sptr;
	struct	pentry	*pptr;

	disable(ps);
	if (isbadsem(sem) || (sptr= &semaph[sem])->sstate==SFREE) {
		restore(ps);

		if(isTracing) {
			proc_syscall_time[currpid][WAIT] = ctr1000 - start + proc_syscall_time[currpid][WAIT];
		}

		return(SYSERR);
	}
	
	if (--(sptr->semcnt) < 0) {
		(pptr = &proctab[currpid])->pstate = PRWAIT;
		pptr->psem = sem;
		enqueue(currpid,sptr->sqtail);
		pptr->pwaitret = OK;
		resched();
		restore(ps);

		if(isTracing) {
			proc_syscall_time[currpid][WAIT] = ctr1000 - start + proc_syscall_time[currpid][WAIT];
		}

		return pptr->pwaitret;
	}
	restore(ps);

	if(isTracing) {
		proc_syscall_time[currpid][WAIT] = ctr1000 - start + proc_syscall_time[currpid][WAIT];
	}

	return(OK);
}
