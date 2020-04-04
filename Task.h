//
// Created by karla on 06. 03. 2020..
//

#ifndef TASK_H
#define TASK_H


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

        int get_id();
        double get_phase();
        double get_period();
        double get_duration();
        double get_abs_due_date();
        double get_remaining();
        double get_tardiness();
        double get_priority() const;
        int get_instance();

        void initialize_task();

        bool is_missed( double time );
        bool is_next_instance( double time );

        void write_task( FILE *fd );

        Task( double phase, int instance, double period, double rel_due_date, int id, double time_slice, double duration ) :
            phase( phase ), instance( instance ), period( period ), rel_due_date( rel_due_date ), 
            id( id ), time_slice( time_slice ), duration( duration )
        {
            remaining = duration;
            tardiness = 0;
            isPreempted = false;
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
        double arrival_time;
        double duration;
        double abs_due_date;
        double priority;
        double tardiness;
        double remaining;
};


#endif 


