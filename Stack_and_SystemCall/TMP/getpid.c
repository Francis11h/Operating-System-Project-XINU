/* getpid.c - getpid */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 * getpid  --  get the process id of currently executing process
 *------------------------------------------------------------------------
 */
SYSCALL getpid()
{
		//counts and start time;
	unsigned long start = ctr1000;
	if (isTracing) {
		isCalled[currpid] = TRUE;
		proc_syscall_count[currpid][GETPID]++;
	}

	//end time
	if (isTracing) {
		proc_syscall_time[currpid][GETPID] = ctr1000 - start + proc_syscall_time[currpid][GETPID];
	}

	return(currpid);
}
