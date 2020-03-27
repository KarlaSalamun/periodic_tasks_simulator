#include <cmath>
#include "Simulator.h"

void Simulator::initialize()
{
	TaskCreator *tc = new TaskCreator( 2 );
	tc->create_test_set( pending );
	for( auto & element : pending ) {
		element->arrival_time = element->phase;
		// printf( "%d: arrival %f\n", element->id, element->arrival_time );
	}
}

void Simulator::run()
{
	Scheduler *sched = new Scheduler();
	Task *running = nullptr;				// TODO: ovo je leak
	while( abs_time < finish_time ) {
		printf( "time: %f\n", abs_time );
		int i=0;
		std::vector<Task *>::iterator it = pending.begin();

		while( it != pending.end() ) {
			if ( (*it)->isReady( abs_time ) ) {
				(*it)->inc_instance();
				printf( "task %d is ready!\n", (*it)->id );
				ready.push_back( std::move( *it ) );
				it = pending.erase( it );
			}
			else {
				it++;
			}
		}
		
		if( !ready.empty() ) {
			printf( "scheduling tasks : " );
			for( auto & element : ready ) {
				printf( "%d ", element->id );
			}
			printf( "\n" );
			sched->schedule_next( pending, ready, running, abs_time );
			// update_params( ready, abs_time ) ;
		}
		abs_time += time_slice;

		for( std::vector<Task *>::iterator it = ready.begin(); it != ready.end(); it++ ) {
			if( (*it)->instance * (*it)->period < abs_time ) {
				printf("missed task %d\n", (*it)->id);
				missed++;
				ready.erase( it );
			}
		}
		
		update_params( pending, abs_time );
		update_params( ready, abs_time );

	}
}

void Simulator::update_params( std::vector<Task *> tasks, double time )
{
	for( auto & element : tasks ) {
		element->update_tardiness( time );
		//element->set_arrival_time();
		element->set_abs_dd();
	}
}
