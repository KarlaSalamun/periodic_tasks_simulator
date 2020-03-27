#include <cstdio>
#include "Task.h"
#include "TaskCreator.h"
#include "Simulator.h"

int main()
{
	Simulator *sim = new Simulator( 0.1, 5 );
	sim->initialize();
	sim->run();
	return 0;
}