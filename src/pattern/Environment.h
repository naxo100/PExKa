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
#include "Compartment.h"
#include "Channel.h"
#include "../state/AlgExpression.h"
//#include "../grammar/ast/Basics.h"
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
protected:
	typedef unordered_map<string,short> IdMap;

	//IdMap algMap, kappaMap;
	IdMap varMap, tokenMap, compartmentMap, channelMap;
	//vector<string> algNames, kappaNames;
	vector<string> varNames, tokenNames, compartmentNames;
	vector<Signature> signatures;
	vector<Compartment> compartments;
	vector<Channel> channels;

	bool exists(const string &name,const IdMap &map);
public:
	Environment();
	~Environment();

	short declareToken(const string &name);
	short declareVariable(const string &name,bool isKappa);
	short declareSignature(const Signature& sign);
	short declareCompartment(const Compartment& comp);
	short declareChannel(const string &name);
	short idOfAlg(const string& name);

	Signature& getSignature(short id) const;
	const Channel& getChannel(short id) const;
	const Compartment& getCompartment(short id) const;

	short getVarId(const string &name) const;
	short getChannelId(const string &name) const;
	state::BaseExpression* getVarExpression(const string &name) const;

};

} /* namespace pattern */

#endif /* PATTERN_ENVIRONMENT_H_ */
