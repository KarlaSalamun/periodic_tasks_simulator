//
// Created by karla on 06. 03. 2020..
//

#ifndef TASK_H
#define TASK_H


class Task {
    public:
        double phase;
        int instance;
        int id;
        double period;
        double arrival_time;
        double duration;
        double abs_due_date;
        double rel_due_date;
        double priority;
        double time_started;
        double time_ended;
        double tardiness;
        double remaining;
        bool isPreempted;
        double compute_tardiness();
        void update_tardiness( double time );
        void set_arrival_time();
        void set_abs_dd();
        void update_params();
        bool isReady( double time );
        bool isFinished( double time );
        void inc_instance();
        void set_remaining( double time );

        Task( double phase, int instance, double period, double rel_due_date ) :
            phase( phase ), instance( instance ), period( period ), rel_due_date( rel_due_date )
        {
            tardiness = 0;
            isPreempted = false;
        }
        Task() {}
        ~Task() {}

};


#endif 
