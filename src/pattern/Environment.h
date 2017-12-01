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
#include "../simulation/Rule.h"
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
	//static Environment env;
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
	vector<list<Mixture::Agent> > agentPatterns;
	vector<simulation::Rule> rules;

	bool exists(const string &name,const IdMap &map);
public:
	//inline static Environment& getInstance() {return env;}
	Environment();
	~Environment();

	short declareToken(const ast::Id &name);
	short declareVariable(const ast::Id &name,bool isKappa);
	Signature& declareSignature(const ast::Id& sign);
	Compartment& declareCompartment(const ast::Id& comp);
	UseExpression& declareUseExpression(unsigned short id,size_t n);
	Channel& declareChannel(const ast::Id &channel);
	Mixture& declareMixture(const Mixture& m);
	Mixture::Component& declareComponent(const Mixture::Component& c);
	Mixture::Agent& declareAgentPattern(const Mixture::Agent* a,bool is_lhs = true);
	simulation::Rule& declareRule(const ast::Id &name,const Mixture& mix);

	void buildInfluenceMap();

	const Signature& getSignature(short id) const;
	const vector<Signature>& getSignatures() const;
	const list<Channel>& getChannels(short id) const;
	const Compartment& getCompartment(short id) const;
	const UseExpression& getUseExpression(short id) const;
	//const Mixture& getMixture(small_id id) const;
	const list<Mixture>& getMixtures() const;
	const list<Mixture::Component>& getComponents() const;
	const list<Mixture::Agent>& getAgentPatterns(small_id id) const;
	const vector<simulation::Rule>& getRules() const;


	const Compartment& getCompartmentByCellId(unsigned id) const;

	short idOfAlg(const string& name) const;
	short getVarId(const string &name) const;
	short getChannelId(const string &name) const;
	short getCompartmentId(const string &name) const;
	short getSignatureId(const string &name) const;
	short getTokenId(const string &name) const;

	state::BaseExpression* getVarExpression(const string &name) const;

	template <typename T>
	size_t size() const;
	template <typename T>
	void reserve(size_t count);


	//DEBUG methods
	std::string cellIdToString(unsigned int cell_id) const;
	void show() const;

};

//using env = Environment::getInstance();

} /* namespace pattern */

#endif /* PATTERN_ENVIRONMENT_H_ */
