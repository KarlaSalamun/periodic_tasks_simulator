//
// Created by karla on 06. 03. 2020..
//
#include <cmath>
#include <cstdio>
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
	// if( isPreempted ) {
	// 	if( isgreaterequal( time+0.00001, time_started + remaining ) ) {
	// 		isPreempted = false;
	// 		return true;
	// 	}
	// }
	// return isgreaterequal( time+0.00001,  time_started + duration );

	//TODO: 0.1 je timeslice koristi varijablu
	return islessequal( fabs(remaining), 0.1 );
}

void Task::update_tardiness( double time )
{
	if( time > abs_due_date ) {
		tardiness += time - abs_due_date;
	}
}

void Task::inc_instance()
{
	instance++;
}

//TODO: izbrisati
void Task::set_remaining( double time )
{
	remaining = time_started + duration - time;
}

void Task::update_remaining()
{
	remaining -= 0.1;			// TODO ovo je timeslice i primi kroz konstruktor!
}