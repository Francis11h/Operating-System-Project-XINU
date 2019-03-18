/* resched.c  -  resched */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <stdio.h>
#include <sched.h>

unsigned long currSP;	/* REAL sp of current process */
extern int ctxsw(int, int, int, int);

/*-----------------------------------------------------------------------
 * resched  --  reschedule processor to highest priority ready process
 *
 * Notes:	Upon entry, currpid gives current process id.
 *		Proctab[currpid].pstate gives correct NEXT state for
 *			current process if other than PRREADY.
 *------------------------------------------------------------------------
 */

int resched() {
	int sched_class = getschedclass();
	if (sched_class == 1) {
		return agesched();
	}
	if (sched_class == 2) {
		return linuxsched();
	}
	if (sched_class == 3) {
		return defaultsched();
	}
	return SYSERR;
}




int isnewepoch() {
	int i;
	for (i = 0; i <NPROC; i++) {	//if any proc in ready queue has left quantum, this epoch will not end
		if ((proctab[i].pstate == PRREADY)&&(proctab[i].counter > 0)) {
			return FALSE;
		}
	}
	return TRUE;
}

int newepoch() {
	int i;
	//for all processes, recalculate the quantum（counter）
	for (i = 0; i < NPROC; i++) {
		if (proctab[i].pstate != PRFREE) { //not used up
			proctab[i].goodness = proctab[i].pprio + proctab[i].counter;    //goodness = counter + priority
			proctab[i].counter = proctab[i].pprio + proctab[i].counter/2;	//quantum = floor(counter/2) + priority
		}
	}
	return TRUE; //TRUE : 1
}

int getnext() {	//find the maxgoodness process's index
	int i;
	int next = 0;
	int maxgoodness = 0;
	for (i = 0; i < NPROC; i++) {
		if ((proctab[i].pstate == PRREADY)&&(proctab[i].goodness > maxgoodness)) {
			next = i;
			maxgoodness = proctab[i].goodness;
		}
	}
	return next;
}


int linuxsched() {
	register struct	pentry	*optr;	/* pointer to old process entry */
	register struct	pentry	*nptr;	/* pointer to new process entry */
	int next;

	optr = &proctab[currpid];
	optr->counter = preempt;

	if (optr->counter == 0) {  //used all quantuam  
		optr->goodness = 0;    //can't be scheduled in this epoch
	}

	if (isnewepoch()) {
		if ((optr->pstate == PRCURR)&&(optr->counter) > 0) {
			#ifdef	RTCLOCK
				preempt = optr->counter;		/* reset preemption counter, is preempted when it reaches zero*/
			#endif
			return OK;
		}
		newepoch();
	}

	next = getnext();

	if (optr->pstate == PRCURR) {
		if (next == currpid) {
			#ifdef	RTCLOCK
				preempt = optr->counter;		/* reset preemption counter, is preempted when it reaches zero*/
			#endif
			return OK;
		}
		optr->pstate = PRREADY;
		insert(currpid, rdyhead, optr->pprio);
	}

	nptr = &proctab[(currpid = dequeue(next))];
	nptr->pstate = PRCURR;

	if (currpid == 0) { //XINU has a null process (pid = 0) that runs whenever there is no other user process present in the ready queue
		#ifdef RTCLOCK
			preempt = 1;
		#endif
	} else {
		#ifdef RTCLOCK
			preempt = nptr->counter;
		#endif
	}

	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
	return OK;

}

int increasePriority() {
	int prev;
	prev = q[rdytail].qprev;

	while (prev < NPROC) {
		q[prev].qkey++;
		prev = q[prev].qprev;
	}
	return TRUE;
}

int agesched() {

	register struct	pentry	*optr;	/* pointer to old process entry */
	register struct	pentry	*nptr;	/* pointer to new process entry */

	/* no switch needed if current process priority higher than next*/
	optr= &proctab[currpid];
	// every time call the resched(), increment the priority of proc in ready queue by 1
	increasePriority();

	if ((optr->pstate == PRCURR) &&(lastkey(rdytail)<optr->pprio)) {
		return(OK);
	}
	
	/* force context switch */

	if (optr->pstate == PRCURR) {
		optr->pstate = PRREADY;
		insert(currpid,rdyhead,optr->pprio);
	}

	/* remove highest priority process at end of ready list */

	nptr = &proctab[ (currpid = getlast(rdytail)) ];
	nptr->pstate = PRCURR;		/* mark it currently running	*/
		#ifdef	RTCLOCK
			preempt = QUANTUM;		/* reset preemption counter	*/
		#endif
	
	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
	
	/* The OLD process returns here when resumed. */
	return OK;
}


int defaultsched()
{
	register struct	pentry	*optr;	/* pointer to old process entry */
	register struct	pentry	*nptr;	/* pointer to new process entry */

	/* no switch needed if current process priority higher than next*/

	if ( ( (optr= &proctab[currpid])->pstate == PRCURR) &&
	   (lastkey(rdytail)<optr->pprio)) {
		return(OK);
	}
	
	/* force context switch */

	if (optr->pstate == PRCURR) {
		optr->pstate = PRREADY;
		insert(currpid,rdyhead,optr->pprio);
	}

	/* remove highest priority process at end of ready list */

	nptr = &proctab[ (currpid = getlast(rdytail)) ];
	nptr->pstate = PRCURR;		/* mark it currently running	*/
		#ifdef	RTCLOCK
			preempt = QUANTUM;		/* reset preemption counter	*/
		#endif
	
	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
	
	/* The OLD process returns here when resumed. */
	return OK;
}




