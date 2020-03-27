#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <cstdio>
#include <vector>
#include "Task.h"

class Scheduler {
public:
	Scheduler() {}
	void schedule_next( std::vector<Task *> &pending, std::vector<Task *> &ready, Task *&running, double time );
	void set_start( Task *&task, double time );
	void set_end( Task *&task, double time );
	bool preempt( const Task *t1, const Task *t2 );
};

#endif