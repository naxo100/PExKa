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

struct Dependency {
	enum Dep : char {TIME,EVENT,KAPPA,TOK,VAR,RULE,PERT,AUX,NONE};
	Dep type;
	//union{
		big_id id;
		string aux;
	//};
	Dependency(Dep d = NONE,unsigned i = 0);
	Dependency(string _aux);
	bool operator<(const Dependency& d) const;
};

class Dependencies {
public:
	struct DepSet2 {
		set<Dependency> deps;
		multimap<float,Dependency> ordered_deps;//todo idea to improve time execution in time perturbations?
	};
protected:
	map< Dependency , DepSet2 > deps;
public:
	Dependencies();
	~Dependencies();

	void addDependency(Dependency key,Dependency::Dep d, unsigned int id);
	void addTimePertDependency(unsigned p_id,float time);
	const DepSet2& getDependencies(Dependency d);

	void erase(Dependency key,Dependency trigger);
	void eraseTimePerts(multimap<float,Dependency>::const_iterator first,
			multimap<float,Dependency>::const_iterator last);
};

typedef set<Dependency> DepSet;


} /* namespace pattern */



#endif /* PATTERN_DEPENDENCIES_H_ */
