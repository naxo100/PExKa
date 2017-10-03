/*
 * Parameters.h
 *
 *  Created on: Aug 25, 2017
 *      Author: naxo
 */

#ifndef SRC_SIMULATION_PARAMETERS_H_
#define SRC_SIMULATION_PARAMETERS_H_

namespace simulation {

class Parameters {
	static Parameters singleton;
	long maxEvent;
	float maxTime;
	Parameters();
public:
	~Parameters();
	static Parameters& getInstance();

	const long limitEvent() const;
	const long limitTime() const;
};

} /* namespace simulation */

#endif /* SRC_SIMULATION_PARAMETERS_H_ */
