#include <stdio.h>
#include <conf.h>
#include <kernel.h>
#include <proc.h>


unsigned long *esp;

void printprocstks(int priority) {
	int i;
	struct pentry *process;

    kprintf("\nvoid printprocstks(int priority)\n");

	for (i = 0; i < NPROC; i++) {
		process = &proctab[i];

		if (process->pstate != PRFREE && process->pprio > priority) {
			if (process->pstate == PRCURR) {
				asm("movl %esp, esp");
			} else {
				esp = (unsigned long *)process->pesp;
			}			
				kprintf("Process [%s]\n", process->pname);
				kprintf("\tpid: %d\n", i);
				kprintf("\tpriority: %d\n", process->pprio);
				kprintf("\tbase: 0x%08x\n", process->pbase);
				kprintf("\tlimit: 0x%08x\n", process->plimit);
				kprintf("\tlen: %d\n", process->pstklen);
				kprintf("\tpesp: 0x%08x\n", esp);
		}
	}	
}
