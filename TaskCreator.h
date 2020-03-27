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

        int task_number;
};


#endif 
