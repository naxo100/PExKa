/*
 * Parameters.cpp
 *
 *  Created on: Aug 25, 2017
 *      Author: naxo
 */

#include "Parameters.h"

#include <limits>

namespace simulation {

Parameters Parameters::singleton;

Parameters::Parameters() : maxEvent(std::numeric_limits<UINT_TYPE>::max()),
		maxTime(/*std::numeric_limits<FL_TYPE>::infinity()*/0){}

Parameters::~Parameters() {}


Parameters& Parameters::getInstance(){
	return singleton;
}


UINT_TYPE Parameters::limitEvent() const{
	return maxEvent;
}
FL_TYPE Parameters::limitTime() const{
	return maxTime;
}

} /* namespace simulation */
