#include <algorithm>
#include <cstdio>
#include "Scheduler.h"

struct {
    bool operator()(Task *a, Task *b) const
    {
        return a->priority < b->priority;
    }
} customLess;

void Scheduler::schedule_next( std::vector<Task *> &pending, std::vector<Task *> &ready, Task *&running, double time )
{
	// printf( "priorities: " );
	for( int i=0; i<ready.size(); i++ ) {
		ready[i]->priority = ready[i]->abs_due_date - time;
		// printf( "%d: %f\n", ready[i]->id, ready[i]->priority );
	}
	// printf( "\n" );

	std::sort( ready.begin(), ready.end(), customLess );

	if( running ) {
		if ( preempt( ready[0], running ) ) {
			printf( "task %d is preempted, ", running->id );
			running->isPreempted = true;
			// running->set_remaining( time );
			printf( "remaining time %f\n", running->remaining );
			std::swap( ready[0], running );
			set_start( running, time );
		}
	}	
	else {
		running = std::move( ready[0] );
		if( !running->isPreempted ) {
			set_start( running, time );
		}
		// running->update_remaining();
		ready.erase( ready.begin() );
	}

	printf( "instance %d of task %d is running!\n", running->instance - 1, running->id );
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
	return t1->priority < t2->priority;
}
