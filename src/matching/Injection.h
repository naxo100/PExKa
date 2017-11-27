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
#include <vector>
#include "../util/params.h"
#include "../state/SiteGraph.h"
#include "../pattern/Mixture.h"


namespace matching {

using namespace std;

using Node = state::Node;

class Injection {
	const pattern::Pattern& ptrn;
public:
	Injection(const pattern::Pattern& ptrn);
	virtual ~Injection();
	virtual const vector<Node*>& getEmbedding() const = 0;

	virtual bool isTrashed() const = 0;

	virtual void codomain(Node* injs[],set<Node*> cod) const = 0;

	virtual unsigned count() const = 0;

	const pattern::Pattern& pattern() const;

	bool operator< (const Injection& inj) const;

};

class CcInjection : public Injection {
	//map<small_id,big_id> ccAgToNode;//unordered
	vector<Node*> ccAgToNode;
	mid_id address;
	//const pattern::Mixture::Component& cc;//cc_id

public:
	CcInjection(const pattern::Mixture::Component& cc,Node& node,
			two<list<state::Node::Internal*> >& port_list,small_id root = 0);
	~CcInjection();

	const vector<Node*>& getEmbedding() const override;

	bool isTrashed() const override;

	void codomain(Node* injs[],set<Node*> cod) const override;

	unsigned count() const override;
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
class InjSet : public set<Injection*> {
	unsigned counter;
public:
	const Injection& chooseRandom() const;
	unsigned count() const;
};

} /* namespace simulation */

#endif /* SRC_SIMULATION_INJECTION_H_ */
