#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <cstdio>

#include "Task.h"
#include "TaskCreator.h"
#include "Scheduler.h"

class Simulator {
public:
	Simulator( double time_slice, double finish_time ) : time_slice( time_slice ), finish_time( finish_time ) 
	{
		abs_time = 0;
		missed = 0;
	}
	void run();
	void initialize();
	void update_params( Task *&task, double time );
private:
	std::vector<Task *> pending;
	std::vector<Task *> ready;
	double time_slice;
	double finish_time;
	double abs_time;
	int missed;
};

#endif