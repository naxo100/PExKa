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
#include "Mixture.h"
//#include "../state/AlgExpression.h"
//#include "../grammar/ast/Basics.h"
#include "../util/Exceptions.h"

using namespace std;

namespace ast {
class Id;
}

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
	IdMap varMap, tokenMap, compartmentMap, channelMap, signatureMap;
	//vector<string> algNames, kappaNames;
	vector<string> varNames, tokenNames, compartmentNames;
	vector<Signature> signatures;
	vector<Compartment> compartments;
	vector<list<Channel> > channels;
	vector<UseExpression> useExpressions;
	list<Mixture> mixtures;
	list<Mixture::Component> components;
	list<Mixture::Agent> agentPatterns;

	bool exists(const string &name,const IdMap &map);
public:
	Environment();
	~Environment();

	short declareToken(const ast::Id &name);
	short declareVariable(const ast::Id &name,bool isKappa);
	Signature& declareSignature(const ast::Id& sign);
	Compartment& declareCompartment(const ast::Id& comp);
	UseExpression& declareUseExpression(unsigned short id,size_t n);
	Channel& declareChannel(const ast::Id &channel);
	const Mixture& declareMixture(const Mixture& m);
	const Mixture::Component& declareComponent(const Mixture::Component& c);
	const Mixture::Agent& declareAgentPattern(const Mixture::Agent& a);
	short idOfAlg(const string& name);

	const Signature& getSignature(short id) const;
	const vector<Signature>& getSignatures() const;
	const Channel& getChannel(short id) const;
	const Compartment& getCompartment(short id) const;
	const UseExpression& getUseExpression(short id) const;

	short getVarId(const string &name) const;
	short getChannelId(const string &name) const;
	short getCompartmentId(const string &name) const;
	short getSignatureId(const string &name) const;
	short getTokenId(const string &name) const;
	state::BaseExpression* getVarExpression(const string &name) const;


	//DEBUG methods
	void show() const;

};

} /* namespace pattern */

#endif /* PATTERN_ENVIRONMENT_H_ */
