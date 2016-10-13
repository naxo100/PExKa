/*
 * Dependencies.h
 *
 *  Created on: Apr 21, 2016
 *      Author: naxo
 */

#ifndef PATTERN_DEPENDENCIES_H_
#define PATTERN_DEPENDENCIES_H_

#include <map>
#include <set>
#include <vector>
#include <list>

namespace pattern {

using namespace std;
class Dependencies {
	enum Dep : char {TIME,EVENT,KAPPA,VAR,RULE,PERT};
	struct Dependency {
		Dep dep;
		unsigned int id;
	};
	struct DepSet {
		set<Dependency> deps;
		list<Dependency> ordered_deps;
	};
	map< Dependency , DepSet > deps;
public:
	Dependencies();
	~Dependencies();

	void addDependency(Dependency key,Dep d, unsigned int id);
	DepSet getDependencies(Dependency d);
};

} /* namespace pattern */

#endif /* PATTERN_DEPENDENCIES_H_ */
