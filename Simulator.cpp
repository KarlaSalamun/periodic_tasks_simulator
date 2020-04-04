#include <cmath>
#include <math.h>
#include <cassert>
#include "Simulator.h"

void Simulator::load()
{
	tc->load_tasks( pending );
	initialize_tasks();
}

void Simulator::create()
{
	tc->create_test_set( pending );
	initialize_tasks();
	tc->write_tasks( pending );
}

void Simulator::run()
{
	//TODO: budući da running uvijek pokazuje samo na jedan objekt može biti unique_ptr
	Task *running = nullptr;				// TODO: ovo je leak
	while( abs_time < finish_time ) {
		std::vector<Task *>::iterator it = pending.begin();

		while( it != pending.end() ) {

			assert( (*it)->get_period() );
			assert( (*it)->get_abs_due_date() > 0 );
			
			if ( (*it)->isReady( abs_time ) ) {
				(*it)->inc_instance();
				printf( "task %d is ready!\n", (*it)->get_id() );
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
				printf( "%d ", element->get_id() );
			}
			printf( "\n" );
			sched->schedule_next( ready, running, abs_time );
		}

		it = ready.begin();
		while( it != ready.end() ) {
			// next arrival time is less than current time -> missed task, new instance appeared
			if( (*it)->is_missed( abs_time + time_slice ) ) {
				printf("Missed task %d\n", (*it)->get_id() );
				pending.push_back( *it );
				it = ready.erase( it );
				missed++;
			}
			else {
				it++;
			}
		}

		abs_time += time_slice;

		printf( "\ntime: %f\n\n", abs_time );

		if( running ) {
			if( running->isFinished() ) {
				printf( "task %d is finished!\n", running->get_id() );
				running->update_tardiness( abs_time );
				pending.push_back( std::move( running ) );
				running = nullptr;
			}
			else {
				running->update_remaining();
				printf("remaining time: %f\n", running->get_remaining() );
			}
		}
		
		for( auto & element : pending ) {
			if( element->is_next_instance( abs_time ) ) {
				element->update_params();
			}
		}
	}

	for( auto & element : pending ){
		printf( "%d tardiness: %f\n", element->get_id(), element->get_tardiness() );
	}
	for( auto & element : ready ){
		printf( "%d tardiness: %f\n", element->get_id(), element->get_tardiness() );
	}
	if( running ) {
		printf( "%d tardiness: %f\n", running->get_id(), running->get_tardiness() );	
	}
	
	printf("missed tasks: %d\n", missed);
}

