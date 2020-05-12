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
	Simulator( double time_slice, double finish_time, TaskCreator *tc, Scheduler *sched, bool GPSched, bool display_sched ) :
		time_slice( time_slice ), finish_time( finish_time ), tc( tc ), sched( sched ), GPScheduling( GPSched ), display_sched( display_sched )
	{
		tc->set_time_slice( time_slice );
		abs_time = 0;
		missed = 0;
		completed = 0;
		all_tasks = 0;
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
    void set_heuristic( std::pair<T, T> heur )
    {
        this->priority_heuristic = heur.first;
        this->acceptance_heuristic = heur.second;
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
	double get_qos() {
	    return qos;
	}
	double get_mean_skip_factor() {
	    return mean_skip_factor;
	}
	void set_display() {
	    display_sched = true;
	}
	void set_filename( std::string filename ) {
	    this->filename = filename;
	}

    double compute_deviation();
	double compute_skip_fitness();
    void display_info();
    void compute_mean_skip_factor();

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
	int completed;
	int all_tasks;
	double total_tardiness;
    T priority_heuristic;
    T acceptance_heuristic;
	bool GPScheduling;
	bool idle;
	double total_skip_factor;
	std::vector<double> deadline_vector;
	std::vector<double> idle_time_vector;
	double qos;
	double mean_skip_factor;
	std::string filename;
	bool display_sched;

	void initialize_tasks()
	{
		for( auto & element : pending ) {
			element->initialize_task();
			element->set_max_instances( floor( tc->get_hyperperiod() / element->get_period() ) );
		}
	}
};

#endif