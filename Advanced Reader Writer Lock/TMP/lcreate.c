#include <kernel.h>
#include <lock.h>
#include <proc.h>

SYSCALL lcreate() {
    STATWORD ps;
    disable(ps);

    int i;
    int ret;
    for(i = 0; i < NLOCKS; i++) {
        if(locktab[i].lstate == LFREE) {
            locktab[i].lstate = LCREATED;
            locktab[i].lsignature += 1;
            if(locktab[i].lsignature == MAXSIG) {
                locktab[i].lsignature = 0;
            }
            //kprintf("create: %d\n", i * 100 + locktab[i].lsignature);
            restore(ps);
            return i * MAXSIG + locktab[i].lsignature;
        }
    }

    kprintf("No locks available!\n");

    restore(ps);
    return SYSERR; 
}