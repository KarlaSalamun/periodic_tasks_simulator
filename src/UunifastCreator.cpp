#include <random>
#include "UunifastCreator.h"

void UunifastCreator::create_test_set( std::vector<Task *> &test_tasks )
{
    test_tasks.resize( task_number );

    std::vector<double> u_values = generate_utils();
    std::vector<double> T_values = generate_log_uniform();

    for( size_t i=0; i<test_tasks.size(); i++ ) {
    	double period = T_values[i];
    	double phase = 0;
    	double duration = u_values[i] * T_values[i] * overload_factor;
        if( duration > period ) {
            duration = period;
        }
        printf( "%f\n", duration );
        test_tasks[i] = std::move( new Task( phase, 1, period, period, i, time_slice, duration ) );
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