/*
 * Parameters.cpp
 *
 *  Created on: Aug 25, 2017
 *      Author: naxo
 */

#include "Parameters.h"

namespace simulation {

Parameters Parameters::singleton;

Parameters::Parameters() : maxTime(5.) {
	// TODO Auto-generated constructor stub

}

Parameters::~Parameters() {
	// TODO Auto-generated destructor stub
}


Parameters& Parameters::getInstance(){
	return singleton;
}


const long Parameters::limitEvent() const{
	return maxEvent;
}
const long Parameters::limitTime() const{
	return maxTime;
}

} /* namespace simulation */
