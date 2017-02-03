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


using namespace std;

namespace pattern {

//Forward declaration needed for setComponents() and toString()
class Environment;

//pair of shorts used for (agent,agent) or (agent,site) ids.
typedef pair<short,short> ag_st_id;

//struct to compare id_pair needed for maps
template <typename T1,typename T2>
struct ComparePair{
	bool operator()(pair<T1,T2> p1,pair<T1,T2> p2);
};



/** \brief Representation of a set of agent complexes.
 *
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

	/** \brief Identify and declare connected components in this mixture.
	 * Using the links and agents in this mixture, this method identifies
	 * all connected components and declare them in the environment. Agent
	 * pointers contained in this mixture are removed and moved to components.
	 * Also the graph is cleared and moved to the components.
	 * @param env the environment of simulation.
	 */
	void setComponents(Environment &env);

	size_t size() const;

	/** \brief returns a string representation of this mixture.
	 *
	 * @param env the environment of simulation.
	 */
	string toString(const Environment &env) const;


private:
	union {
		const Agent** agents;
		vector<const Component*>* comps;
	};
	size_t agentCount;
	size_t siteCount;
	size_t compCount;
	//typedef pair<const Agent*,short> ag_st_pntr;
	//two consecutive pairs are a link
	list< ag_st_id > links;

};

//bool (*f)(pair<short,short>,pair<short,short>) = [](pair<short,short>& p1,pair<short,short>& p2) {return p1.first < p2.first ? p1 : (p1.second < p2.second ? p1 : p2 );};


struct Mixture::Site {
	ValueType val_type;
	union {
		int int_value;
		float float_value;
		short id_value;
		string* lbl;
	} state;
	//short path_id;
	LinkType link_type;
	std::pair<short,short> lnk;//agent,site

	bool operator==(const Site &s) const;

	//Site();
};

class Mixture::Agent {
	short signId;
	std::unordered_map<int,Site> interface;

public:
	Agent(short name_id);

	short getId() const;
	Site& addSite(short name_id);
	void setSiteValue(short mix_site,short label);
	void setSiteValue(short mix_site,int val);
	void setSiteValue(short mix_site,float val);

	void setSiteLink(short mix_site,LinkType l);

	const Site& getSite(short id) const;
	bool operator==(const Agent& a) const;
};

class Mixture::Component {
	vector<const Agent*> agents;
	union {
		list<ag_st_id> *links;
		map<ag_st_id,ag_st_id> *graph;
	};

public:
	Component();
	Component(const Component& comp);
	bool contains(const Mixture::Agent* a);
	short addAgent(const Mixture::Agent* a);
	void addLink(const pair<ag_st_id,ag_st_id> &lnk,const map<short,short> &mask);

	size_t size() const;
	void setGraph();

	bool operator==(const Mixture::Component &m) const;
};

} /* namespace pattern */

#endif /* PATTERN_MIXTURE_H_ */
