/*
 * InjRandSet.h
 *
 *  Created on: Jun 26, 2018
 *      Author: naxo100
 */

#ifndef SRC_MATCHING_INJRANDSET_H_
#define SRC_MATCHING_INJRANDSET_H_

#include "Injection.h"
#include "../data_structs/DistributionTree.h"
#include "../simulation/Rule.h"


namespace matching {

//using Node = state::Node;

class InjRandContainer {
protected:
	list<CcInjection*> freeInjs;
	const pattern::Mixture::Component& cc;
	virtual void insert(CcInjection* inj,const state::State& state) = 0;
	virtual CcInjection* newInj() const = 0;

public:

	InjRandContainer(const pattern::Mixture::Component& _cc);
	virtual ~InjRandContainer();
	virtual const Injection& chooseRandom(default_random_engine& randGen) const = 0;
	virtual const Injection& choose(unsigned id) const  = 0;
	virtual size_t count() const = 0;
	virtual FL_TYPE partialReactivity() const = 0;

	virtual Injection* emplace(Node& node,two<std::set<state::Internal*> > &port_lists,
			const state::State& state,small_id root = 0);
	virtual Injection* emplace(Injection* base_inj,map<Node*,Node*>& mask,
			const state::State& state);
	virtual void erase(Injection* inj) = 0;

	virtual void selectRule(int rid,small_id cc) const;

	virtual FL_TYPE sumInternal(const expressions::BaseExpression* aux_func,
			const map<string,two<small_id>>& aux_map, const state::State &state) const = 0;
	//vector<CcInjection*>::iterator begin();
	//vector<CcInjection*>::iterator end();
};


class InjRandSet : public InjRandContainer {
	size_t counter;
	size_t multiCount;
	vector<CcInjection*> container;
	void insert(CcInjection* inj,const state::State& state) override;
	virtual CcInjection* newInj() const override;
public:
	InjRandSet(const pattern::Mixture::Component& _cc);
	~InjRandSet();
	const Injection& chooseRandom(default_random_engine& randGen) const override;
	const Injection& choose(unsigned id) const override;
	size_t count() const override;
	virtual FL_TYPE partialReactivity() const;

	/*Injection* emplace(Node& node,two<std::list<state::Internal*> > &port_lists,
			const state::State& state,small_id root = 0) override;*/
	//Injection* emplace(Injection* base_inj,map<Node*,Node*>& mask) override;
	void erase(Injection* inj) override;

	FL_TYPE sumInternal(const expressions::BaseExpression* aux_func,
			const map<string,two<small_id>>& aux_map, const state::State &state) const override;

	//vector<CcInjection*>::iterator begin();
	//vector<CcInjection*>::iterator end();
};


class InjRandTree : public InjRandContainer {
	static const float MAX_INVALIDATIONS;//3%
	//list<FL_TYPE> hints;

	//list<CcInjection*> freeInjs;
	list<CcInjection*> infList;

	struct Root {
		distribution_tree::DistributionTree<CcValueInj>* tree;
		two<FL_TYPE> second_moment;//(sum x*x,sum x) at validation
		bool is_valid;

		Root();
	};
	unsigned counter;
	// (r_id,cc_index) -> contribution to rule-rate per cc
	map<pair<int,small_id>,Root*> roots,roots_to_push;
	mutable int invalidations;

	mutable pair<int,small_id> selected_root;

	void insert(CcInjection *inj,const state::State& state) override;
	virtual CcInjection* newInj() const override;

public:
	InjRandTree(const pattern::Mixture::Component& cc,const vector<simulation::Rule::Rate>& rates);
	~InjRandTree();
	const Injection& chooseRandom(default_random_engine& randGen) const override;
	const Injection& choose(unsigned id) const override;
	size_t count() const override;
	virtual FL_TYPE partialReactivity() const;

	FL_TYPE getM2() const;

	void erase(Injection* inj) override;


	void selectRule(int rid,small_id cc) const override;

	FL_TYPE sumInternal(const expressions::BaseExpression* aux_func,
				const map<string,two<small_id>>& aux_map, const state::State &state) const override;
	//void addHint(FL_TYPE value);
};

}

#endif /* SRC_MATCHING_INJRANDSET_H_ */
