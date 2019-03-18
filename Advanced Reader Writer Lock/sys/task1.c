#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <lock.h>
#include <stdio.h>
#include <lock.h>

#define DEFAULT_LOCK_PRIO 20

void semP1(char msg, int sem) {
    int i, j;
    int count = 0;
    kprintf ("  %c begin.\n", msg);
    kprintf ("  %c being at Critical Section.\n", msg);
    wait(sem);
    kprintf ("  %c finish!!.\n", msg);
    sleep (1);
    // for hold cpu
    for (i = 0; i < 100000; i++) {
        for (j = 0; j < 10000; j++) {
        }
        count += 1;
    }
    signal(sem);
}

void semP3(char msg, int sem) {
    kprintf ("  %c begin.\n", msg);
    kprintf ("  %c being at Critical Section.\n", msg);
    wait(sem);
    kprintf ("  %c finish!!.\n", msg);
    signal(sem);
}

void lock1(char msg, int lck) {
    int i, j;
    int count = 0;
    kprintf ("  %c begin.\n", msg);
    kprintf ("  %c to acquire lock.\n", msg);
    lock (lck, WRITE, DEFAULT_LOCK_PRIO);
    kprintf ("  %c finish!!.\n", msg);
    sleep (1);
    // for hold cpu
    for (i = 0; i < 100000; i++) {
        for (j = 0; j < 10000; j++) {
        }
        count += 1;
    }
    releaseall (1, lck);
}

void lock3(char msg, int lck) {
    kprintf ("  %c begin.\n", msg);
    kprintf ("  %c to acquire lock.\n", msg);
    lock (lck, WRITE, DEFAULT_LOCK_PRIO);
    kprintf ("  %c finish!!.\n", msg);
    releaseall (1, lck);
}

void preemptor(char msg, int lck) {
    int i;

    kprintf ("  %c begin.\n", msg);
    kprintf ("  %c holds cpu and ouputs itself.\n", msg);
    for(i = 0; i < 30; i++) {
        kprintf ("%c ", msg);
    }
    kprintf("\n");
}

void sem_test() {
    int sem0, sem1, sem2;
    int sem;

    kprintf("\nTest by using semaphores. --- BEGIN\n\n");

    sem = screate(1);
    sem0 = create(semP1, 2000, 20, "semP1", 2, 'A', sem);
    sem1 = create(preemptor, 2000, 30, "semP2", 2, 'B', sem);
    sem2 = create(semP3, 2000, 50, "semP3", 2, 'C', sem);

    resume(sem0);
    sleep(1);
    resume(sem2);
    resume(sem1);
    sleep(5);

    sdelete(sem);
}

void lock_test() {
    int lck0, lck1, lck2;
    int lck;

    kprintf("\nTest by using locks. --- BEGIN\n\n");

    lck = lcreate();
    lck0 = create(lock1, 2000, 20, "lock1", 2, 'A', lck);
    lck1 = create(preemptor,  2000, 30, "lock2",  2, 'B', lck);
    lck2 = create(lock3, 2000, 50, "lock3", 2, 'C', lck);

    resume(lck0);
    sleep(1);
    resume(lck2);
    resume(lck1);
    sleep(5);

    ldelete(lck);
}

int main() {
    
    sem_test();
    lock_test();

    shutdown();
    return 0;
}