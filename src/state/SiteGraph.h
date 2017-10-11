/*
 * SiteGraph.h
 *
 *  Created on: Mar 23, 2016
 *      Author: naxo
 */

#ifndef STATE_SITEGRAPH_H_
#define STATE_SITEGRAPH_H_

#include <utility> //pair
#include <list>
#include <vector>
#include <queue>
//#include "../pattern/Environment.h"
#include "../util/params.h"
#include "../state/AlgExpression.h"
#include "../pattern/Signature.h"
#include "../pattern/Mixture.h"
//#include "../matching/Injection.h"

namespace matching {
	class InjSet;
}

namespace pattern {
	class Environment;
}
namespace state {

using namespace std;

class SiteGraph {
public:
	class Node;
	struct Internal;
	SiteGraph();
	~SiteGraph();

	void allocate(Node* n);
	size_t getNodeCount() const;
	vector<Node*>::iterator begin();
	vector<Node*>::iterator end();
	size_t getPopulation() const;


protected:
	Node& addNode(short sign_id);
	Node& getNode(size_t address);
	vector<Node*> container;
	size_t fresh;
	size_t nodeCount;
	size_t population;
	list<size_t> free;


};


struct SiteGraph::Internal {
	SomeValue val;
	pair<Node*,small_id> link;
	//for value, for link //TODO refs????
	pair<matching::InjSet*,matching::InjSet*> deps;//pointers to accept forward declaration of InjSet
	Internal();
	~Internal();

	string toString(const pattern::Signature::Site& s) const;

};

class SiteGraph::Node {
	pop_size n;
	short_id signId;
	big_id address;
	Internal *interface;
	small_id intfSize;
public:
	Node(short sign_id,short intf_size);
	Node(const pattern::Signature& sign);
	~Node();
	void alloc(big_id addr);

	template<typename T>
	void setState(small_id  site_id,T value);
	void setLink(small_id site_src,Node* lnk,small_id site_trgt);

	//inline?? TODO
	Internal* begin();
	Internal* end();

	void setCount(pop_size q);
	pop_size getCount() const;

	short_id getId() const;

	/** \brief Tests whether a site of this node matches with a mixture site.
	 * Tests if interface[id_site.first] matches with id_site.second.
	 * Stores in port_list every site_id that needs to match, used later to
	 * save injections. Also it stores in match_queue every pair
	 * (to_follow_,Node*) that will be matched later.
	 */
	bool test(const pair<small_id,pattern::Mixture::Site>& id_site,
			std::queue<pair<small_id,Node&> > &match_queue,
			two<list<Internal*> > &port_list) const;

	//unsafe
	Internal& getInternalState(small_id);
	pair<matching::InjSet*,matching::InjSet*>& getLifts(small_id site);


	//DEBUG
	string toString(const pattern::Environment &env) const;

};





} /* namespace state */

#endif /* STATE_SITEGRAPH_H_ */
