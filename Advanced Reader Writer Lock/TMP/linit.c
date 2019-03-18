#include <kernel.h>
#include <lock.h>
#include <proc.h>

struct lentry locktab[NLOCKS];
int nextlqueue = NPROC;
struct l_qent lq[NLQENT];

SYSCALL linit() {
    STATWORD ps;
    disable(ps);

    int i, j;
    for(i = 0; i < NLOCKS; i++) {
        locktab[i].lstate = LFREE;
        locktab[i].lsignature = -1;
        locktab[i].nreaders = 0;
        locktab[i].lhead = lq_newqueue();
        locktab[i].lqmaxpprio = 0;
        for(j = 0; j < NPROC; j++) {
            locktab[i].lholders[j] = 0;
        }
    }

    restore(ps);
    return OK;
}