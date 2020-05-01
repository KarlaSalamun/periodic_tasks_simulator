#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <cstdio>

#include "Task.h"
#include "TaskCreator.h"
#include "Scheduler.h"

struct task_ctx {
    Task *task;
    std::vector<Task *> pending;
    std::vector<Task *> processed;
};

class Dummy {
public:
    void execute( void * ) {}
};

template <typename T>
class Simulator {
public:
	Simulator( double time_slice, double finish_time, TaskCreator *tc, Scheduler *sched, bool GPSched ) :
		time_slice( time_slice ), finish_time( finish_time ), tc( tc ), sched( sched ), GPScheduling( GPSched )
	{
		tc->set_time_slice( time_slice );
		abs_time = 0;
		missed = 0;
		idle = false;
		running = nullptr;
	}
	double get_total_tardiness()
    {
	    return total_tardiness;
    }
    std::vector<Task *> get_pending()
    {
        return pending;
    }
    int get_missed()
    {
	    return missed;
    }
    void set_heuristic( T heur )
    {
	    this->heuristic = heur;
    }
    void set_finish_time( double time )
    {
	    finish_time = time;
    }
    void set_pending( std::vector<Task *> pending )
    {
	    this->pending = pending;
    }
	void run();
	void load();
	void create();
	std::vector<double> get_deadline_vector() {
	    return deadline_vector;
	}
	std::vector<double> get_idle_time_vector() {
	    return idle_time_vector;
	}

    double compute_deviation();
	double compute_skip_fitness();

private:
	//TODO: clanovi mogu biti unique_ptr
    struct task_ctx tctx;
	std::vector<Task *> pending;
	std::vector<Task *> ready;
	Task *running;
	double time_slice;
	double finish_time;
	TaskCreator *tc;
	Scheduler *sched;
	double abs_time;
	int missed;
	double total_tardiness;
	T heuristic;
	bool GPScheduling;
	bool idle;
	std::vector<double> deadline_vector;
	std::vector<double> idle_time_vector;

	void initialize_tasks()
	{
		for( auto & element : pending ) {
			element->initialize_task();
		}
	}
};

#endif