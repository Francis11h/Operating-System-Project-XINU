/* sleep1000.c - sleep1000 */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 * sleep1000 --  delay the caller for a time specified in 1/100 of seconds
 *------------------------------------------------------------------------
 */
SYSCALL sleep1000(int n)
{
	unsigned long start = ctr1000;
	if(isTracing) {
		isCalled[currpid] = TRUE;
		proc_syscall_count[currpid][SLEEP1000]++;
	}

	STATWORD ps;    

	if (n < 0  || clkruns==0) {

		if(isTracing) {
			proc_syscall_time[currpid][SLEEP1000] = ctr1000 - start + proc_syscall_time[currpid][SLEEP1000];
		}
	    return(SYSERR);
	}
	disable(ps);
	if (n == 0) {		/* sleep1000(0) -> end time slice */
	        ;
	} else {
		insertd(currpid,clockq,n);
		slnempty = TRUE;
		sltop = &q[q[clockq].qnext].qkey;
		proctab[currpid].pstate = PRSLEEP;
	}
	resched();
    restore(ps);
    
    if(isTracing) {
		proc_syscall_time[currpid][SLEEP1000] = ctr1000 - start + proc_syscall_time[currpid][SLEEP1000];
	}

	return(OK);
}
