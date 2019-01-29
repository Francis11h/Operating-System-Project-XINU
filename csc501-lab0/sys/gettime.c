/* gettime.c - gettime */

#include <conf.h>
#include <kernel.h>
#include <date.h>
#include <proc.h>
#include <lab0.h>

extern int getutim(unsigned long *);

/*------------------------------------------------------------------------
 *  gettime  -  get local time in seconds past Jan 1, 1970
 *------------------------------------------------------------------------
 */
SYSCALL	gettime(long *timvar)
{
	//counts and start time;
	unsigned long start = ctr1000;
	if (isTracing) {
		isCalled[currpid] = TRUE;
		proc_syscall_count[currpid][GETTIME]++;
	}

    /* long	now; */

	/* FIXME -- no getutim */

	//end time
	if (isTracing) {
		proc_syscall_time[currpid][GETTIME] = ctr1000 - start + proc_syscall_time[currpid][GETTIME];
	}

    return OK;
}
