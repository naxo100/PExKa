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


namespace matching {

//using Node = state::Node;

class InjRandContainer {
protected:
	list<CcInjection*> freeInjs;
	const pattern::Mixture::Component& cc;
	virtual const list<Injection*>& insert(CcInjection* inj,const state::State& state) = 0;
	virtual CcInjection* newInj() const = 0;

public:

	InjRandContainer(const pattern::Mixture::Component& _cc);
	virtual ~InjRandContainer();
	virtual const Injection& chooseRandom(default_random_engine& randGen) const = 0;
	virtual size_t count() const = 0;
	virtual FL_TYPE partialReactivity() const = 0;

	virtual const list<Injection*>& emplace(Node& node,two<std::set<state::Internal*> > &port_lists,
			const state::State& state,small_id root = 0);
	virtual const list<Injection*>& emplace(Injection* base_inj,map<Node*,Node*>& mask,
			const state::State& state);
	virtual void erase(Injection* inj) = 0;

	virtual void selectRule(int rid,small_id cc) const;
	//vector<CcInjection*>::iterator begin();
	//vector<CcInjection*>::iterator end();
};


class InjRandSet : public InjRandContainer {
	size_t counter;
	size_t multiCount;
	vector<CcInjection*> container;
	const list<Injection*>& insert(CcInjection* inj,const state::State& state) override;
	virtual CcInjection* newInj() const override;
public:
	InjRandSet(const pattern::Mixture::Component& _cc);
	~InjRandSet();
	const Injection& chooseRandom(default_random_engine& randGen) const override;
	size_t count() const override;
	virtual FL_TYPE partialReactivity() const;

	/*Injection* emplace(Node& node,two<std::list<state::Internal*> > &port_lists,
			const state::State& state,small_id root = 0) override;*/
	//Injection* emplace(Injection* base_inj,map<Node*,Node*>& mask) override;
	void erase(Injection* inj) override;

	//vector<CcInjection*>::iterator begin();
	//vector<CcInjection*>::iterator end();
};


class InjRandTree : public InjRandContainer {
	//list<FL_TYPE> hints;

	list<CcInjection*> freeInjs;
	list<CcInjection*> infList;

	FL_TYPE average;
	unsigned counter;
	map<int,map<small_id,distribution_tree::DistributionTree<CcValueInj>*> > roots;

	mutable pair<int,small_id> selected_root;

	const list<Injection*>& insert(CcInjection *inj,const state::State& state) override;
	virtual CcInjection* newInj() const override;

public:
	InjRandTree(const pattern::Mixture::Component& cc);
	const Injection& chooseRandom(default_random_engine& randGen) const override;
	size_t count() const override;
	virtual FL_TYPE partialReactivity() const;

	void erase(Injection* inj) override;


	void selectRule(int rid,small_id cc) const override;
	//void addHint(FL_TYPE value);
};

}

#endif /* SRC_MATCHING_INJRANDSET_H_ */
