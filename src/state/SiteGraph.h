/*
 * SiteGraph.h
 *
 *  Created on: Mar 23, 2016
 *      Author: naxo
 */

#ifndef STATE_SITEGRAPH_H_
#define STATE_SITEGRAPH_H_

#include <utility> //pair

namespace state {

using namespace std;

class SiteGraph {
public:
	class Node;
	struct Internal;
	SiteGraph();
	~SiteGraph();

	Node& addNode(short sign_id);
	Node& getNode(size_t address);

};


struct SiteGraph::Internal {
	union {
		short lVal;
		int iVal;
		float fVal;
	};
	//link(node,site_id)
	pair<Node*,short> link;
	//link(node_address,site_id) - for marshalize?
	//pair<unsigned,short> fLink;

};

class SiteGraph::Node {
	short name;
	long address;
	Internal *interface;
public:
	Node(short sign_id,short intf_size);
	void setAddress(size_t addr);

	template<typename T>
	void setState(short site_id,T value);
	void setLink(short site_src,Node* lnk,short site_trgt);


};





} /* namespace state */

#endif /* STATE_SITEGRAPH_H_ */
