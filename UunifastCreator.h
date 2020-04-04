#pragma once

#include <vector>
#include "TaskCreator.h"

class UunifastCreator : public TaskCreator {
public:
	UunifastCreator( int task_number, std::string filename, double lim_u, double lim_d, double granulation ) : TaskCreator( task_number, filename )
	{
		this->lim_u = lim_u;
		this->lim_d = lim_d;
		this->granulation = granulation;
	}

	virtual void create_test_set( std::vector<Task *> &test_tasks ) override;

private:
	double lim_u;
	double lim_d;
	double granulation;
	double mean_u = 1;

	std::vector<double> generate_utils();
	std::vector<double> generate_log_uniform();
};