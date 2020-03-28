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
			// update_params( running, abs_time ) ;
		}
		abs_time += time_slice;

		printf( "time: %f\n", abs_time );
/*
		for( std::vector<Task *>::iterator it = ready.begin(); it != ready.end(); it++ ) {
			if( (*it)->instance * (*it)->period < abs_time ) {
				printf("missed task %d\n", (*it)->id);
				missed++;
				ready.erase( it );
			}
		}
*/
		if( running ) {
			// update_params( running, abs_time );
			if( running->isFinished( abs_time ) ) {
				printf( "task %d is finished!\n", running->id );
				pending.push_back( std::move( running ) );
				running = nullptr;
			}
		}
		
		for( auto & element : pending ) {
			update_params( element, abs_time );
		}
		
		for( auto & element : pending ) {
			update_params( element, abs_time );
		}
		

	}
}

void Simulator::update_params( Task *&task, double time )
{
	task->update_tardiness( time );
	task->set_arrival_time();
	task->set_abs_dd();
}
