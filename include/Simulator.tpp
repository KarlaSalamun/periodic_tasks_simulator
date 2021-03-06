#pragma once

#include <cmath>
#include <math.h>
#include <cassert>
#include "Simulator.h"

static int compare_members(const void *m1, const void *m2)
{
    if (*(double*)m1 > *(double*)m2) { // TODO ne smije se castati const u non-const
        return -1;
    }

    else if( (*(double*)m1 < *(double*)m2) ) {
        return 1;
    }

    else {
        return  0;
    }
}

static int compare_factors(const void *m1, const void *m2)
{
    if (*(int*)m1 > *(int*)m2) {
        return -1;
    }

    else if( (*(int*)m1 < *(int*)m2) ) {
        return 1;
    }

    else {
        return  0;
    }
}

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
    completed = 0;
    total_tardiness = 0;
    all_tasks = 0;
	abs_time = 0;
	wasted_time = 0;
	running = nullptr;
	ready.clear();

	initialize_tasks();

    FILE *fd;
    if( display_sched ) {
        fd = fopen( "../schedule.tex", "w+" );
        fprintf( fd, "\\begin{figure}[ht]\n" );
        fprintf( fd, "\\begin{RTGrid}[width=10cm]{%lu}{%d}\n", pending.size(), static_cast<int>(finish_time) );
        for( auto & element : pending ) {
            fprintf( fd, "\\multido{\\n=0+%d}{%d}{\n", static_cast<int>(element->get_period()), static_cast<int>( finish_time / element->get_period() ) );
            fprintf( fd, "\t\\TaskArrDead{%d}{\\n}{%d}\n", element->get_id() + 1, static_cast<int>(element->get_period()) );
            fprintf( fd, "}\n" );
        }
    }

	for( auto & element : pending ) {
	    assert( element->get_phase() == 0 );
	    element->initialize_task();
	    element->set_max_instances( finish_time / element->get_period() );
	}

	while( abs_time < finish_time ) {
		std::vector<Task *>::iterator it;

		// abort running task if about to miss deadline
        if( running ) {
            assert(running->get_phase() == 0);
            if( abs_time == running->get_abs_due_date() and
                std::isgreater( running->get_remaining(), 0  )  ) {
                wasted_time += running->get_duration() - running->get_remaining();
                running->inc_instance();
                running->update_params();
                running->inc_missed();
                running->reset_remaining();
                running->skip_factors.push_back( running->get_curr_skip_value() );
                running->reset_skip_value();
                if( display_sched ) {
                    fprintf( fd, "{%d}\n", static_cast<int>(abs_time) );
                }
                pending.push_back( std::move( running ) );
                running = nullptr;
                missed++;
            }
        }

        it = ready.begin();
        while( it != ready.end() ) {
            assert( (*it)->get_phase() == 0 );
            if ((*it)->is_next_instance(abs_time)) {
                (*it)->inc_instance();
                (*it)->update_params();
                (*it)->reset_remaining();
                (*it)->inc_missed();
                (*it)->skip_factors.push_back( (*it)->get_curr_skip_value() );
                (*it)->reset_skip_value();
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
                ( *it )->inc_released();
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

                it = ready.begin();
                while( it != ready.end() ) {
                    double priority;
                    it = ready.erase( it );
                    update_state( *it, ready );
                    heuristic->execute( &priority );
                    (*it)->set_priority( priority );
                    ready.push_back( *it );
                    it++;
                }
                assert( ready[0]->get_phase() == 0 );
			}
            else { // EDF scheduling
                for( size_t i=0; i<ready.size(); i++ ) {
                    ready[i]->update_priority( abs_time );
                    // printf( "%d: %f\n", ready[i]->id, ready[i]->priority );
                }
            }

            assert( ready.size() <= 6 );

            if(running) {
                assert(running->get_phase() == 0);
            }

            assert( ready[0]->get_phase() == 0 );
            assert( ready.size() <= 6  && ready.size() != 0);
			sched->schedule_next( ready, running, abs_time );
            assert( ready[0]->get_phase() == 0 );
            assert(running->get_phase() == 0);
            if( display_sched ) {
                if( sched->preempted ) {
                    fprintf( fd, "{%d}\n", static_cast<int>(abs_time) );
                    fprintf( fd, "\\TaskExecution{%d}{%d}", running->get_id() + 1, static_cast<int>(abs_time) );
                }
                if( sched->taskStarted ) {
                    fprintf( fd, "\\TaskExecution{%d}{%d}", running->get_id() + 1, static_cast<int>(abs_time) );
                }
            }
        }

		abs_time += time_slice;
//		if( running )
//            printf( "task %d is running, %f remaining\n", running->get_id(), running->get_remaining() );

//        printf( "\ntime: %f\n\n", abs_time );

		if( running ) {
            assert(running->get_phase() == 0);
		    idle = false;
			if( running->isFinished() ) {
                if( display_sched ) {
                    fprintf( fd, "{%d}\n", static_cast<int>(abs_time) );
                }
//				printf( "task %d is finished!\n", running->get_id() );
				running->update_tardiness( abs_time );
				running->reset_remaining();
				running->inc_completed();
				completed++;
				running->inc_instance();
				running->update_params();
				running->inc_skip_value();
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
		element->skip_factors.push_back( element->get_curr_skip_value() );
    }
	for( auto & element : ready ) {
//		printf( "%d tardiness: %f\n", element->get_id(), element->get_tardiness() );
        total_tardiness += element->get_tardiness();
        if( element->get_arrival_time() < abs_time ) {
            missed++;
            element->inc_missed();
            element->skip_factors.push_back( element->get_curr_skip_value() );
        }
	}
	if( running ) {
//		printf( "%d tardiness: %f\n", running->get_id(), running->get_tardiness() );
        total_tardiness += running->get_tardiness();
        if( abs_time == running->get_abs_due_date() and
            std::isgreater( running->get_remaining(), 0  )  ) {
            running->skip_factors.push_back( running->get_curr_skip_value() );
            running->inc_missed();
            missed++;
        }
	}


	qos = static_cast<double>( completed ) / static_cast<double>( all_tasks );
    if( running ) {
        pending.push_back( std::move( running ) );
    }
    if( !ready.empty() ) {
        std::copy( ready.begin(), ready.end(), std::back_inserter( pending ) );
    }
    if( display_sched ) {
        fprintf( fd, "{%d}\n", static_cast<int>(abs_time) );
        fprintf( fd, "\\end{RTGrid}\n" );
        fprintf( fd, "\\end{figure}\n" );
        fclose(fd);
    }

}

template <typename T>
double Simulator<T>::compute_skip_fitness()
{
    double sum = 0;
    int tasks = 0;
    for( auto & element : pending ) {
        qsort( element->skip_factors.data(), element->skip_factors.size(), sizeof(int), compare_factors );
        sum += element->get_weight() * element->compute_mean_skip_factor() / ( element->skip_factors[0] * element->skip_factors.size() );
        tasks++;
    }
    assert( sum / static_cast<double>( tasks ) <= 1 );
    return sum / static_cast<double>( tasks );
}

template <typename T>
double Simulator<T>::compute_deviation()
{
    double dev = 0;
    std::vector<double> factors;

    for( auto & element : pending ) {
        factors.push_back( element->compute_mean_skip_factor()/ static_cast<double>( element->get_max_instances() ));
    }
    qsort( factors.data(), factors.size(), sizeof(double), compare_members );

    dev = ( factors[0] - factors[factors.size() - 1] ) / factors.size();

    assert( dev >= 0 );

    return dev;
}

template <typename T>
void Simulator<T>::display_info()
{
    double sum = 0;
    int tasks = 0;
    for( auto & element : pending ) {
        qsort( element->skip_factors.data(), element->skip_factors.size(), sizeof(int), compare_factors );
        sum += element->get_weight() * element->compute_mean_skip_factor();
        tasks++;
    }
    mean_skip_factor = sum / static_cast<double>( tasks );

    printf( "mean skip factor:\t%lf\n", mean_skip_factor );

    double dev = 0;
    std::vector<double> factors;

    for( auto & element : pending ) {
        factors.push_back( element->compute_mean_skip_factor() / static_cast<double>( element->get_max_instances() ));
    }
    qsort( factors.data(), factors.size(), sizeof(double), compare_members );

    dev = ( factors[0] - factors[factors.size() - 1] ) / factors.size();

    printf( "mean deviation:\t%lf\n", dev );
    printf( "QoS:\t%lf\n", static_cast<double>( this->completed ) / static_cast<double>( this->all_tasks ) );
}

template <typename T>
void Simulator<T>::compute_mean_skip_factor()
{
    double sum = 0;
    int tasks = 0;
    for( auto & element : pending ) {
        qsort( element->skip_factors.data(), element->skip_factors.size(), sizeof(int), compare_factors );
        sum += element->compute_mean_skip_factor();
        assert( sum != 0 );
        tasks++;
    }
    mean_skip_factor = sum / static_cast<double>( tasks );
}

template <typename T>
double Simulator<T>::compute_gini_coeff()
{
    double sum = 0;
    for( size_t  i=0; i<pending.size(); i++) {
        for( size_t  j=0; j<pending.size(); j++) {
//            if( pending[i]->compute_mean_skip_factor() == 0 ) {
//                return 1;
//            }
            sum += fabs( pending[i]->compute_mean_skip_factor() - pending[j]->compute_mean_skip_factor() );
        }
    }
    assert(sum == sum);
    compute_mean_skip_factor();
    assert( mean_skip_factor > 0 );
    sum /= ( 2 * pow(static_cast<double>(pending.size()), 2 ) * mean_skip_factor );
    assert( sum == sum );
    return sum;
}

template <typename T>
void Simulator<T>::update_state( Task *current, std::vector<Task *> ready_tasks ) {
    double due_date = static_cast<double>(current->get_abs_due_date());
    double remaining_no = static_cast<double>(ready_tasks.size());
    double spr = 0;
    double sd = 0;
    for( auto & element : ready_tasks ) {
        spr += static_cast<double>(element->get_duration());
        sd += static_cast<double>(element->get_abs_due_date());
    }
    double pt = static_cast<double>(current->get_duration());
    double sl = static_cast<double>(std::max( current->get_abs_due_date() - current->get_duration() - current->get_time_started(), 0));
    double w = static_cast<double>(current->get_weight());
    double rt = static_cast<double>(current->get_remaining());
    double skip = static_cast<double>(current->get_curr_skip_value());
    double QoS = static_cast<double>(current->get_completed()) / static_cast<double>(current->get_released());

    heuristic->setTerminalValue( "dd", &due_date);
    heuristic->setTerminalValue( "nr", &remaining_no );
    heuristic->setTerminalValue( "spr", &spr );
    heuristic->setTerminalValue( "pt", &pt );
    heuristic->setTerminalValue( "sd", &sd );
    heuristic->setTerminalValue( "sl", &sl );
    heuristic->setTerminalValue( "w", &w );
    heuristic->setTerminalValue( "rt", &rt );
    heuristic->setTerminalValue( "skip", &skip );
    heuristic->setTerminalValue( "QoS", &QoS );
}