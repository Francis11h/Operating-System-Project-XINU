#include<sched.h>
#include<kernel.h>
#include<proc.h>

int scheduling_class = 3;

void setschedclass(int sched_class) {
	scheduling_class = sched_class;
	resched();
}

int getschedclass() {
	return scheduling_class;
}