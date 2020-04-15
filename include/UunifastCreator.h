#pragma once

#include <vector>
#include "TaskCreator.h"

class UunifastCreator : public TaskCreator {
public:
	UunifastCreator( int task_number, std::string filename, bool skippable, double lim_u, double lim_d, double granulation, double overload_factor ) :
		TaskCreator( task_number, filename, skippable )
	{
		this->lim_u = lim_u;
		this->lim_d = lim_d;
		this->granulation = granulation;
		this->overload_factor = overload_factor;
	}

	virtual void create_test_set( std::vector<Task *> &test_tasks ) override;

private:
	double lim_u;
	double lim_d;
	double granulation;
	double mean_u = 1;
	double overload_factor;

	std::vector<double> generate_utils();
	std::vector<double> generate_log_uniform();
};