/*
 * Warning.h
 *
 *  Created on: May 30, 2017
 *      Author: naxo
 */

#ifndef SRC_UTIL_WARNING_H_
#define SRC_UTIL_WARNING_H_

#include <list>
#include <vector>
#include "../grammar/location.hh"


class Warning {
	std::string msg;
	yy::location loc;

public:
	Warning(const std::string& msg,const yy::location& loc);
	~Warning();

	const std::string what() const;
};


class WarningStack : private std::list<Warning> {
	static WarningStack stack;
	static const int MAX_WARNS = 3;
	//static int global_id;
	std::vector<int> counts;
	WarningStack();
	WarningStack(const WarningStack&);
public:
	static WarningStack& getStack();
	int add(int id,const std::string& msg,const yy::location &loc = yy::location());
	void show(bool show_nowarns = true);
};


#define ADD_WARN(msg,loc) _Pragma("omp critical") {static int id = 0;id = WarningStack::getStack().add(id,msg,loc);}
#define ADD_WARN_NOLOC(msg) _Pragma("omp critical") {static int id = 0;id = WarningStack::getStack().add(id,msg);}

#endif /* SRC_UTIL_WARNING_H_ */
