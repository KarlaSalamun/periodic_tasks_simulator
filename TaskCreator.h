//
// Created by karla on 09. 03. 2020..
//

#ifndef TASKCREATOR_H
#define TASKCREATOR_H

#include <vector>
#include "Task.h"

class TaskCreator {
    public:
        TaskCreator( int task_number ) :
            task_number( task_number ) {}

        TaskCreator() {}

        void set_task_number( int task_number )
        {
            task_number = task_number;
        }
        void set_dd_range( double dd_range )
        {
            dd_range = dd_range;
        }
        void set_dd_tightness( double dd_tightness )
        {
            dd_tightness = dd_tightness;
        }

        void create_test_set( std::vector<Task *> &test_tasks );
        void write_tasks( std::vector<Task *>  &test_tasks );
        void load_tasks( std::vector<Task *> &test_tasks );
        void UUnifast_create_tasks( std::vector<Task *> &test_tasks );
        std::vector<double> UUnifast_generate_u( int n, double mean_u );
        std::vector<double> generate_log_uniform( int n, double lim_u, double lim_d, int granularity );

        int task_number;
};


#endif 
