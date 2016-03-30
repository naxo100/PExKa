/*
 * Environment.h
 *
 *  Created on: Mar 23, 2016
 *      Author: naxo
 */

#ifndef PATTERN_ENVIRONMENT_H_
#define PATTERN_ENVIRONMENT_H_

namespace pattern {

/** \brief All the mappings for names and ids needed by the simulation.
 *
 * Manipulate and store all the mappings between names and ids of
 * elements declared with kappa. Global elements are stored first
 * to broadcast the global environment among MPI processes.
 */
class Environment {
public:
	Environment();
	~Environment();
};

} /* namespace pattern */

#endif /* PATTERN_ENVIRONMENT_H_ */
