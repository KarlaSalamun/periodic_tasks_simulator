#pragma once

#include <vector>
#include "Task.h"

class Scheduler {
public:
	Scheduler() {}
	void schedule_next( std::vector<Task *> &ready, Task *&running, double time );
private:
	bool preempt( const Task *t1, const Task *t2 );
};