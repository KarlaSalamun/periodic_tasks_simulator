//
// Created by karla on 09. 03. 2020..
//

#include <random>
#include <cstdio>
#include <cassert>
#include "TaskCreator.h"

void TaskCreator::create_test_set( std::vector<Task *> &test_tasks )
{
    FILE *fp = fopen( "task_set.txt", "w+" );
    fprintf(fp, "id\tphase\tperiod\tdur\trel_due_date\n" );
    test_tasks.resize( 2 );
    // for( int i=0; i<test_tasks.size(); i++ ) {
    //     test_tasks[i] = std::move( new Task() );
    //     // test_tasks[i]->phase = static_cast<double>( 0.5 * (rand() % 2) + ( rand() % 3 ) ); // 1 to 10
    //     test_tasks[i]->period = static_cast<double>( 1 + rand() % 5 ); // 1 to 10
    //     test_tasks[i]->instance = 1;
    //     test_tasks[i]->duration = 0.5 * test_tasks[i]->period;
    //     test_tasks[i]->rel_due_date = 0.9 * test_tasks[i]->period;
    //     test_tasks[i]->id = i;
        
    //     fprintf(fp, "%d\t%.2f\t%.2f\t%.2f\t%.2f\n", 
    //         test_tasks[i]->id, test_tasks[i]->phase, test_tasks[i]->period, test_tasks[i]->duration, test_tasks[i]->rel_due_date );
    // }
    for( int i=0; i<2; i++ ) {
        test_tasks[i] = std::move( new Task() );
    }
    test_tasks[0]->period = 7;
    test_tasks[1]->period = 5;

    test_tasks[0]->duration = 4;
    test_tasks[1]->duration = 2;

    for( int i=0; i<2; i++ ) {
        test_tasks[i]->phase = 0;
        test_tasks[i]->rel_due_date = test_tasks[i]->period;
        test_tasks[i]->id = i;
        test_tasks[i]->instance = 1;
    }
    fclose(fp);
}

void TaskCreator::write_tasks( std::vector<Task *>  &test_tasks )
{
    FILE *fd = fopen( "../task_set.txt", "w+" );
    for( auto & element: test_tasks ) {
        fprintf( fd, "%d %f %f \n", element->id, element->period, element->duration );
    }
    fclose(fd);
}

void TaskCreator::load_tasks( std::vector<Task *>  &test_tasks )
{
    //primi ovaj path preko konstruktora
    FILE *fd = fopen( "../task_set.txt", "r" );

    test_tasks.assign( task_number, 0 );

    for( auto & element: test_tasks ) {
        Task *curr_task = new Task();
        //TODO: ovdje treba pozvati konstruktor od Task ili neka init metoda koja prima fd
        curr_task->time_started = 0;
        fscanf( fd, "%d", &curr_task->id );
        fscanf( fd, "%lf", &curr_task->period );
        fscanf( fd, "%lf", &curr_task->duration );
        curr_task->instance = 1;
        curr_task->rel_due_date = curr_task->period;
        curr_task->remaining = curr_task->duration;
        element  = curr_task;
    }
    fclose(fd);
}

//TODO: instancirati novi razred koji nasljeđuje TaskCreator i tamo implementirati ovo
void TaskCreator::UUnifast_create_tasks( std::vector<Task *> &test_tasks )
{
    FILE *fp = fopen( "task_set.txt", "w+" );
    fprintf(fp, "id\tphase\tutil\tperiod\tdur\trel_due_date\n" );
    test_tasks.resize( task_number );
    double util=0;

    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist_dd(0.5, 1);

    std::vector<double> u_values = UUnifast_generate_u( test_tasks.size(), 1 );
    std::vector<double> T_values = generate_log_uniform( test_tasks.size(), 10, 1, 1 );

    for( int i=0; i<test_tasks.size(); i++ ) {
        test_tasks[i] = std::move( new Task );
        test_tasks[i]->period = T_values[i];
        // test_tasks[i]->phase = static_cast<double>( 0.5 * (rand() % 2) + ( rand() % 3 ) ); // 1 to 10
        test_tasks[i]->phase = 0;
        test_tasks[i]->instance = 1;
        // assert( u_values[i]!=0 );
        test_tasks[i]->duration = u_values[i] * test_tasks[i]->period;
        // test_tasks[i]->rel_due_date = test_tasks[i]->period * dist_dd(e2);
        test_tasks[i]->rel_due_date = test_tasks[i]->period;
        test_tasks[i]->id = i;
        test_tasks[i]->remaining = test_tasks[i]->duration;
        util += test_tasks[i]->duration / test_tasks[i]->period;
        printf( "Task %d : %f %f\n", test_tasks[i]->id, test_tasks[i]->duration, test_tasks[i]->period );
        fprintf(fp, "%d\t%.2f\t%.3f\t%.2f\t%.2f\t%.2f\n", 
            test_tasks[i]->id, test_tasks[i]->phase, u_values[i], test_tasks[i]->period, test_tasks[i]->duration, test_tasks[i]->rel_due_date );
    }
    printf( "util: %f\n", util );
    fclose(fp);
}

std::vector<double> TaskCreator::UUnifast_generate_u( int n, double mean_u )
{
    std::vector<double> result(n);
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist(0, 1);
    double sum_u = mean_u;
    double util;
    
    for( int i=0; i<n-1; i++ ) {
        double next_sum_u = sum_u * pow( dist( e2 ), static_cast<double>( 1. / ( n-i ) ) );
        next_sum_u = std::floor(next_sum_u * 10.0) / 10.0;        // round to 2 decimals
        result[i] = sum_u - next_sum_u;
        // assert( result[i]!=0 );
        sum_u = next_sum_u;
        util += result[i];
    }

    result[n-1] = sum_u;
    util += result[n-1];
    printf( "util :%f\n", util );
    return result;
}

std::vector<double> TaskCreator::generate_log_uniform( int n, double lim_u, double lim_d, int granularity )
{
    std::vector<double> result(n);
    std::vector<double> r_factor(n);
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist( log( lim_d ), log( lim_u + granularity ) );

    for( int i=0; i<n; i++ ) {
        r_factor[i] = dist(e2);
        result[i] = floor( exp( r_factor[i] ) / granularity ) * granularity;
    }
    return result;
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