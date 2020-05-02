//
// Created by karla on 09. 03. 2020..
//
#include <cstdio>
#include <cassert>
#include <random>
#include "TaskCreator.h"

void TaskCreator::create_test_set( std::vector<Task *> &test_tasks )
{
    test_tasks.resize( task_number );
    for( size_t i=0; i<test_tasks.size(); i++ ) {
        double phase = static_cast<double>( 0.5 * (rand() % 2) + ( rand() % 3 ) ); // 1 to 10
        double period = static_cast<double>( 1 + rand() % 5 ); // 1 to 10
        double duration = 0.5 * period;
        double rel_due_date = 0.9 * period;
        test_tasks[i] = std::move( new Task( phase, 1, period, rel_due_date, i, time_slice, duration ) );
    }
}

void TaskCreator::write_tasks( std::vector<Task *>  &test_tasks )
{
    FILE *fd = fopen( filename.c_str(), "w+" );
    assert(fd);
    for( auto & element: test_tasks ) {
        element->write_task(fd);
    }
    fclose(fd);
}

void TaskCreator::load_tasks( std::vector<Task *>  &test_tasks )
{
    FILE *fd = fopen( filename.c_str(), "r+" );
    assert(fd);
    test_tasks.assign( task_number, 0 );

    for( size_t i=0; i<test_tasks.size(); i++ ) {
        int id;
        double phase, period, duration;
        int skip_factor;

        fscanf( fd, "%d %lf %lf %lf %d", &id, &phase, &period, &duration, &skip_factor );
        // TODO treba li mi ovdje move
        test_tasks[i] = new Task( phase, 1, period, period, i, time_slice, duration );
        test_tasks[i]->set_time_slice( this->time_slice );
        if( skippable ) {
            test_tasks[i]->set_skip_factor( skip_factor );
        }
    }
    fclose(fd);
}


