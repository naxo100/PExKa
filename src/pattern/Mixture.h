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
#include "../util/params.h"
#include "../state/AlgExpression.h"


using namespace std;

namespace pattern {

//Forward declaration needed for setComponents() and toString()
class Environment;

//pair of shorts used for (agent,agent) or (agent,site) ids.
typedef pair<small_id,small_id> ag_st_id;

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
class Mixture {
public:
	struct Site;
	class Agent;
	class Component;
	enum LinkType {FREE,WILD,BIND,BIND_TO,PATH};
	enum ValueType {VOID,LABEL,INT_VAL,FLOAT_VAL};

	/** \brief Initialize a new Mixture with fixed capacity for agents
	 *
	 * @param agent_count total agents of this mixture.
	 */
	Mixture(short agent_count);
	Mixture(const Mixture& m);
	~Mixture();

	/** \brief Add a new agent to the mixture.
	 * Add a new agent to the mixture from a pointer to
	 * an agent in environment.
	 * @param a a pointer to agent declared in the environment.
	 */
	void addAgent(const Agent* a);

	/** \brief Add a new connection between agent's sites
	 *
	 * @param first,second the new connection in the format
	 * pair(agmix_id,site_id).
	 */
	void addLink(const ag_st_id &first,const ag_st_id &second);

	/** \brief Declare Agents in the environment.
	 *
	 * Need to call this method before setComponents(env).
	 * @param env the environment of simulation.
	 */
	void declareAgents(Environment &env);

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

	ag_st_id follow(small_id cc_id,small_id ag_id,small_id site) const;

	/** \brief returns the agent counter.
	 *
	 */
	size_t size() const;

	const vector<const Component*>::iterator begin() const;
	const vector<const Component*>::iterator end() const;

	/** \brief returns a string representation of this mixture.
	 *
	 * @param env the environment of simulation.
	 */
	string toString(const Environment &env) const;
	string toString(const Environment &env,const vector<ag_st_id> &mask) const;


private:
	//true for valid comps; false for valid agents.
	bool declaredComps;
	union {//we need to preserve mixture order!!!
		const Agent** agents;
		vector<const Component*>* comps;
	};

	size_t agentCount;
	size_t siteCount;
	size_t compCount;
	//typedef pair<const Agent*,short> ag_st_pntr;
	//link (agent<,site)->(agent>,site)
	map< ag_st_id , ag_st_id> links;

};

typedef pair<const Mixture&,const vector<ag_st_id&> >OrderedMixture;

//bool (*f)(pair<short,short>,pair<short,short>) = [](pair<short,short>& p1,pair<short,short>& p2) {return p1.first < p2.first ? p1 : (p1.second < p2.second ? p1 : p2 );};


struct Mixture::Site {
	state::SomeValue state;

	LinkType link_type;
	//only valid if type is BIND_TO
	std::pair<short,short> lnk_ptrn;//agent,site

	bool operator==(const Site &s) const;

	Site();
};

class Mixture::Agent {
	short signId; //signature ID
	std::unordered_map<small_id,Site> interface;

public:
	Agent(short name_id);

	short getId() const;
	Site& addSite(short name_id);
	void setSiteValue(small_id mix_site,small_id label);
	void setSiteValue(small_id mix_site,int val);
	void setSiteValue(small_id mix_site,float val);

	void setSiteLink(short mix_site,LinkType l);

	const Site& getSite(short id) const;
	bool operator==(const Agent& a) const;

	const unordered_map<small_id,Site>::const_iterator begin() const;
	const unordered_map<small_id,Site>::const_iterator end() const;

	const string toString( const Environment& env, short mixAgId=-1,
			map<ag_st_id,short> bindLabels = map<ag_st_id,short>() ) const;
};

/** \brief Defines a set of agents that are explicitly connected by sites.
 * Class Mixture is initialized empty and then filled with agents and links.
 * After that, setGraph() must be called to reorder agents and produce graph,
 * making the mixture comparable and ready to be declared in the environment.
 *
 */
class Mixture::Component {
	vector<const Agent*> agents;
	union {
		list<ag_st_id> *links;
		//(comp_ag_id,site_id) -> (comp_ag_id,site_id)
		map<ag_st_id,ag_st_id> *graph;
	};

public:
	Component();
	Component(const Component& comp);
	~Component();
	bool contains(const Mixture::Agent* a);
	short addAgent(const Mixture::Agent* a);
	void addLink(const pair<ag_st_id,ag_st_id> &lnk,const map<short,short> &mask);

	size_t size() const;

	const vector<const Agent*>::const_iterator begin() const;
	const vector<const Agent*>::const_iterator end() const;

	vector<small_id> setGraph();
	const map<ag_st_id,ag_st_id>& getGraph() const;
	string toString(const Environment& env) const;

	const Mixture::Agent& getAgent(small_id ag) const;
	ag_st_id follow(small_id ag_id,small_id site) const;

	bool operator==(const Mixture::Component &m) const;
};

} /* namespace pattern */

#endif /* PATTERN_MIXTURE_H_ */
