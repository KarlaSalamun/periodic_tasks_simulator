#include <cmath>
#include <math.h>
#include "Simulator.h"

void Simulator::initialize()
{
	TaskCreator *tc = new TaskCreator( 3 );
	tc->create_test_set( pending );
	for( auto & element : pending ) {
		element->arrival_time = element->phase;
		element->set_abs_dd();
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

		it = ready.begin();
		while( it != ready.end() ) {
			// next arrival time is less than current time -> missed task, new instance appeared
			if( islessequal( (*it)->phase + ( (*it)->instance - 1 ) * (*it)->period, abs_time + time_slice ) ) {
				printf("Missed task %d\n", (*it)->id );
				pending.push_back( *it );
				it = ready.erase( it );
				// update_params( (*it), abs_time + time_slice );
				missed++;
			}
			else {
				it++;
			}
		}

		abs_time += time_slice;



		printf( "\ntime: %f\n\n", abs_time );


		

		if( running ) {
			// update_params( running, abs_time );
			if( running->isFinished( abs_time ) ) {
				printf( "task %d is finished!\n", running->id );
				running->update_tardiness( abs_time );
				pending.push_back( std::move( running ) );
				running = nullptr;
			}
		}
		
		for( auto & element : pending ) {
			if( element->phase + element->instance * element->period >= abs_time ) {
				update_params( element, abs_time );	
			}
			
		}
		
		// for( auto & element : ready ) {
		// 	update_params( element, abs_time );
		// }
		

	}
	for( auto & element : pending ){
		printf( "%d tardiness: %f\n", element->id, element->tardiness );
	}
	for( auto & element : ready ){
		printf( "%d tardiness: %f\n", element->id, element->tardiness );
	}
	if( running ) {
		printf( "%d tardiness: %f\n", running->id, running->tardiness );	
	}
	

}

void Simulator::update_params( Task *&task, double time )
{
	// task->update_tardiness( time );
	task->set_arrival_time();
	task->set_abs_dd();
}
