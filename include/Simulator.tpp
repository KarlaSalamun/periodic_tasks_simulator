#include <cmath>
#include <math.h>
#include <cassert>
#include "Simulator.h"

template <typename T>
void Simulator<T>::load()
{
	tc->load_tasks( pending );
	initialize_tasks();
}

template <typename T>
void Simulator<T>::create()
{
	tc->create_test_set( pending );
	initialize_tasks();
	for( auto & element : pending ) {
	    element->set_weight( 1 );
	}
	tc->write_tasks( pending );
}

template <typename T>
void Simulator<T>::run()
{
    double tmp_idle = 0;
    double start_idle = 0;
    missed = 0;
    total_tardiness = 0;
    int all_tasks = 0;
	//TODO: budući da running uvijek pokazuje samo na jedan objekt može biti unique_ptr
	Task *running = nullptr;				// TODO: ovo je leak
	abs_time = 0;
	while( abs_time < finish_time ) {
		std::vector<Task *>::iterator it;
/*
        if( running ) {
            if( abs_time == running->get_abs_due_date() and
                std::isgreater( running->get_remaining(), 0  )  ) {
                running->inc_instance();
                running->update_params();
                running->reset_remaining();
                pending.push_back( std::move( running ) );
                running = nullptr;
//                printf( "MISS\n" );
                missed++;
            }
        }
        */

        it = ready.begin();
        while( it != ready.end() ) {
            if ((*it)->is_next_instance(abs_time)) {
                (*it)->inc_instance();
                (*it)->update_params();
                (*it)->reset_remaining();
                pending.push_back(std::move(*it));
                it = ready.erase(it);
                missed++;
            } else {
                it++;
            }
        }

        it = pending.begin();
        while( it != pending.end() ) {

			assert( (*it)->get_period() );
			assert( (*it)->get_abs_due_date() > 0 );
			
			if ( (*it)->isReady( abs_time ) ) {
			    all_tasks++;
//				printf( "task %d is ready!\n", (*it)->get_id() );
				ready.push_back( std::move( *it ) );
				it = pending.erase( it );
			}
			else {
				it++;
			}
		}

        for( auto & element : pending ) {
            assert( element->get_arrival_time() > abs_time );
        }

        if( !running ) {
            if( !idle ) {
                start_idle = abs_time;
                idle = true;
//                printf("IDLE\n");
            }
            else {
                tmp_idle += time_slice;
//                printf("idle: %f\n", tmp_idle);
            }
        }
        else {
            if( isgreater(fabs( tmp_idle ), 0) ) {
                idle_time_vector.push_back( tmp_idle );
                deadline_vector.push_back( start_idle );
                tmp_idle = 0;
                idle = false;
            }
        }

		if( !ready.empty() ) {
//			printf( "scheduling tasks : " );
//			for( auto & element : ready ) {
//				printf( "%d ", element->get_id() );
//			}
//			printf( "\n" );

			if( GPScheduling ) {
			    std::vector<Task *> tmp_pending;
			    std::vector<Task *> tmp_processed;
                tctx.processed.clear();

//                std::copy( ready.begin(), ready.end(), std::back_inserter( tctx.pending ) );
                it = ready.begin();
                while( it != ready.end() ) {
                    tctx.pending.clear();
                    Task *tmp = new Task( *it );
                    tctx.task = tmp;
                    it = ready.erase( it );
                    std::copy( ready.begin(), ready.end(), std::back_inserter( tctx.pending ) );
                    heuristic->execute( &tctx );
                    tctx.processed.push_back( tmp );
                }
                std::copy( tctx.processed.begin(), tctx.processed.end(), std::back_inserter( ready ) );

                /*
                for( auto & element : ready ) {         // TODO: staviti pointer na vektore u tctx
                    tctx.task = element;
                    heuristic->execute( &tctx );
                    element->set_priority( tctx.task->get_priority() );
                }
                 */
			}
            else { // EDF scheduling
                for( size_t i=0; i<ready.size(); i++ ) {
                    ready[i]->update_priority( abs_time );
                    // printf( "%d: %f\n", ready[i]->id, ready[i]->priority );
                }
            }
			sched->schedule_next( ready, running, abs_time );
		}

		abs_time += time_slice;
//		if( running )
//            printf( "task %d is running, %f remaining\n", running->get_id(), running->get_remaining() );

//        printf( "\ntime: %f\n\n", abs_time );

		if( running ) {
		    idle = false;
			if( running->isFinished() ) {
//				printf( "task %d is finished!\n", running->get_id() );
				running->update_tardiness( abs_time );
				running->reset_remaining();
				running->inc_instance();
				running->update_params();
				running->isPreempted = false;
//				if( islessequal( running->get_arrival_time(), abs_time ) ) {
//				    ready.push_back( std::move( running ) );
//				}
//				else {
                    pending.push_back( std::move( running ) );
//				}
				running = nullptr;
			}
			else {
				running->update_remaining();
//				printf("remaining time: %f\n", running->get_remaining() );
			}
		}
	}

	if( idle ) {
	    deadline_vector.push_back( start_idle );
	    idle_time_vector.push_back( tmp_idle + time_slice );
	}

	for( auto & element : pending ) {
//		printf( "%d tardiness: %f\n", element->get_id(), element->get_tardiness() );
		total_tardiness += element->get_tardiness();
	}
	for( auto & element : ready ) {
//		printf( "%d tardiness: %f\n", element->get_id(), element->get_tardiness() );
        total_tardiness += element->get_tardiness();
        if( element->get_arrival_time() < abs_time ) {
            missed++;
        }
	}
	if( running ) {
//		printf( "%d tardiness: %f\n", running->get_id(), running->get_tardiness() );
        total_tardiness += running->get_tardiness();
        if( abs_time == running->get_abs_due_date() and
            std::isgreater( running->get_remaining(), 0  )  ) {
            missed++;
        }
	}

//	printf("missed tasks: %d\n", missed);
}

