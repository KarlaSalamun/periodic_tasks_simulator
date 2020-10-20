#include <random>
#include "UunifastCreator.h"

void UunifastCreator::create_test_set( std::vector<Task *> &test_tasks )
{
    double duration;
    double period;
    test_tasks.resize( task_number );
    mean_u = overload_factor;
    std::vector<double> u_values = generate_utils();
//    convert( u_values );
    std::vector<double> T_values = generate_log_uniform();

    for( size_t i=0; i<test_tasks.size(); i++ ) {
    	period = T_values[i];
    	double phase = 0;
//        convert( u_values );
    	duration = floor( u_values[i] * T_values[i] );
    	if( duration == 0 ) {
    	    duration = 1;
    	}
        test_tasks[i] = new Task( phase, 1, period, period, i, time_slice, duration );
        test_tasks[i]->set_weight( 1 );
        test_tasks[i]->initialize_task();
    }
}

std::vector<double> UunifastCreator::generate_utils()
{
    std::vector<double> result(task_number);
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist(0, 1);
    double sum_u = mean_u;
    
    for( int i=0; i<task_number-1; i++ ) {
        double next_sum_u = sum_u * pow( dist( e2 ), static_cast<double>( 1. / ( task_number-i ) ) );
        next_sum_u = std::floor(next_sum_u * 10.0) / 10.0;        // round to 2 decimals
        result[i] = sum_u - next_sum_u;
        // assert( result[i]!=0 );
        sum_u = next_sum_u;
    }

    result[task_number-1] = sum_u;
    return result;
}

std::vector<double> UunifastCreator::generate_log_uniform()
{
    std::vector<double> result(task_number);
    std::vector<double> r_factor(task_number);
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist( log( lim_d ), log( lim_u + granulation ) );

    for( int i=0; i<task_number; i++ ) {
        r_factor[i] = dist(e2);
        result[i] = floor( exp( r_factor[i] ) / granulation ) * granulation;
    }
    return result;
}

void UunifastCreator::convert( std::vector<double> &values )
{
    for( auto & element : values ) {
        element = round( element * 10 );
    }
}

void TaskCreator::compute_hyperperiod(std::vector<Task *> &tasks)
{
    std::vector<int> periods;
    for( auto & element : tasks ) {
        periods.push_back( static_cast<int>( element->get_period() ) );
    }
    hyperperiod = std::accumulate(periods.begin(), periods.end(), 1,
                                  [](int a, int b) -> int {
                                      return abs( a * b) / std::__gcd(a, b);
                                  });
}

void UunifastCreator::compute_overloaded( std::vector<Task *> &test_tasks, std::vector<double> init )
{
    for( size_t i =0; i<test_tasks.size(); i++ ) {
        double duration = static_cast<int>( init[i] * overload_factor * 100 );
        duration /= 100.0;
        test_tasks[i]->set_duration( duration );
    }
}
