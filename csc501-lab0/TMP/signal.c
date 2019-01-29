/* signal.c - signal */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 * signal  --  signal a semaphore, releasing one waiting process
 *------------------------------------------------------------------------
 */
SYSCALL signal(int sem)
{
	unsigned long start = ctr1000;
	if(isTracing) {
		isCalled[currpid] = TRUE;
		proc_syscall_count[currpid][SIGNAL]++;
	}

	STATWORD ps;    
	register struct	sentry	*sptr;

	disable(ps);
	if (isbadsem(sem) || (sptr= &semaph[sem])->sstate==SFREE) {
		restore(ps);

		if(isTracing) {
			proc_syscall_time[currpid][SIGNAL] = ctr1000 - start + proc_syscall_time[currpid][SIGNAL];
		}

		return(SYSERR);
	}
	if ((sptr->semcnt++) < 0)
		ready(getfirst(sptr->sqhead), RESCHYES);
	restore(ps);

	if(isTracing) {
		proc_syscall_time[currpid][SIGNAL] = ctr1000 - start + proc_syscall_time[currpid][SIGNAL];
	}

	return(OK);
}
