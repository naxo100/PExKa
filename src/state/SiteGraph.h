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
#include "../state/AlgExpression.h"
#include "../pattern/Signature.h"
#include "../pattern/Mixture.h"


namespace pattern {
	class Environment;
}
namespace state {

using namespace std;

class SiteGraph {
public:
	SiteGraph();
	~SiteGraph();

	void addComponents(unsigned n,const pattern::Mixture::Component& cc,
			const pattern::Environment& env);

	void allocate(Node* n);
	void remove(Node* n);
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









} /* namespace state */

#endif /* STATE_SITEGRAPH_H_ */
