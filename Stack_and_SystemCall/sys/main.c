/* user.c - main */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>
#include <lab0.h>

/*------------------------------------------------------------------------
 *  main  --  user main program
 *------------------------------------------------------------------------
 */
int prX;
void halt();

prch(c)
char c;
{
	int i;
	sleep(5);	
}

int main()
{
	kprintf("\n\nHello World, Xinu lives\n\n");

	//Task 1
	long testIn=0xaabbccdd;
	long testOut = zfunction(testIn);
	kprintf("Test input is %x\n", testIn);
	kprintf("Test output is %x\n", testOut);

	//Task 2
	printsegaddress();

	//Task 3
	printtos();

	//Task 4
	printprocstks(-1);

	//Task 5
	syscallsummary_start();        
	resume(prX = create(prch,2000,20,"proc X",1,'A'));
	sleep(10);
	syscallsummary_stop();
	printsyscallsummary();

	return 0;
}
