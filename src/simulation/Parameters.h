/*
 * Parameters.h
 *
 *  Created on: Aug 25, 2017
 *      Author: naxo
 */

#ifndef SRC_SIMULATION_PARAMETERS_H_
#define SRC_SIMULATION_PARAMETERS_H_

#include "Counter.h"

int main(int argc, char* argv[]);

namespace simulation {

class Parameters {
	friend class Counter;
	static Parameters singleton;
	UINT_TYPE maxEvent;
	FL_TYPE maxTime;
	Parameters();
public:
	friend int ::main(int argc, char* argv[]);
	~Parameters();
	static Parameters& getInstance();

	UINT_TYPE limitEvent() const;
	FL_TYPE limitTime() const;
};

} /* namespace simulation */

#endif /* SRC_SIMULATION_PARAMETERS_H_ */
