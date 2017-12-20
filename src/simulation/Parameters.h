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
	unsigned points;

	string file;

	Parameters();
public:
	friend int ::main(int argc, char* argv[]);
	~Parameters();
	static Parameters& getInstance();

	UINT_TYPE limitEvent() const;
	FL_TYPE limitTime() const;
	unsigned plotPoints() const;
	const string& outputFile() const;
};

} /* namespace simulation */

#endif /* SRC_SIMULATION_PARAMETERS_H_ */
