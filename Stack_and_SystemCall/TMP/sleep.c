/* sleep.c - sleep */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 * sleep  --  delay the calling process n seconds
 *------------------------------------------------------------------------
 */
SYSCALL	sleep(int n)
{
	unsigned long start = ctr1000;
	if(isTracing) {
		isCalled[currpid] = TRUE;
		proc_syscall_count[currpid][SLEEP]++;
	}

	STATWORD ps;    
	if (n<0 || clkruns==0) {

		if(isTracing) {
			proc_syscall_time[currpid][SLEEP] = ctr1000 - start + proc_syscall_time[currpid][SLEEP];
		}

		return(SYSERR);
	}
	
	if (n == 0) {
	        disable(ps);
		resched();
		restore(ps);

		if(isTracing) {
			proc_syscall_time[currpid][SLEEP] = ctr1000 - start + proc_syscall_time[currpid][SLEEP];
		}

		return(OK);
	}
	while (n >= 1000) {
		sleep10(10000);
		n -= 1000;
	}
	if (n > 0)
		sleep10(10*n);

	if(isTracing) {
		proc_syscall_time[currpid][SLEEP] = ctr1000 - start + proc_syscall_time[currpid][SLEEP];
	}

	return(OK);
}
