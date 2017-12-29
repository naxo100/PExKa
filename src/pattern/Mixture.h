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


/** \brief Structure of an agent site of a mixture.
 * It stores the site value and link state of kappa
 * declared site of an agent mixture.
 */
struct Pattern::Site {
	state::SomeValue state;

	LinkType link_type;
	//only valid if type is BIND_TO
	ag_st_id lnk_ptrn;//agent,site (-1,-1)

	bool operator==(const Site &s) const;
	int compare(const Site &s) const;

	/** \brief Constructs an empty site.
	 * Default state value is (small_id)-1 and
	 * default link_type is FREE.
	 */
	Site();
	int compareLinkPtrn(ag_st_id ptrn) const;
	bool isEmptySite() const;//TODO inline???
	bool isBindToAny() const;
};

/** \brief Class of an agent declared in a mixture.
 * Stores all site information of a kappa declared
 * agent in a mixture.
 */
class Pattern::Agent {
	short signId; //signature ID
	std::unordered_map<small_id,Site> interface;
	map<small_id,list<Agent*> > parents;
	map<small_id,list<Agent*> > childs;
	mutable list<pair<const Mixture::Component*,small_id> > includedIn;

public:
	Agent(short name_id);
	~Agent();

	short getId() const;
	/** \brief Includes the site with id name_id in this agent.
	 * The new site is added to a map with the default values,
	 * like it has no declared value or link status. i.e.
	 * (small_id)-1 and FREE.
	 */
	Site& addSite(short name_id);
	void setSiteValue(small_id mix_site,small_id label);
	void setSiteValue(small_id mix_site,int val);
	void setSiteValue(small_id mix_site,float val);

	//void setSiteLink(short mix_site,LinkType l);
	void setLinkPtrn(small_id trgt_site,small_id ag_ptrn,small_id site_ptrn);

	const Site& getSite(short id) const;
	bool operator==(const Agent& a) const;

	/** \brief Compare two agents.
	 * @return 0 if equal, 1 if this contains 'a',
	 * -1 if 'a' contains this, throws False if none.
	 */
	int compare(const Agent& a,set<small_id>& already_done) const;

	void addParent(small_id id,Agent *a);
	void addParents(small_id id,const list<Agent*>& la);
	void addChild(small_id id,Agent *a);
	void addChilds(small_id id,const list<Agent*>& la);
	const list<Agent*>& getParentPatterns(small_id id) const;
	const list<Agent*>& getChildPatterns(small_id id) const;
	const map<small_id,list<Agent*> >& getParentPatterns() const;
	const map<small_id,list<Agent*> >& getChildPatterns() const;

	void addCc(const Mixture::Component* cc,small_id id) const;
	const list<pair<const Mixture::Component*,small_id> >& getIncludes() const;

	const unordered_map<small_id,Site>::const_iterator begin() const;
	const unordered_map<small_id,Site>::const_iterator end() const;

	string toString( const Environment& env, short mixAgId=-1,
			map<ag_st_id,short> bindLabels = map<ag_st_id,short>() ) const;
};

/** \brief Defines a set of agents that are explicitly connected by sites.
 * Class Mixture is initialized empty and then filled with agents and links.
 * After that, setGraph() must be called to reorder agents and produce graph,
 * making the mixture comparable and ready to be declared in the environment.
 *
 */
class Mixture::Component : public Pattern {
	short_id id;
	vector<Agent*> agents;
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
	short addAgent(Mixture::Agent* a);
	void addLink(const pair<ag_st_id,ag_st_id> &lnk,const map<short,short> &mask);

	size_t size() const override;
	void setId(short_id i);
	short_id getId() const;

	const vector<Agent*>::const_iterator begin() const;
	const vector<Agent*>::const_iterator end() const;

	vector<small_id> setGraph();
	const map<ag_st_id,ag_st_id>& getGraph() const;
	string toString(const Environment& env) const;

	const Agent& getAgent(small_id ag_id ) const override;

	/** \brief Returns agent and site ids of site-link
	 */
	ag_st_id follow(small_id ag_id,small_id site) const;

	bool operator==(const Mixture::Component &m) const;
};

} /* namespace pattern */

#endif /* PATTERN_MIXTURE_H_ */
