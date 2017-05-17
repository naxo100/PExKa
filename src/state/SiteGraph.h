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
	//link(node,site_id)
	pair<Node*,small_id> link;
	//link(node_address,site_id) - for marshalize?
	//pair<unsigned,short> fLink;
	Internal();

};

class SiteGraph::Node {
	pop_size n;
	short_id signId;
	big_id address;
	Internal *interface;
public:
	Node(short sign_id,short intf_size);
	Node(const pattern::Signature& sign);
	~Node();
	void alloc(big_id addr);

	template<typename T>
	void setState(small_id  site_id,T value);
	void setLink(small_id site_src,Node* lnk,small_id site_trgt);

	void setCount(pop_size q);
	pop_size getCount() const;


};





} /* namespace state */

#endif /* STATE_SITEGRAPH_H_ */
