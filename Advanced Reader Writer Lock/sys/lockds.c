/* some data structure functions for locks */
#include <kernel.h>
#include <lock.h>
#include <proc.h>
#include <q.h>

/* for lock queue  */
int lq_newqueue() {
    struct l_qent *hptr;
	struct l_qent *tptr;
	int hindex, tindex;

    hptr = &lq[ hindex=nextlqueue++]; /* assign and rememeber queue	*/
	tptr = &lq[ tindex=nextlqueue++]; /* index values for head&tail	*/
	hptr->lqnext = tindex;
	hptr->lqprev = EMPTY;
	hptr->lqtype  = EMPTY;
    hptr->lqtime = EMPTY;
    hptr->lqprio = MAXINT;
	tptr->lqnext = EMPTY;
	tptr->lqprev = hindex;
	tptr->lqtype  = EMPTY;
    tptr->lqtime = EMPTY;
    tptr->lqprio = MININT;

	return hindex;
}

int lq_isempty(int head) {
    if(lq[head].lqnext >= NPROC) {
        return 1;
    } 
    return 0;
}

int lq_insert(int proc, int head, int lprio, int type, unsigned long currtime) {
    int curr;
    int prev;

    curr = lq[head].lqnext;
    while(1) {
        if(lprio >= lq[curr].lqprio) {  //>=
            break;
        }
        if((type == WRITE) && (lprio == lq[curr].lqprio) 
        && (lq[curr].lqtype == READ) 
        && (currtime - lq[curr].lqtime < 600)) {
            break;
        }
        curr = lq[curr].lqnext;
    }

    lq[proc].lqprio = lprio;
    lq[proc].lqtype = type;
    lq[proc].lqtime = currtime;
    lq[proc].lqnext = curr;
    lq[proc].lqprev = prev = lq[curr].lqprev;
    lq[prev].lqnext = proc;
    lq[curr].lqprev = proc;

    return OK;
}

int get_maxlprio(int head) {
    int firstproc = lq[head].lqnext;
    return lq[firstproc].lqprio;
}

// get max pprio or pinh
int get_maxpprio(int head) {
    int maxpprio = MININT;
    int curr = lq[head].lqnext;

    while(lq[curr].lqnext != EMPTY) {
        if(proctab[curr].pprio > proctab[curr].pinh) {
            if(proctab[curr].pprio > maxpprio) {
                maxpprio = proctab[curr].pprio;
            }
        }
        else {
            if(proctab[curr].pinh > maxpprio) {
                maxpprio = proctab[curr].pinh;
            }
        }
        curr = lq[curr].lqnext;
    }

    return maxpprio;
}

int get_firstproc(int head) {
    return lq[head].lqnext;
}

int lq_dequeue(int item) {
    struct l_qent *tmp = &lq[item];
    lq[tmp->lqprev].lqnext = tmp->lqnext;
	lq[tmp->lqnext].lqprev = tmp->lqprev;
	return item;
}

int isbadldes(int ldes) {
    if(ldes < 0 || ldes > NLOCKS-1) {
        return 1;
    }
    return 0;
}
