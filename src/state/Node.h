/*
 * Node.h
 *
 *  Created on: Oct 18, 2017
 *      Author: naxo
 */

#ifndef SRC_STATE_NODE_H_
#define SRC_STATE_NODE_H_

#include <utility>
#include <map>
#include <queue>
#include "../pattern/Signature.h"
#include "../pattern/Mixture.h"
//#include "../matching/Injection.h"

namespace matching {
	class InjSet;
	class InjRandSet;
	class Injection;
}

namespace state {

class SiteGraph;
struct EventInfo;

using namespace std;

class Node {
public:
	struct Internal;
protected:
	short_id signId;
	big_id address;
	Internal *interface;
	small_id intfSize;
	matching::InjSet* deps;

	Node(const Node& node);
	Node& operator=(const Node& node);
public:
	Node(short sign_id,short intf_size);
	Node(const pattern::Signature& sign);
	Node(const Node& node,const map<Node*,Node*>& mask);
	virtual ~Node();

	void copyDeps(const Node& node,EventInfo& ev,matching::InjRandSet* injs);//unsafe
	void alloc(big_id addr);
	big_id getAddress() const;

	template<typename T>
	void setState(small_id  site_id,T value);
	void setLink(small_id site_src,Node* lnk,small_id site_trgt);


	virtual void removeFrom(EventInfo& ev,matching::InjRandSet* injs,SiteGraph& graph);
	virtual void changeIntState(EventInfo& ev,matching::InjRandSet* injs,small_id id,small_id value);
	virtual void unbind(EventInfo& ev,matching::InjRandSet* injs,small_id id,bool side_eff = false);
	virtual void bind(EventInfo& ev,matching::InjRandSet* injs,small_id id,Node* trgt_node,small_id trgt_site,bool side_eff = false);


	//inline?? TODO
	Internal* begin();
	Internal* end();

	void setCount(pop_size q);
	virtual pop_size getCount() const;

	short_id getId() const;

	/** \brief Tests whether a site of this node matches with a mixture site.
	 * Tests if interface[id_site.first] matches with id_site.second.
	 * Stores in port_list every site_id that needs to match, used later to
	 * save injections. Also it stores in match_queue every pair
	 * (to_follow_,Node*) that will be matched later.
	 */
	Node* test(const pair<small_id,pattern::Mixture::Site>& id_site,
			two<list<Internal*> > &port_list) const;

	//unsafe
	const SomeValue& getInternalState(small_id);
	const pair<Node*,small_id>& getLinkState(small_id);

	void addDep(matching::Injection* inj);
	bool removeDep(matching::Injection* inj);
	pair<matching::InjSet*,matching::InjSet*>& getLifts(small_id site);
	//DEBUG
	virtual string toString(const pattern::Environment &env,bool show_binds = false) const;

};

struct Node::Internal {
	SomeValue val;
	pair<Node*,small_id> link;
	//for value, for link //TODO refs????
	pair<matching::InjSet*,matching::InjSet*> deps;//pointers to accept forward declaration of InjSet
	Internal();
	~Internal();

	void negativeUpdate(EventInfo& ev,matching::InjRandSet* injs);
	static void negativeUpdate(EventInfo& ev,matching::InjRandSet* injs,matching::InjSet* deps);


	string toString(const pattern::Signature::Site& s,bool show_binds = false) const;

};

class MultiNode;

class SubNode : public Node {
	MultiNode& cc;
	small_id multiId;
public:
	SubNode(const pattern::Signature& sign,MultiNode& multi);
	~SubNode();
	pop_size getCount() const override;


	void removeFrom(EventInfo& ev,matching::InjRandSet* injs,SiteGraph& graph) override;

	void changeIntState(EventInfo& ev,matching::InjRandSet* injs,small_id id,small_id value) override;
	void unbind(EventInfo& ev,matching::InjRandSet* injs,small_id id,bool side_eff = false) override;
	void bind(EventInfo& ev,matching::InjRandSet* injs,small_id id,
			Node* trgt_node,small_id trgt_site,bool side_eff = false) override;

	/*bool test(const pair<small_id,pattern::Mixture::Site>& id_site,
			std::queue<pair<small_id,Node&> > &match_queue,
			two<list<Internal*> > &port_list) const;*/

	string toString(const pattern::Environment& env,bool show_binds = false) const override;
};

class MultiNode {
	pop_size population;

	SubNode** cc;
	small_id count;

	~MultiNode();
public:
	MultiNode(unsigned n,pop_size pop);

	pop_size getPopulation() const;

	small_id add(SubNode* node);
	void remove();
	void dec(EventInfo& ev);

	class OutOfInstances : exception {};
};


/** \brief Structure that stores the information related to an event.
 */
struct EventInfo {
	//map of emb LHS [cc_id][ag_id]
	Node*** emb;
	small_id cc_count;
	//map of new_nodes RHS
	Node** fresh_emb;
	//node_address,site_id
	std::multimap<Node*,small_id> side_effects;
	//perturbation_ids
	std::set<mid_id> pert_ids;
	//null events
	small_id warns;

	//new cc derived from multinode
	map<Node*,Node*> new_cc;
	//mask for new injections, nullptr are erased injs
	map<matching::Injection*,matching::Injection*> inj_mask;

	set<const pattern::Pattern*> to_update;

	EventInfo();
	~EventInfo();

};

} /* namespace state */

#endif /* SRC_STATE_NODE_H_ */
