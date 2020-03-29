//
// Created by karla on 09. 03. 2020..
//

#include <random>
#include <cstdio>
#include "TaskCreator.h"

void TaskCreator::create_test_set( std::vector<Task *> &test_tasks )
{
    FILE *fp = fopen( "task_set.txt", "w+" );
    fprintf(fp, "id\tphase\tperiod\tdur\trel_due_date\n" );
    test_tasks.resize( task_number );
    for( int i=0; i<test_tasks.size(); i++ ) {
        test_tasks[i] = std::move( new Task() );
        test_tasks[i]->phase = static_cast<double>( 0.5 * (rand() % 2) + ( rand() % 3 ) ); // 1 to 10
        test_tasks[i]->period = static_cast<double>( 1 + rand() % 5 ); // 1 to 10
        test_tasks[i]->instance = 1;
        test_tasks[i]->duration = 0.5 * test_tasks[i]->period;
        test_tasks[i]->rel_due_date = 0.9 * test_tasks[i]->period;
        test_tasks[i]->id = i;
        fprintf(fp, "%d\t%.2f\t%.2f\t%.2f\t%.2f\n", 
            test_tasks[i]->id, test_tasks[i]->phase, test_tasks[i]->period, test_tasks[i]->duration, test_tasks[i]->rel_due_date );
    }
    fclose(fp);
}

void TaskCreator::write_tasks( std::vector<Task *>  &test_tasks )
{
    FILE *fd = fopen( "../task_set.txt", "w+" );
    for( auto & element: test_tasks ) {
        fprintf( fd, "%f %f %f %f\n", element->phase, element->period, element->duration, element->rel_due_date );
    }
    fclose(fd);
}
/*
void TaskCreator::load_tasks( std::vector<Task *> &test_tasks )
{
    test_tasks.assign( task_number, 0 );
    FILE *fd = fopen( "../task_set.txt", "r" );
    fseek(fd, 0L, SEEK_SET);
    for( int i=0; i<test_tasks.size(); i++ ) {
        test_tasks[i] = std::move( new Task() );
        test_tasks[i]->time_started = 0;
        int retval = fscanf( fd, "%lf %lf %d", &test_tasks[i]->due_date, &test_tasks[i]->weight, &test_tasks[i]->duration );
    }
    fclose(fd);
}
*/