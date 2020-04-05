#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <cstdio>

#include "Task.h"
#include "TaskCreator.h"
#include "Scheduler.h"

class Simulator {
public:
	//TODO: time_slice treba poslati dalje u task tako da zna napraviti update
	Simulator( double time_slice, double finish_time, TaskCreator *tc, Scheduler *sched ) : 
		time_slice( time_slice ), finish_time( finish_time ), tc( tc ), sched( sched )
	{
		tc->set_time_slice( time_slice );
		abs_time = 0;
		missed = 0;
	}
	void run();
	void load();
	void create();
private:
	//TODO: clanovi mogu biti unique_ptr
	std::vector<Task *> pending;
	std::vector<Task *> ready;
	double time_slice;
	double finish_time;
	TaskCreator *tc;
	Scheduler *sched;
	double abs_time;
	int missed;

	void initialize_tasks()
	{
		for( auto & element : pending ) {
			element->initialize_task();
		}
	}
};

#endif