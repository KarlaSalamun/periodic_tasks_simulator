//
// Created by karla on 05. 04. 2020..
//

#ifndef GP_SCHEDULING_STATICCREATOR_H
#define GP_SCHEDULING_STATICCREATOR_H

#include <vector>
#include "Task.h"
#include "TaskCreator.h"

class StaticCreator : public TaskCreator {
public:
    StaticCreator(  double dd_range, double dd_tightness ) :
             dd_range( dd_range ), dd_tightness( dd_tightness ) {}

    StaticCreator() = default;

    void set_dd_range( double dd_range )
    {
        this->dd_range = dd_range;
    }
    void set_dd_tightness( double dd_tightness )
    {
        this->dd_tightness = dd_tightness;
    }

    void create_static_tasks( std::vector<Task *> &test_tasks );

private:
    std::vector<double> UUnifast_generate_u( int n, double mean_u );
    std::vector<double> generate_log_uniform( int n, double lim_u, double lim_d, int granularity );
    double dd_range;
    double dd_tightness;


};

#endif //GP_SCHEDULING_STATICCREATOR_H
