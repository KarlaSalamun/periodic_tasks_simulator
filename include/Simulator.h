#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <cstdio>

#include "Task.h"
#include "TaskCreator.h"
#include "Scheduler.h"

struct task_ctx {
    Task *task;
    std::vector<Task *> pending;
    std::vector<Task *> ready;
};

class Dummy {
public:
    void execute( void * ) {}
};

template <typename T>
class Simulator {
public:
	Simulator( double time_slice, double finish_time, TaskCreator *tc, Scheduler *sched, bool GPSched ) :
		time_slice( time_slice ), finish_time( finish_time ), tc( tc ), sched( sched ), GPScheduling( GPSched )
	{
		tc->set_time_slice( time_slice );
		abs_time = 0;
		missed = 0;
	}
	double get_total_tardiness()
    {
	    return total_tardiness;
    }
    void set_heuristic( T heur )
    {
	    this->heuristic = heur;
}
	void run();
	void load();
	void create();
private:
	//TODO: clanovi mogu biti unique_ptr
    struct task_ctx tctx;
	std::vector<Task *> pending;
	std::vector<Task *> ready;
	double time_slice;
	double finish_time;
	TaskCreator *tc;
	Scheduler *sched;
	double abs_time;
	int missed;
	double total_tardiness;
	T heuristic;
	bool GPScheduling;

	void initialize_tasks()
	{
		for( auto & element : pending ) {
			element->initialize_task();
		}
	}
};

#endif