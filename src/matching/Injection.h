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
//#include "../state/SiteGraph.h"
#include "../pattern/mixture/Mixture.h"
#include "../data_structs/DistributionTree.h"

namespace state {
	class Node;
	struct Internal;
}

namespace matching {

using namespace std;

using Node = state::Node;

class InjRandSet;

/*struct try_match {
	const pattern::Mixture::Component* comp;
	Node* node;
	two<list<Internal*> > port_lists;
	small_id root;
};*/


class Injection {
protected:
	friend class InjRandSet;
	const pattern::Pattern& ptrn;
	int address;
public:
	Injection(const pattern::Pattern& ptrn);
	virtual Injection* clone(const map<Node*,Node*>& mask) const = 0;
	virtual ~Injection();
	virtual const vector<Node*>& getEmbedding() const = 0;

	virtual bool isTrashed() const = 0;

	virtual void codomain(vector<Node*>& injs,set<Node*>& cod) const = 0;

	virtual size_t count() const = 0;

	const pattern::Pattern& pattern() const;

	virtual void alloc(int addr);
	int getAddress() const;

	//bool operator< (const Injection& inj) const;
	virtual bool operator==(const Injection& inj) const = 0;

};

class CcInjection : public Injection {
	friend class InjRandSet;
	//map<small_id,big_id> ccAgToNode;//unordered
	vector<Node*> ccAgToNode;
	//const pattern::Mixture::Component& cc;//cc_id

public:
	CcInjection(const pattern::Pattern& _cc);
	CcInjection(const pattern::Mixture::Component& cc);
	Injection* clone(const map<Node*,Node*>& mask) const override;
	void copy(const CcInjection* inj,const map<Node*,Node*>& mask);
	~CcInjection();

	bool reuse(const pattern::Mixture::Component& cc,Node& node,
			two<set<state::Internal*> >& port_list,const state::State& state,small_id root = 0);

	const vector<Node*>& getEmbedding() const override;

	virtual bool isTrashed() const override;

	void codomain(vector<Node*>& injs,set<Node*>& cod) const override;

	size_t count() const override;

	bool operator==(const Injection& inj) const override;
};


/*namespace distribution_tree{
	template <typename T>
	class DistributionTree<T>;
}*/

class CcValueInj : public CcInjection {
	map<distribution_tree::DistributionTree<CcValueInj>*,int> containers;//TODO try with unordered_map
	//FL_TYPE value;
public:
	CcValueInj(const pattern::Mixture::Component& _cc);
	CcValueInj(const CcInjection& inj);

	virtual bool isTrashed() const override;

	void addContainer(distribution_tree::DistributionTree<CcValueInj>& cont,int addr);
	void selfRemove();
	//returns true if the containers list is empty
	bool removeContainer(distribution_tree::DistributionTree<CcValueInj>& cont);

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



} /* namespace simulation */

#endif /* SRC_SIMULATION_INJECTION_H_ */
