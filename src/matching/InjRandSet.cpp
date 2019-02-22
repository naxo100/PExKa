/*
 * InjRandSet.cpp
 *
 *  Created on: Jun 26, 2018
 *      Author: naxo100
 */

#include "InjRandSet.h"
#include "../state/Node.h"
#include "../simulation/Rule.h"
#include "../state/State.h"
#include "../pattern/Environment.h"
#include "../pattern/mixture/Component.h"
#include "../pattern/mixture/Agent.h"

namespace matching {

InjRandContainer::InjRandContainer(const pattern::Mixture::Component& _cc) : cc(_cc){}

InjRandContainer::~InjRandContainer(){
	for(auto inj : freeInjs)
		delete inj;
}

const list<Injection*>& InjRandContainer::emplace(Node& node,
		two<std::set<state::Internal*> > &port_lists,const state::State& state,small_id root) {
	CcInjection* inj;
	static list<Injection*> empty;
	if(freeInjs.empty()){
		inj = newInj();
		freeInjs.push_back(inj);
	}
	else {
		inj = freeInjs.front();
	}
	if(inj->reuse(cc,node,port_lists,state,root) == false)
		return empty;
	auto& injs = insert(inj,state);
	freeInjs.pop_front();
	return injs;
}

const list<Injection*>& InjRandContainer::emplace(Injection* base_inj,map<Node*,Node*>& mask,const state::State& state){
	CcInjection* inj;
	if(freeInjs.empty()){
		inj = static_cast<CcInjection*>(base_inj->clone(mask));
		freeInjs.push_back(inj);
	}
	else
		inj = freeInjs.front();

	inj->copy(static_cast<CcInjection*>(base_inj),mask);//TODO static_cast??
	//container[inj->address];
	auto& injs = insert(inj,state);
	freeInjs.pop_front();

/*#if DEBUG
	for(auto inj2 : container)
		if(inj != inj2 && *inj == *inj2)
			throw invalid_argument("InjSet cannot contain the same injection twice.");
#endif*/
	return injs;
}

void InjRandContainer::selectRule(int rid,small_id cc) const {/*do nothing*/};

/********* MultiInjSet ********************/

InjRandSet::InjRandSet(const pattern::Mixture::Component& _cc) : InjRandContainer(_cc)
		,counter(0),multiCount(0) {
	container.reserve(100);
}

InjRandSet::~InjRandSet(){
	for(auto inj : container)
		delete inj;
}

//TODO better way to count?
size_t InjRandSet::count() const {
	size_t c = 0;
	for(size_t i = 0; i < multiCount; i++)
		c += container[i]->count();
	return c + container.size() - multiCount;
}

//TODO
const Injection& InjRandSet::chooseRandom(default_random_engine& randGen) const{
	auto uni = uniform_int_distribution<size_t>(0,count()-1);
	auto selection = uni(randGen);
	if(selection < container.size() - multiCount)
		return *container[selection + multiCount];
	else{
		selection -= container.size() - multiCount;
		auto it = container.begin();
		while(selection > (*it)->count()){
			selection -= (*it)->count();
			it++;
		}
		return **it;
	}
}

const list<Injection*>& InjRandSet::insert(CcInjection* inj,const state::State& state){
	static list<Injection*> ret;
	ret.clear();
	inj->alloc(container.size());
	container.push_back(inj);
	if(inj->count() > 1){
		counter += inj->count();
		multiCount++;
	}
	else
		counter++;
	ret.push_back(inj);
	return ret;
}

void InjRandSet::erase(Injection* inj){
	//if(container.empty())
	//	throw invalid_argument("InjRandSet is empty, what injection are you trying to delete?");
	if(inj->getAddress() < multiCount)
		multiCount--;
	freeInjs.push_back(static_cast<CcInjection*>(inj));
	if(container.size() > 1){
		container.back()->alloc(inj->getAddress());
		container[inj->getAddress()] = container.back();
	}
	inj->alloc(size_t(-1));//dealloc
	container.pop_back();
	counter -= inj->count();
}


CcInjection* InjRandSet::newInj() const{
	return new CcInjection(cc);
}


FL_TYPE InjRandSet::partialReactivity() const {
	return count();
}
/*vector<CcInjection*>::iterator InjRandSet::begin(){
	return container.begin();
}

vector<CcInjection*>::iterator InjRandSet::end(){
	return container.end();
}
*/


/************ InjRandTree *******************/


InjRandTree::InjRandTree(const pattern::Mixture::Component& _cc) :
		InjRandContainer(_cc),average(0),counter(0),selected_root(0,0){
	auto& deps = cc.getRateDeps();
	distribution_tree::Node<CcValueInj>* pointer = nullptr;
	for(auto r_cc : deps){
		if(r_cc.first.getReduction().aux_functions.size())
			roots[r_cc.first.getId()][r_cc.second] = new distribution_tree::Node<CcValueInj>(1.0);
		else
			roots[r_cc.first.getId()][r_cc.second] = nullptr;
	}

}

const list<Injection*>& InjRandTree::insert(CcInjection* inj,const state::State& state) {
	static list<Injection*> injs; //be careful TODO test performance using new
	injs.clear();
	for(auto& rid_ccnode : roots){
		for(auto& cc_node : rid_ccnode.second){
			auto& r = state.getEnv().getRules()[rid_ccnode.first];
			state::AuxMap aux_values;
			for(auto& aux : r.getLHS().getAux()){
				if(get<0>(aux.second) == cc_node.first)// to select proper CC
					aux_values[aux.first] =
							inj->getEmbedding()[get<1>(aux.second)]->
							getInternalState(get<2>(aux.second)).valueAs<FL_TYPE>();
				/*else
					aux_values[aux.first] = 1.0;//default factor-aux values TODO non factor-aux*/
			}
			auto val = 1.0;
			for(auto& aux_val : aux_values)
				val *= r.getReduction().aux_functions.at(aux_val.first)->
						getValue(state, move(aux_values)).valueAs<FL_TYPE>();
			auto new_inj = new CcValueInj(*inj);
			injs.push_back(new_inj);
			cc_node.second->push(new_inj,val);//TODO static cast?
		}
	}
	counter += inj->count();
	delete inj;//TODO do not delete
	return injs;
}


const Injection& InjRandTree::chooseRandom(default_random_engine& randGen) const {
	distribution_tree::DistributionTree<CcValueInj> *root = nullptr;
	if(roots.count(selected_root.first)){
		auto& root_map = roots.at(selected_root.first);
		if(root_map.count(selected_root.second))
			root = root_map.at(selected_root.second);
	}

	if(root){
		auto uni = uniform_real_distribution<FL_TYPE>(0,partialReactivity());
		auto selection = uni(randGen);
		return root->choose(selection);
		/*for(auto& cc_node : roots.at(selected_root.first)){
		if(selection > cc_node.second->total())
			selection -= cc_node.second->total();
		else
			return cc_node.second->choose(selection);*/
	}
	else{
		auto uni = uniform_int_distribution<unsigned int>(0,count());
		auto selection = uni(randGen);
		return *root->choose(selection).first;
	}
	//throw std::out_of_range("Selected injection out of range. [InjRandTree::chooseRandom()]");
}

void InjRandTree::erase(Injection* inj){
	auto val_inj = static_cast<CcValueInj*>(inj);
	val_inj->selfRemove();
	freeInjs.push_back(val_inj);
	counter -= inj->count();
}

CcInjection* InjRandTree::newInj() const{
	return new CcValueInj(cc);
}

size_t InjRandTree::count() const {
	return roots.begin()->second.begin()->second->total();
}


FL_TYPE InjRandTree::partialReactivity() const {
	distribution_tree::DistributionTree<CcValueInj> *root = nullptr;
	if(roots.count(selected_root.first)){
		auto& root_map = roots.at(selected_root.first);
		if(root_map.count(selected_root.second))
			root = root_map.at(selected_root.second);
	}
	if(root)
		return root->total();
	else
		return count();
}


void InjRandTree::selectRule(int rid,small_id cc) const {
	selected_root.first = rid;
	selected_root.second = cc;
};

}

