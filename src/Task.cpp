//
// Created by karla on 06. 03. 2020..
//

#include <cmath>
#include <cstdio>
#include <cassert>
#include "Task.h"

double Task::compute_tardiness( double time )
{
	return time > abs_due_date ? time - abs_due_date : 0;
}

 
void Task::write_task(FILE *fd)
{
	assert(fprintf( fd, "%d %f %f %f \n", id, phase, period, duration ) > 0);
}

void Task::set_arrival_time()
{
	arrival_time = phase + ( instance - 1 ) * period;
}

void Task::set_abs_dd()
{
	abs_due_date = phase + ( instance - 1 ) * period + rel_due_date;
} 

bool Task::isReady( double time ) 
{
	return fabs( time - arrival_time ) < 0.001;
}

bool Task::isFinished() 
{
	return std::islessequal( fabs(remaining), time_slice );
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

void Task::update_remaining()
{
	remaining -= time_slice;
}

// void Task::set_period( double period )
// {
//     this->period = period;
// }

// void Task::set_duration( double duration )
// {
//     this->duration = duration;
// }

// void Task::set_phase( double phase )
// {
//     this->phase = phase;
// }

// void Task::set_instance( int instance )
// {
//     this->instance = instance;
// }

void Task::set_arrival_time( double arrival_time )
{
    this->arrival_time = arrival_time;
}

void Task::set_priority( double priority )
{
	this->priority = priority;
}

void Task::set_tardiness( double tard )
{
	this->tardiness = tard;
}

void Task::set_time_started(double time)
{
    this->time_started = time;
}

void Task::set_state( state_t state )
{
    this->state = state;
}

void Task::set_skip_factor(int factor)
{
    this->skip_factor = factor;
}

// TODO staviti sve gettere const
int Task::get_id()
{
	return this->id;
}

double Task::get_phase()
{
	return this->phase;
}

double Task::get_period()
{
	return this->period;
}

double Task::get_duration()
{
	return this->duration;
}

double Task::get_abs_due_date()
{
	return this->abs_due_date;
}

double Task::get_remaining()
{
	return this->remaining;
}

double Task::get_tardiness()
{
	return this->tardiness;
}

double Task::get_priority() const
{
	return this->priority;
}

int Task::get_instance()
{
	return this->instance;
}

double Task::get_weight()
{
    return this->weight;
}

double Task::get_time_started()
{
    return this->time_started;
}

state_t Task::get_state()
{
    return this->state;
}

void Task::initialize_task() 
{
	arrival_time = phase;
	set_abs_dd();
}

bool Task::is_missed( double time )
{
	return std::islessequal( phase + (instance-1) * period, time );
}

bool Task::is_next_instance( double time )
{
	return std::isgreaterequal( phase + instance * period, time );
}

void Task::update_params()
{
	set_arrival_time();
	set_abs_dd();
}

void Task::update_rb_params()
{
    set_arrival_time();
    set_abs_dd();
    if( this->skip_factor == 0 || this->skip_factor == 1 ) {
        return;
    }
    this->state = ( this->instance % skip_factor == 0 ) ? BLUE : RED;
}

void Task::update_priority( double time )
{
	priority = abs_due_date - time;
}

void Task::reset_remaining()
{
    this->remaining = this->duration;
}

bool Task::missed_deadline(double time)
{
    return fabs( time - abs_due_date ) < 0.001 && std::isgreaterequal( remaining, abs_due_date-time );
}