//
// Created by karla on 05. 04. 2020..
//
#include <random>
#include "StaticCreator.h"

void StaticCreator::create_static_tasks( std::vector<Task *> &test_tasks )
{
    double sum_1 = 0;
    double sum_2 = 0;

    std::vector<double> durations( test_tasks.size() );
    std::vector<double> weights( test_tasks.size() );

    test_tasks.resize( task_number );
    for( size_t i=0; i<test_tasks.size(); i++ ) {
        durations[i] = static_cast<double>( 1 + rand() % 100 );
        weights[i] = static_cast<double>( 1 + rand() % 100 ) / 100.;
        sum_1 += durations[i] * ( 1 - dd_tightness - dd_range/2. );
        sum_2 += durations[i] * ( 1 - dd_tightness + dd_range/2. );
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis( sum_1, sum_2 );

    for( size_t i=0; i<test_tasks.size(); i++ ) {
        double due_date = dis(gen);
        test_tasks[i] = std::move( new Task( weights[i], durations[i], due_date ) );
        test_tasks[i]->set_time_started(0);
    }
}