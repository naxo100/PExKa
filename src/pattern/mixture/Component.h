/*
 * Component.h
 *
 *  Created on: Jan 21, 2019
 *      Author: naxo100
 */

#ifndef SRC_PATTERN_MIXTURE_COMPONENT_H_
#define SRC_PATTERN_MIXTURE_COMPONENT_H_

#include "Mixture.h"

namespace pattern {

/** \brief Defines a set of agents that are explicitly connected by sites.
 * Class Mixture is initialized empty and then filled with agents and links.
 * After that, setGraph() must be called to reorder agents and produce graph,
 * making the mixture comparable and ready to be declared in the environment.
 *
 */
class Mixture::Component : public Pattern {
	friend class Mixture;
	short_id id;
	vector<Agent*> agents;
	union {
		list<ag_st_id> *links;
		//(comp_ag_id,site_id) -> (comp_ag_id,site_id)
		map<ag_st_id,ag_st_id> *graph;
	};

	list<pair<const simulation::Rule&,small_id>> deps;
	//map<big_id,ag_st_id> auxiliars;

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

	//void setAuxCoords(const std::map<std::string,std::tuple<int,small_id,small_id>>& aux_coords);

	/** \brief Returns agent and site ids of site-link
	 * exception-safe
	 * return ag_id,site on fail
	 */
	ag_st_id follow(small_id ag_id,small_id site) const;

	bool operator==(const Mixture::Component &m) const;

	void addRateDep(const simulation::Rule& dep,small_id cc);
	const list<pair<const simulation::Rule&,small_id>>& getRateDeps() const;
};


} /* namespace pattern */

#endif /* SRC_PATTERN_MIXTURE_COMPONENT_H_ */
