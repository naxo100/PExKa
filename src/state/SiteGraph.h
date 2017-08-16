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
//#include "../pattern/Environment.h"
#include "../util/params.h"
#include "../state/AlgExpression.h"
#include "../pattern/Signature.h"
#include "../matching/Injection.h"

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


protected:
	Node& addNode(short sign_id);
	Node& getNode(size_t address);
	vector<Node*> container;
	size_t fresh;
	size_t nodeCount;
	list<size_t> free;


};


struct SiteGraph::Internal {
	SomeValue val;
	pair<Node*,small_id> link;
	//for value, for link //TODO refs????
	pair<matching::InjSet,matching::InjSet> deps;
	Internal();

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



	//unsafe
	Internal& getInternalState(small_id);
	pair<matching::InjSet,matching::InjSet>& getLifts(small_id site);


	//DEBUG
	string toString(const pattern::Environment &env) const;

};





} /* namespace state */

#endif /* STATE_SITEGRAPH_H_ */
