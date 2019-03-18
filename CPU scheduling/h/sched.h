#define AGESCHED 1 
#define LINUXSCHED 2
#define DEFAULTSCHED 3

void setschedclass(int sched_class);
int getschedclass();
int defaultsched();  
int agesched();
int increasePriority();
int linuxsched();
int isnewepoch();
int newepoch();
int getnext();
