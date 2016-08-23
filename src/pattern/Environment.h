/*
 * Environment.h
 *
 *  Created on: Mar 23, 2016
 *      Author: naxo
 */

#ifndef PATTERN_ENVIRONMENT_H_
#define PATTERN_ENVIRONMENT_H_

#include <map>
#include <unordered_map>
#include <vector>
#include <string>
#include "Signature.h"
#include "../grammar/ast/Basics.h"
#include "../util/Exceptions.h"

using namespace std;

namespace pattern {

typedef unsigned short short_id;

/** \brief All the mappings for names and ids needed by the simulation.
 *
 * Manipulate and store all the mappings between names and ids of
 * elements declared with kappa. Global elements are stored first
 * to broadcast the global environment among MPI processes.
 */
class Environment {
	typedef unordered_map<string,short> IdMap;

	IdMap algMap,kappaMap;
	vector<string> algNames, kappaNames, tokenNames;
	vector<Signature> signatures;

	bool exists(const string &name,const IdMap &map);
public:
	Environment();
	~Environment();

	short declareVariable(const string &name,bool isKappa);
	short idOfAlg(const string& name);

};

} /* namespace pattern */

#endif /* PATTERN_ENVIRONMENT_H_ */
