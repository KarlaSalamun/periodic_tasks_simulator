//
// Created by karla on 06. 03. 2020..
//

#ifndef TASK_H
#define TASK_H

#include <cstdio>

typedef enum _state { RED, BLUE} state_t;

class Task {
    public:
        double compute_tardiness( double time );

        void update_tardiness( double time );
        void update_remaining();
        void update_params();
        void update_rb_params();
        void update_priority( double time );

        bool isReady( double time );
        bool isFinished();

        void inc_instance();

        int get_id();
        double get_phase();
        double get_period() const;
        double get_duration();
        double get_abs_due_date() const;
        double get_remaining();
        double get_tardiness();
        double get_priority() const;
        int get_instance();
        double get_weight();
        double get_time_started();
        state_t get_state();
        int get_skip_factor();
        double get_arrival_time() const;
        int get_curr_skip_value();

        void set_arrival_time();
        void set_abs_dd();
        void set_arrival_time( double arrival_time );
        void set_priority( double priority );
        void set_tardiness( double tard );
        void set_time_started( double time );
        void set_state( state_t state );
        void set_skip_factor( int factor );
        void set_time_slice( double timeslice );
        void reset_remaining();
        void set_curr_skip_value( int value );
        void set_duration( double duration );
        void set_weight( double weight );

        void initialize_task();

        bool is_missed( double time );
        bool is_next_instance( double time );
        bool missed_deadline(double time );

        void write_task( FILE *fd );

        void inc_skip_value();

        // this ctor is used for periodic tasks
        Task( double phase, int instance, double period, double rel_due_date, int id, double time_slice, double duration ) :
            phase( phase ), instance( instance ), period( period ), rel_due_date( rel_due_date ), 
            id( id ), time_slice( time_slice ), duration( duration )
        {
            remaining = duration;
            tardiness = 0;
            isPreempted = false;
            state = RED;
        }

        // this ctor is used for static tasks
        Task( double weight, double duration, double due_date ) : weight( weight ), duration( duration ), abs_due_date( due_date ) {}

        Task() = default;

        Task( const Task *task ) {
            this->period = task->period;
            this->remaining = task->remaining;
            this->rel_due_date = task->rel_due_date;
            this->arrival_time = task->arrival_time;
            this->duration = task->duration;
            this->instance = task->instance;
            this->state = task->state;
            this->time_slice = task->time_slice;
            this->id = task->id;
            this->abs_due_date = task->abs_due_date;
            this->tardiness = task->tardiness;
            this->isPreempted = task->isPreempted;
            this->phase = task->phase;
        }

        ~Task() = default;

        bool isPreempted;

    private:
        double phase;
        int instance;
        double period;
        double rel_due_date;
        int id;
        double time_slice;
        double weight;
        double arrival_time;
        double duration;
        int skip_factor;
        double abs_due_date;
        double priority;
        double tardiness;
        double remaining;
        double time_started;
        state_t state;
        int current_skip_value;
};



#endif 


