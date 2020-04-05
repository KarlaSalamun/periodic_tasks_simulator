#include <algorithm>
#include <cstdio>
#include "Scheduler.h"

void Scheduler::schedule_next( std::vector<Task *> &ready, Task *&running, double time )
{
	// printf( "priorities: " );
	for( size_t i=0; i<ready.size(); i++ ) {
		ready[i]->update_priority( time );
		// printf( "%d: %f\n", ready[i]->id, ready[i]->priority );
	}
	// printf( "\n" );

	std::sort( ready.begin(), ready.end(), 
		[]( const Task *a, const Task *b ) -> bool { return a->get_priority() < b->get_priority(); });

	if( running ) {
		if ( preempt( ready[0], running ) ) {
			printf( "task %d is preempted, ", running->get_id() );
			running->isPreempted = true;
			printf( "remaining time %f\n", running->get_remaining() );
			std::swap( ready[0], running );
		}
	}
	else {
		running = std::move( ready[0] );
		ready.erase( ready.begin() );
	}

	printf( "instance %d of task %d is running!\n", running->get_instance() - 1, running->get_id() );
}

bool Scheduler::preempt( const Task *t1, const Task *t2 )
{
	return t1->get_priority() < t2->get_priority();
}
