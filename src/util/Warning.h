/*
 * Warning.h
 *
 *  Created on: May 30, 2017
 *      Author: naxo
 */

#ifndef SRC_UTIL_WARNING_H_
#define SRC_UTIL_WARNING_H_

#include <list>
#include "../grammar/location.hh"

namespace simulation {

class Warning {
	std::string msg;
	yy::location loc;

public:
	Warning(const std::string& msg,const yy::location& loc);
	~Warning();

	const std::string what() const;
};


class WarningStack : std::list<Warning> {
	static WarningStack stack;
	WarningStack();
	WarningStack(const WarningStack&);
public:
	static WarningStack& getStack();
	using std::list<Warning>::emplace_back;
};

} /* namespace simulation */

#endif /* SRC_UTIL_WARNING_H_ */
