//
// Created by karla on 06. 03. 2020..
//

#ifndef TASK_H
#define TASK_H

#include <cstdio>
#include <vector>

typedef enum _state { RED, BLUE} state_t;

class Task {
    public:
        int compute_tardiness( int time );

        void update_tardiness( int time );
        void update_remaining();
        void update_params();
        void update_rb_params();
        void update_priority( double time );

        bool isReady( int time );
        bool isFinished();

        void inc_instance();

        int get_id();
        int get_phase();
        int get_period() const;
        int get_duration();
        int get_abs_due_date() const;
        int get_remaining();
        int get_tardiness();
        double get_priority() const;
        int get_instance();
        double get_weight();
        int get_time_started();
        state_t get_state();
        int get_skip_factor();
        int get_arrival_time() const;
        int get_curr_skip_value();
        int get_max_instances();

        void set_arrival_time();
        void set_abs_dd();
        void set_arrival_time( int arrival_time );
        void set_priority( double priority );
        void set_tardiness( int tard );
        void set_time_started( int time );
        void set_state( state_t state );
        void set_skip_factor( int factor );
        void reset_remaining();
        void set_curr_skip_value( int value );
        void set_duration( int duration );
        void set_weight( double weight );
        void set_max_instances( int number );

        void initialize_task();

        bool is_missed( double time );
        bool is_next_instance( int time );
        bool missed_deadline(double time );

        void write_task( FILE *fd );

        void inc_skip_value();
        void reset_skip_value();
        double compute_mean_skip_factor();

        void inc_missed(void);

        int get_released();
        int get_completed();

        void inc_released();
        void inc_completed();

    // this ctor is used for periodic tasks
        Task( int phase, int instance, int period, int rel_due_date, int id, int duration ) :
            phase( phase ), instance( instance ), period( period ), rel_due_date( rel_due_date ), 
            id( id ), duration( duration )
        {
            current_skip_value = 1;
            remaining = duration;
            tardiness = 0;
            isPreempted = false;
            state = RED;
            weight = 1;
            priority = 0;
        }

        // this ctor is used for static tasks
        Task( double weight, int duration, int due_date ) : weight( weight ), duration( duration ), abs_due_date( due_date ) {}

        Task() = default;

        Task( const Task *task ) {
            this->period = task->period;
            this->remaining = task->remaining;
            this->rel_due_date = task->rel_due_date;
            this->arrival_time = task->arrival_time;
            this->duration = task->duration;
            this->instance = task->instance;
            this->state = task->state;
            this->id = task->id;
            this->abs_due_date = task->abs_due_date;
            this->tardiness = task->tardiness;
            this->isPreempted = task->isPreempted;
            this->phase = task->phase;
            this->current_skip_value = task->current_skip_value;
            this->skip_factors = task->skip_factors;
            this->weight = task->weight;
            this->priority = task->priority;
            this->max_instances = task->max_instances;
            this->released = task->released;
            this->current_skip_value = task->current_skip_value;
            this->completed = task->completed;
        }

        Task ( Task && task ) {
            this->period = task.period;
            this->remaining = task.remaining;
            this->rel_due_date = task.rel_due_date;
            this->arrival_time = task.arrival_time;
            this->duration = task.duration;
            this->instance = task.instance;
            this->state = task.state;
            this->id = task.id;
            this->abs_due_date = task.abs_due_date;
            this->tardiness = task.tardiness;
            this->isPreempted = task.isPreempted;
            this->phase = task.phase;
            this->current_skip_value = task.current_skip_value;
            this->skip_factors = task.skip_factors;
            this->weight = task.weight;
            this->priority = task.priority;
            this->max_instances = task.max_instances;
            this->released = task.released;
            this->current_skip_value = task.current_skip_value;
            this->completed = task.completed;
        }

    ~Task() = default;

        bool isPreempted;
        std::vector<int> skip_factors;

    private:
        size_t phase;
        size_t instance;
        size_t period;
        size_t rel_due_date;
        int id;
        double weight;
        size_t arrival_time;
        size_t duration;
        int skip_factor;
        size_t abs_due_date;
        double priority;
        size_t tardiness;
        size_t remaining;
        size_t time_started;
        state_t state;
        int current_skip_value;
        int max_instances;
        int missed;
        int completed;
        int released;
};
#endif 


