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
};


struct SiteGraph::Internal {
	union {
		short lVal;
		int iVal;
		float fVal;
	};

	pair<short,short> link;
};

class SiteGraph::Node {
	short name;
	long address;
	Internal *interface;
};





} /* namespace state */

#endif /* STATE_SITEGRAPH_H_ */
