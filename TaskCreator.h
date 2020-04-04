//
// Created by karla on 09. 03. 2020..
//

#ifndef TASKCREATOR_H
#define TASKCREATOR_H

#include <vector>
#include <string>
#include "Task.h"

class TaskCreator {
    public:
        TaskCreator( int task_number, std::string filename ) :
            task_number( task_number ), filename( filename ) {}

        TaskCreator() = default;

        void set_task_number( int task_number )
        {
            this->task_number = task_number;
        }
        // TODO ovo treba tek kad spojim repo sa statickim poslovima
        void set_dd_range( double dd_range )
        {
            this->dd_range = dd_range;
        }
        void set_dd_tightness( double dd_tightness )
        {
            this->dd_tightness = dd_tightness;
        }
        void set_time_slice( double time_slice )
        {
            this->time_slice = time_slice;
        }

        virtual void create_test_set( std::vector<Task *> &test_tasks );
        void write_tasks( std::vector<Task *>  &test_tasks );
        void load_tasks( std::vector<Task *> &test_tasks );

    protected:
        int task_number;
        double time_slice;
        
    private:
        std::string filename;
        double dd_range;
        double dd_tightness;
};

#endif 
