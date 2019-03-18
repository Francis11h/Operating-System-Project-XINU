#include <kernel.h>
#include <lock.h>
#include <proc.h>
#include <sleep.h>

int lock(int ldes1, int type, int priority) {
    STATWORD ps;
    disable(ps);

    int signature;

    signature = ldes1 % MAXSIG;
    ldes1 = ldes1 / MAXSIG;

    if(isbadldes(ldes1) || (locktab[ldes1].lstate == LFREE)) {
        kprintf("This lock(%d) descriptor is invalid!\n", ldes1);
        restore(ps);
        return SYSERR;
    }

    if(signature != locktab[ldes1].lsignature) {
        kprintf("This lock is staled.\n");
        restore(ps);
        return SYSERR;
    }

    if(locktab[ldes1].lstate != LCREATED) {
        if((locktab[ldes1].lstate == READ) && (type == READ) && (priority > get_maxlprio(locktab[ldes1].lhead))) {
            locktab[ldes1].lstate = type;
            locktab[ldes1].nreaders += 1;
            proctab[currpid].lversion = -1;
            proctab[currpid].waitinglock = -1;
            proctab[currpid].pholders[ldes1] = 1;  
            locktab[ldes1].lholders[currpid] = 1;
            restore(ps);
            return OK;
        }
        else {
            proctab[currpid].lversion = locktab[ldes1].lsignature;
            lq_insert(currpid, locktab[ldes1].lhead, priority, type, ctr1000);
            prioinheritance(ldes1);
            proctab[currpid].pstate = PRLWAIT;
            proctab[currpid].waitinglock = ldes1;
            resched();
        }
    }

    if((proctab[currpid].lversion != -1) && 
    (proctab[currpid].lversion != locktab[ldes1].lsignature)) {
        kprintf("This lock(%d) requested by this process(%d) is deleted!\n", ldes1, currpid);
        restore(ps);
        return SYSERR;
    }

    locktab[ldes1].lstate = type;
    if(type == READ) {
        locktab[ldes1].nreaders += 1;
    }
    proctab[currpid].lversion = -1;
    proctab[currpid].waitinglock = -1;
    proctab[currpid].pholders[ldes1] = 1;  
    locktab[ldes1].lholders[currpid] = 1;

    restore(ps);
    return OK;
}

int prioinheritance(int ldes1) {
    int maxpprio = get_maxpprio(locktab[ldes1].lhead);
    int i;

    locktab[ldes1].lqmaxpprio = maxpprio;
    
    for(i = 0; i < NPROC; i++) {
        if(locktab[ldes1].lholders[i] > 0) {
            if(proctab[i].pprio < maxpprio) {
                proctab[i].pinh = maxpprio;
            }
            else {
                proctab[i].pinh = 0;
            }

            if(proctab[i].waitinglock != -1) {
                prioinheritance(proctab[i].waitinglock);
            }
        }
    }

    return OK;
}
