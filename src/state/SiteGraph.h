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
#include "Node.h"
#include "../expressions/AlgExpression.h"
#include "../pattern/Signature.h"


namespace pattern {
	class Environment;
}
namespace state {

using namespace std;

class SiteGraph {
public:
	SiteGraph();
	~SiteGraph();

	Node** addComponents(unsigned n,const pattern::Mixture::Component& cc,
			const State& state);

	/** \brief Put a node in the sitegraph.
	 * Put a node in the sitegraph, set its address,
	 * and use free space in the container if any.
	 */
	void allocate(Node* n);
	/** \brief Put a subnode in the graph at initialization.
	 * Put a subnode in the graph at the beginning of container,
	 * swapping the first normal node to the last position in
	 * container, and set both addresses. Use only at initialization.
	 */
	void allocate(SubNode* n);
	void remove(Node* n);
	void remove(SubNode* n);
	size_t getNodeCount() const;
	vector<Node*>::iterator begin();
	vector<Node*>::iterator end();
	size_t getPopulation() const;
	void decPopulation(size_t pop = 1);

	string toString(const pattern::Environment& env) const;


protected:
	Node& addNode(short sign_id);
	Node& getNode(size_t address);
	vector<Node*> container;
	size_t fresh;
	size_t nodeCount;
	size_t population;
	big_id subNodeCount;
	list<size_t> free;


};









} /* namespace state */

#endif /* STATE_SITEGRAPH_H_ */
