#include<stdio.h>

unsigned long	*esp;
unsigned long	*ebp;

void printtos() {
	asm("movl %esp,esp");
	asm("movl %ebp,ebp");

	int i = 0;
	kprintf("\nvoid printtos()\n");
	kprintf("Before[0x%08x]: 0x%08x\n", ebp + 2, *(ebp + 2));
	kprintf("After[0x%08x]: 0x%08x\n", ebp, *ebp);
	
	while (i < 4 && esp < ebp) {
		kprintf("\telement[0x%08x] :0x%08x\n", esp + i, *(esp + i));
		i++;
	}
}