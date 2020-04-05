//
// Created by karla on 06. 03. 2020..
//

#ifndef TASK_H
#define TASK_H

#include <cstdio>


class Task {
    public:
        double compute_tardiness( double time );

        void update_tardiness( double time );
        void update_remaining();
        void update_params();
        void update_priority( double time );

        bool isReady( double time );
        bool isFinished();

        void inc_instance();

        void set_arrival_time();
        void set_abs_dd();
        void set_arrival_time( double arrival_time );
        void set_priority( double priority );
        void set_tardiness( double tard );
        void set_time_started( double time );

        int get_id();
        double get_phase();
        double get_period();
        double get_duration();
        double get_abs_due_date();
        double get_remaining();
        double get_tardiness();
        double get_priority() const;
        int get_instance();
        double get_weight();
        double get_time_started();

        void initialize_task();

        bool is_missed( double time );
        bool is_next_instance( double time );

        void write_task( FILE *fd );

        // this ctor is used for periodic tasks
        Task( double phase, int instance, double period, double rel_due_date, int id, double time_slice, double duration ) :
            phase( phase ), instance( instance ), period( period ), rel_due_date( rel_due_date ), 
            id( id ), time_slice( time_slice ), duration( duration )
        {
            remaining = duration;
            tardiness = 0;
            isPreempted = false;
        }

        // this ctor is used for static tasks
        Task( double weight, double duration, double due_date ) : weight( weight ), duration( duration ), abs_due_date( due_date ) {}

        Task() = default;

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
        double abs_due_date;
        double priority;
        double tardiness;
        double remaining;
        double time_started;
};


#endif 


