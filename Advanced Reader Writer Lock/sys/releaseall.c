#include <kernel.h>
#include <lock.h>
#include <proc.h>

SYSCALL releaseall(int numlocks, long args, ...) {
    STATWORD ps;
    disable(ps);

    int i;
    int currldes;
    int iswrong = 0;
    for(i = 0; i < numlocks; i++) {
        currldes = (int)*((&args) + i) / MAXSIG;
        if(releaseonelock(currpid, currldes) == SYSERR) {
            iswrong++;
        }
    }
    
    resched();
    if(iswrong > 0) {
        restore(ps);
        return SYSERR;
    }
    else {
        restore(ps);
        return OK;
    }
}

int releaseonelock(int pid, int ldes) {

    int i;
    int maxpprio = MININT;
    int nextproc;

    if(isbadldes(ldes)) {
        kprintf("Relese failed. Lock %d is invalid.\n", ldes);
        return SYSERR;
    }
    
    if((proctab[pid].pholders[ldes] != 1) || (locktab[ldes].lholders[pid] != 1)) {
        kprintf("Release failed. Lock %d is not holded by process %d\n", ldes, pid);
        return SYSERR;
    }

    /* for process entry */
    proctab[pid].pholders[ldes] = 0;   
    proctab[pid].lversion = -1;
    for(i = 0; i < NLOCKS; i++) {
        if(proctab[pid].pholders[i] == 1) {
            if(maxpprio < locktab[i].lqmaxpprio) {
                maxpprio = locktab[i].lqmaxpprio;
            }
        }
    }
    if(maxpprio == MININT) {
        proctab[pid].pinh = 0;
    }
    else {
        proctab[pid].pinh = maxpprio;
    }
    if(proctab[pid].waitinglock != -1) {
        prioinheritance(proctab[pid].waitinglock);
    }
    
    /* for lock entry */
    locktab[ldes].lholders[pid] = 0;
    if(locktab[ldes].lstate == READ) {
        locktab[ldes].nreaders -= 1;
        if(locktab[ldes].nreaders == 0) {
            locktab[ldes].lstate = LCREATED;
            if(lq_isempty(locktab[ldes].lhead) == 0){
                nextproc = get_firstproc(locktab[ldes].lhead);
                lq_dequeue(nextproc);
                prioinheritance(ldes);
                ready(nextproc, RESCHNO);
            }
        }
    }

    if(locktab[ldes].lstate == WRITE) {
        locktab[ldes].lstate = LCREATED;

        
        while(lq_isempty(locktab[ldes].lhead) == 0) {
            nextproc = get_firstproc(locktab[ldes].lhead);
            lq_dequeue(nextproc);
            prioinheritance(ldes);
            ready(nextproc, RESCHNO);
            if(lq[nextproc].lqtype == WRITE) {
                break;
            }
            nextproc = get_firstproc(locktab[ldes].lhead);
            if(lq[nextproc].lqtype == WRITE) {
                break;
            }
        }
    }

    return OK;
}