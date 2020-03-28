//
// Created by karla on 06. 03. 2020..
//
#include <cmath>
#include <math.h>
#include "Task.h"

double Task::compute_tardiness()
{
	time_ended = time_started + duration;
    return time_ended > abs_due_date ? time_ended-abs_due_date : 0;
}

void Task::set_arrival_time()
{
	arrival_time = phase + ( instance - 1 ) * period;
}

void Task::set_abs_dd()
{
	abs_due_date = phase + ( instance - 1 ) * period + rel_due_date;
}

void Task::update_params()
{
	arrival_time = phase + ( instance - 1 ) * period;
	abs_due_date = phase + ( instance - 1 ) * period;
}

bool Task::isReady( double time ) 
{
	return fabs( time - arrival_time ) < 0.001;
}

bool Task::isFinished( double time ) 
{
	return isgreaterequal( time,  time_started + duration );
}

void Task::update_tardiness( double time )
{
	if( time > abs_due_date ) {
		tardiness += abs_due_date - time;
	}
}

void Task::inc_instance()
{
	instance++;
}

void Task::set_remaining( double time )
{
	remaining = time_started + duration - time;
}