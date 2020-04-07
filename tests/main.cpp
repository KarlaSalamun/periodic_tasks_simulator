#include <cstdio>
#include "Task.h"
#include "TaskCreator.h"
#include "UunifastCreator.h"
#include "Simulator.h"

int main()
{
	TaskCreator *tc = new UunifastCreator( 3, "./test_tasks.txt", 1, 10, 1, 1.5 );
	Scheduler *sched = new Scheduler();
	Simulator *sim = new Simulator( 0.1, 10, tc, sched );
	sim->create();
	sim->run();
	return 0;
}