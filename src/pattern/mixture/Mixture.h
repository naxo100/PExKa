/*
 * Mixture.h
 *
 *  Created on: Apr 26, 2016
 *      Author: naxo
 */

#ifndef PATTERN_MIXTURE_H_
#define PATTERN_MIXTURE_H_

#include <map>
#include <unordered_map>
#include <vector>
#include <list>
#include <utility> //pair
#include <string>
#include "../../util/params.h"
#include "../../expressions/AlgExpression.h"


using namespace std;

//forward declaration of Rule
namespace simulation {
	class Rule;
}

namespace pattern {

//Forward declaration needed for setComponents() and toString()
class Environment;

/* Abstract class Pattern *
 *
 * CCs and Mixtures are patterns.
 * They are included in rule's LHSs.
 */
class Pattern {
protected:
	set<small_id> includes;
public:
	class Agent;
	struct Site;
	enum LinkType {FREE,WILD,BIND,BIND_TO,PATH};
	enum ValueType {VOID,LABEL,INT_VAL,FLOAT_VAL};
	virtual ~Pattern(){};
	virtual short_id getId() const = 0;
	virtual const Agent& getAgent(small_id ag_id ) const = 0;
	virtual size_t size() const = 0;
	const set<small_id>& includedIn() const;
	virtual void addInclude(small_id id);
};



//pair of shorts used for (agent,agent) or (agent,site) ids.
typedef pair<small_id,small_id> ag_st_id;
ostream& operator<<(ostream& os,const ag_st_id &ag_st);

//struct to compare id_pair needed for maps
template <typename T1,typename T2>
struct ComparePair{
	bool operator()(pair<T1,T2> p1,pair<T1,T2> p2);
};



/** \brief Representation of a set of agent complexes.
 * The class Mixture must be initialized with the quantity of agents it will contain.
 * Using the addAgent() and addLink() methods the class is filled until it's complete.
 * Then setComponents(env) must be called to end initialization and declare components
 * in the environment. After that, the mixture is ready to be declare in the environment.
 * Any attempt to add links or agents to the mixture after setComponents() will
 * throw exceptions.
 */
class Mixture : public Pattern {
public:
	using Agent = Pattern::Agent;
	using Site = Pattern::Site;
	class Component;
	using LinkType = Pattern::LinkType;


	/** \brief Initialize a new Mixture with fixed capacity for agents
	 *
	 * @param agent_count total agents of this mixture.
	 */
	Mixture(short agent_count);
	Mixture(const Mixture& m);
	~Mixture();

	void setId(short_id id);
	short_id getId() const override;

	void setAux(string id,small_id ag,small_id site);
	void setAuxCoords(const std::map<std::string,std::tuple<int,small_id,small_id>>& aux_coords);

	/** \brief Add a new agent to the mixture.
	 * Add a new agent to the mixture from a pointer to
	 * an agent in environment.
	 * @param a a pointer to agent declared in the environment.
	 */
	void addAgent(Agent* a);

	/** \brief Add a new connection between agent's sites
	 *
	 * @param first,second the new connection in the format
	 * pair(agmix_id,site_id).
	 */
	void addLink(const ag_st_id &first,const ag_st_id &second,const Environment& env);

	/** \brief Declare Agents in the environment.
	 *
	 * Need to call this method before setComponents(env).
	 * @param env the environment of simulation.
	 */
	void declareAgents(Environment &env,bool is_lhs = true);

	/** \brief Identify and declare connected components in this mixture.
	 * Using the links and agents in this mixture, this method identifies
	 * all connected components and declare them in the environment. Agent
	 * pointers contained in this mixture are removed and moved to components.
	 * Also the graph is cleared and moved to the components. After call this
	 * method declaredComps is set to true.
	 * @param env the environment of simulation.
	 */
	vector<ag_st_id> setComponents();
	vector<ag_st_id> setAndDeclareComponents(Environment& env);

	bool operator==(const Mixture& m) const;

	const Agent& getAgent(small_id cc,small_id ag) const;
	const Agent& getAgent(ag_st_id cc_ag ) const;
	const Agent& getAgent(small_id ag_id ) const override;

	void addInclude(small_id id) override;

	const Component& getComponent(small_id) const;
	Component& getComponent(small_id);

	const map<string,tuple<small_id,small_id,small_id>>& getAux() const;

	ag_st_id follow(small_id cc_id,small_id ag_id,small_id site) const;

	/** \brief returns the agent counter.
	 *
	 */
	size_t size() const;
	size_t compsCount() const;

	const vector<Component*>::iterator begin() const;
	const vector<Component*>::iterator end() const;

	/** \brief returns a string representation of this mixture.
	 *
	 * @param env the environment of simulation.
	 */
	string toString(const Environment &env) const;
	string toString(const Environment &env,const vector<ag_st_id> &mask) const;


private:
	//true for valid comps; false for valid agents.
	bool declaredComps;
	Agent** agents;
	vector<Component*>* comps;
	map<string,tuple<small_id,small_id,small_id>> auxiliars;

	size_t agentCount;
	size_t siteCount;
	size_t compCount;
	//typedef pair<const Agent*,short> ag_st_pntr;
	//link (agent<,site)->(agent>,site)
	map< ag_st_id , ag_st_id> links;
	short_id id;

};

typedef pair<const Mixture&,const vector<ag_st_id&> >OrderedMixture;

//bool (*f)(pair<short,short>,pair<short,short>) = [](pair<short,short>& p1,pair<short,short>& p2) {return p1.first < p2.first ? p1 : (p1.second < p2.second ? p1 : p2 );};






} /* namespace pattern */

#endif /* PATTERN_MIXTURE_H_ */
