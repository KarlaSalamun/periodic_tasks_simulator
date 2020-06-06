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
	assert(fprintf( fd, "%d %f %zu %zu \n", id, phase, period, duration ) > 0);
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
    if( fabs( time - arrival_time ) < 0.001 ) {
        return true;
    }
    return time >= arrival_time;
}

bool Task::isFinished() 
{
	return remaining <= time_slice;
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

void Task::set_time_slice( double timeslice )
{
    this->time_slice = timeslice;
}

void Task::set_curr_skip_value(int value)
{
    this->current_skip_value = value;
}

void Task::set_duration( double duration )
{
    this->duration = duration;
}

void Task::set_weight( double weight )
{
    this->weight = weight;
}

void Task::set_max_instances( int number )
{
    this->max_instances = number;
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

double Task::get_period() const
{
	return this->period;
}

double Task::get_duration()
{
	return this->duration;
}

double Task::get_abs_due_date() const
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

int Task::get_skip_factor()
{
    return this->skip_factor;
}

double Task::get_arrival_time() const
{
    return this->arrival_time;
}

int Task::get_curr_skip_value()
{
    return this->current_skip_value;
}

int Task::get_max_instances()
{
    return this->max_instances;
}


void Task::initialize_task() 
{
	arrival_time = phase;
	instance = 1;
	current_skip_value = 1;
	set_abs_dd();
	remaining = duration;
	state = RED;
	isPreempted = false;
	tardiness = 0;
	skip_factors.clear();
	weight = 1;
}

bool Task::is_missed( double time )
{
	return std::islessequal( phase + (instance-1) * period, time );
}

bool Task::is_next_instance( double time )
{
    if( fabs( phase + instance * period - time ) < 0.001 ) {
        return true;
    }
	return std::isgreaterequal( time, phase + instance * period );
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
    /*
    if( this->state == RED ) {
        this->state = BLUE;
        return;
    }
    current_skip_value++;
    if( current_skip_value >= skip_factor ) {
        current_skip_value = 0;
        this->state = RED;
    }
    else {
        this->state = BLUE;
    }*/
    this->state = BLUE;
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

void Task::inc_skip_value()
{
    this->current_skip_value++;
}

void Task::reset_skip_value()
{
    this->current_skip_value = 1;
}

double Task::compute_mean_skip_factor() {
    if (skip_factors.empty()) {
        return 0;
    }
    double sum = 0;
    for (auto &element : skip_factors) {
        sum += element;
    }
    return sum / static_cast<double>( skip_factors.size() );
}
