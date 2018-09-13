/*
 * InjRandSet.h
 *
 *  Created on: Jun 26, 2018
 *      Author: naxo100
 */

#ifndef SRC_MATCHING_INJRANDSET_H_
#define SRC_MATCHING_INJRANDSET_H_

#include "Injection.h"

namespace matching {

//using Node = state::Node;

class InjRandSet {

public:
	virtual ~InjRandSet();
	virtual const Injection& chooseRandom(default_random_engine& randGen) const = 0;
	virtual FL_TYPE count() const = 0;

	virtual Injection* emplace(Node& node,two<std::list<state::Internal*> > &port_lists,
			small_id root = 0) = 0;
	virtual Injection* emplace(Injection* base_inj,map<Node*,Node*>& mask) = 0;
	virtual void erase(Injection* inj) = 0;

	//vector<CcInjection*>::iterator begin();
	//vector<CcInjection*>::iterator end();
};


class MultiInjSet : public InjRandSet {
	size_t counter;
	size_t multiCount;
	list<CcInjection*> freeInjs;
	vector<CcInjection*> container;
	const pattern::Mixture::Component& cc;
	void insert(CcInjection* inj);
public:
	MultiInjSet(const pattern::Mixture::Component& _cc);
	~MultiInjSet();
	const Injection& chooseRandom(default_random_engine& randGen) const override;
	FL_TYPE count() const override;

	Injection* emplace(Node& node,two<std::list<state::Internal*> > &port_lists,
			small_id root = 0) override;
	Injection* emplace(Injection* base_inj,map<Node*,Node*>& mask) override;
	void erase(Injection* inj) override;

	//vector<CcInjection*>::iterator begin();
	//vector<CcInjection*>::iterator end();
};

template <class Cont,typename T>
class InjTree : InjRandSet {
	struct Node {
		T value;
		vector<Injection*> equals; //equals
		Cont *smaller,*greater;//smaller,greater
		Node *left,*right;
	};
	const pattern::Mixture::Component& cc;
	Node root;
	list<T> hints;

public:
	InjTree(const pattern::Mixture::Component& cc);
	const Injection& chooseRandom(default_random_engine& randGen) const override;
	FL_TYPE count() const override;

	Injection* emplace(const pattern::Mixture::Component& cc,Node& node,
			two<std::list<state::Internal*> > &port_lists,small_id root = 0) override;
	Injection* emplace(Injection* base_inj,map<Node*,Node*>& mask) override;
	void erase(Injection* inj) override;

	void addHint(T value);
};

}

#endif /* SRC_MATCHING_INJRANDSET_H_ */
