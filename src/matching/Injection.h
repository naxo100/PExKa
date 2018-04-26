/*
 * Injection.h
 *
 *  Created on: Apr 11, 2017
 *      Author: naxo
 */

#ifndef SRC_MATCHING_INJECTION_H_
#define SRC_MATCHING_INJECTION_H_

#include <map>
#include <set>
#include <unordered_set>
#include <vector>
#include <list>
#include <random>
#include "../util/params.h"
#include "../state/SiteGraph.h"
#include "../pattern/Mixture.h"


namespace matching {

using namespace std;

using Node = state::Node;

class InjRandSet;

class Injection {
	friend class InjRandSet;
	const pattern::Pattern& ptrn;
	size_t address;
	virtual Injection* clone(const map<Node*,Node*>& mask) const = 0;
public:
	Injection(const pattern::Pattern& ptrn);
	virtual ~Injection();
	virtual const vector<Node*>& getEmbedding() const = 0;

	virtual bool isTrashed() const;

	virtual void codomain(Node* injs[],set<Node*> cod) const = 0;

	virtual size_t count() const = 0;

	const pattern::Pattern& pattern() const;

	void alloc(size_t addr);

	//bool operator< (const Injection& inj) const;
	virtual bool operator==(const Injection& inj) const = 0;

};

class CcInjection : public Injection {
	friend class InjRandSet;
	//map<small_id,big_id> ccAgToNode;//unordered
	vector<Node*> ccAgToNode;
	//const pattern::Mixture::Component& cc;//cc_id

	CcInjection(const pattern::Pattern& _cc);
	CcInjection(const pattern::Mixture::Component& cc,Node& node,
			two<list<state::Node::Internal*> >& port_list,small_id root = 0);
	Injection* clone(const map<Node*,Node*>& mask) const override;
	void copy(const CcInjection* inj,const map<Node*,Node*>& mask);
public:
	~CcInjection();

	void reuse(const pattern::Mixture::Component& cc,Node& node,
			two<list<state::Node::Internal*> >& port_list,small_id root = 0);

	const vector<Node*>& getEmbedding() const override;

	void codomain(Node* injs[],set<Node*> cod) const override;

	size_t count() const override;

	bool operator==(const Injection& inj) const override;
};

/*class MixInjection : Injection {
	big_id** agCcIdToNode;
	mid_id address;
	small_id coordinate;//mix_id

public:
	MixInjection(small_id size);
	~MixInjection();

	const vector<Node*>& getEmbedding() const override;

	bool isTrashed() const override;

	void codomain(Node* injs[],set<Node*> cod) const override;
};*/

//TODO choose the best implementation of InjSet
//typedef set<Injection*> InjSet;
/*class InjSet : public set<Injection*> {
	//unsigned counter;
public:
	//const Injection& chooseRandom() const;
	//unsigned count() const;

};*/

class InjRandSet {
	size_t counter;
	size_t multiCount;
	list<CcInjection*> freeInjs;
	vector<CcInjection*> container;
	void insert(CcInjection* inj);
public:
	InjRandSet();
	~InjRandSet();
	const Injection& chooseRandom(default_random_engine& randGen) const;
	size_t count() const;

	Injection* emplace(const pattern::Mixture::Component& cc,Node& node,
			two<std::list<Node::Internal*> > &port_lists,small_id root = 0);
	Injection* emplace(Injection* base_inj,map<Node*,Node*>& mask);
	void erase(Injection* inj);

	vector<CcInjection*>::iterator begin();
	vector<CcInjection*>::iterator end();
};

} /* namespace simulation */

#endif /* SRC_SIMULATION_INJECTION_H_ */
