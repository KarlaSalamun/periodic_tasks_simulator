#include <algorithm>
#include <cstdio>
#include "Scheduler.h"

struct {
    bool operator()(Task *a, Task *b) const
    {
        return a->abs_due_date < b->abs_due_date;
    }
} customLess;

void Scheduler::schedule_next( std::vector<Task *> &pending, std::vector<Task *> &ready, Task *&running, double time )
{
	std::sort( ready.begin(), ready.end(), customLess );

	if( running ) {
		if ( preempt( ready[0], running ) ) {
			printf( "task %d is preempted, ", running->id );
			running->isPreempted = true;
			running->set_remaining( time );
			printf( "remaining time %f\n", running->remaining );
			std::swap( ready[0], running );
			set_start( running, time );
		}
	}	
	else {
		running = std::move( ready[0] );
		set_start( running, time );
		// running->set_remaining( time );
		ready.erase( ready.begin() );
	}
	
	printf( "task %d is running!\n", running->id );
}

void Scheduler::set_start( Task *&task, double time )
{
	task->time_started = time;
}

void Scheduler::set_end( Task *&task, double time )
{
	task->time_ended = time;
}

bool Scheduler::preempt( const Task *t1, const Task *t2 )
{
	return t1->abs_due_date < t2->abs_due_date;
}
