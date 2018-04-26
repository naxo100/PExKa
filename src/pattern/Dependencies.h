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
public:
	enum Dep : char {TIME,EVENT,KAPPA,TOK,VAR,RULE,PERT};
	struct Dependency {
		Dep type;
		unsigned int id;
		Dependency(Dep d,unsigned i);
		bool operator<(const Dependency& d) const;
	};
	struct DepSet2 {
		set<Dependency> deps;
		list<Dependency> ordered_deps;//todo idea to improve time execution in time perturbations?
	};
protected:
	map< Dependency , DepSet2 > deps;
public:
	Dependencies();
	~Dependencies();

	void addDependency(Dependency key,Dep d, unsigned int id);
	const DepSet2& getDependencies(Dependency d);
};

typedef set<Dependencies::Dependency> DepSet;


} /* namespace pattern */



#endif /* PATTERN_DEPENDENCIES_H_ */
