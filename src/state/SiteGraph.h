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
#include "../util/params.h"
#include "../state/AlgExpression.h"
#include "../pattern/Signature.h"

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
	//for value, for link
	pair<matching::InjSet&,matching::InjSet&> deps;
	Internal();

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


	inline Internal* begin();
	inline Internal* end();

	void setCount(pop_size q);
	pop_size getCount() const;



	//unsafe
	Internal& getInternalState(small_id);
	pair<matching::InjSet&,matching::InjSet&>& getLifts(small_id site);


};





} /* namespace state */

#endif /* STATE_SITEGRAPH_H_ */
