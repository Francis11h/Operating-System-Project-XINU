#include <kernel.h>
#include <lock.h>
#include <proc.h>

SYSCALL ldelete(int lockdescriptor) {
    STATWORD ps;
    disable(ps);

    lockdescriptor = lockdescriptor / 100;

    int isdeleted = 0;
    int i;
    int firstproc;

    if(isbadldes(lockdescriptor) || locktab[lockdescriptor].lstate == LFREE) {
        kprintf("Delete failed. Lock %d is not created.\n", lockdescriptor);
        restore(ps);
        return SYSERR;
    }

    if(locktab[lockdescriptor].lstate != LCREATED) {
        kprintf("Delete Warning: Lock %d is still being locked.\n", lockdescriptor);
        isdeleted++;
    }
    locktab[lockdescriptor].lstate = LFREE;
    locktab[lockdescriptor].nreaders = 0;
    locktab[lockdescriptor].lqmaxpprio = 0;
    for(i = 0; i < NPROC; i++) {
        locktab[lockdescriptor].lholders[i] = 0;
    }

    if(!lq_isempty(locktab[lockdescriptor].lhead)) {
        kprintf("Delete Warning: There are processes waiting in the queue of lock %d\n", lockdescriptor);
        isdeleted++;
        firstproc = get_firstproc(locktab[lockdescriptor].lhead);
        while(firstproc > NPROC-1) {
            lq_dequeue(firstproc);
            ready(firstproc, RESCHNO);
            firstproc = get_firstproc(locktab[lockdescriptor].lhead);
        }
        resched();
    }

    if(isdeleted == 0) {
        restore(ps);
        return OK;
    }
    else {
        restore(ps);
        return DELETED;
    }    
}