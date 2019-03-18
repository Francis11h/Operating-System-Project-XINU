/* lock.h - for locks */

#ifndef _LOCK_H_
#define _LOCK_H_

#include "proc.h"

#define NLOCKS      50
#define NLQENT      NLOCKS * 2 + NPROC
#define MAXSIG      100     

#define LFREE       0
#define READ        1
#define WRITE       2
#define LCREATED    3



// this entry is for lock to store the waiting processes
struct l_qent {
    int lqprio;
    int lqtype;
    unsigned long lqtime;
    int lqprev;
    int lqnext;
};

struct lentry {
    int lstate;         // LFREE or LCREATED or READ or WRITE
    int lsignature;     // each lock has a signature to mark its version
    int nreaders;       // record number of readers holding this lock
    int lhead;          // head of waiting queue
    int lqmaxpprio;       // maxpprio in the waiting queue
    // to record processes hold this lock, 0 not hold, >0 hold.
    int lholders[NPROC];
};

extern struct lentry locktab[];
extern int nextlqueue;
extern struct l_qent lq[];
extern unsigned long ctr1000;

// fucntions for locks
SYSCALL linit();
SYSCALL lcreate();
SYSCALL ldelete(int lockdescriptor);
SYSCALL lock(int ldes1, int type, int priority);
SYSCALL releaseall(int numlocks, long args, ...);
int isbadldes(int ldes);
int prioinheritance(int ldes1);

// functions for lock queue
int lq_newqueue();
int lq_isempty(int head);
int lq_insert(int proc, int head, int lprio, int type, unsigned long currtime);
int get_maxlprio(int head);
int get_maxpprio(int head);

#endif