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

using Node = state::SiteGraph::Node;

class Injection {
public:
	//virtual ~Injection() = 0;
	virtual const vector<Node*>& getEmbedding() const = 0;

	virtual bool isTrashed() const = 0;

	virtual void codomain(Node* injs[],set<Node*> cod) const = 0;

	bool operator< (const Injection& inj) const;
};

class CcInjection : public Injection {
	//map<small_id,big_id> ccAgToNode;//unordered
	vector<state::SiteGraph::Node*> ccAgToNode;
	mid_id address;
	small_id coordinate;//cc_id

public:
	CcInjection(const pattern::Mixture::Component& cc,state::SiteGraph::Node& node,
			two<list<state::SiteGraph::Internal*> >& port_list);
	~CcInjection();

	const vector<Node*>& getEmbedding() const override;

	bool isTrashed() const override;

	void codomain(Node* injs[],set<Node*> cod) const override;
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

public:
	const Injection& chooseRandom() const;
};

} /* namespace simulation */

#endif /* SRC_SIMULATION_INJECTION_H_ */
