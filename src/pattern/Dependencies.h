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
#include "../util/params.h"

namespace pattern {

using namespace std;
class Dependencies {
public:
	enum Dep : char {TIME,EVENT,KAPPA,TOK,VAR,RULE,PERT,AUX};
	struct Dependency {
		Dep type;
		//union{
			big_id id;
			string aux;
		//};
		Dependency(Dep d,unsigned i);
		Dependency(string _aux);
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
