#include <cstdio>
#include "Task.h"
#include "TaskCreator.h"
#include "UunifastCreator.h"
#include "Simulator.h"
#include "Simulator.tpp"

int main()
{
	TaskCreator *tc = new UunifastCreator( 3, "./test_tasks.txt", false, 1, 10, 1, 1.5 );
	Scheduler *sched = new Scheduler();
	Simulator<Dummy *> *sim = new Simulator<Dummy *>( 0.01, 10, tc, sched, false, false );
	sim->set_heuristic( std::make_pair(nullptr, nullptr) );
	sim->create();
	sim->run();
	return 0;
}