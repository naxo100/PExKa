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

Injection* InjRandContainer::emplace(Node& node,
		two<std::set<state::Internal*> > &port_lists,const state::State& state,small_id root) {
	CcInjection* inj;
	if(freeInjs.empty()){
		inj = newInj();
		freeInjs.push_back(inj);
	}
	else {
		inj = freeInjs.front();
	}
	if(inj->reuse(cc,node,port_lists,state,root) == false)
		return nullptr;
	insert(inj,state);
	freeInjs.pop_front();
	return inj;
}

Injection* InjRandContainer::emplace(Injection* base_inj,map<Node*,Node*>& mask,const state::State& state){
	CcInjection* inj;
	if(freeInjs.empty()){
		inj = static_cast<CcInjection*>(base_inj->clone(mask));
		freeInjs.push_back(inj);
	}
	else
		inj = freeInjs.front();

	inj->copy(static_cast<CcInjection*>(base_inj),mask);//TODO static_cast??
	//container[inj->address];
	insert(inj,state);
	freeInjs.pop_front();

/*#if DEBUG
	for(auto inj2 : container)
		if(inj != inj2 && *inj == *inj2)
			throw invalid_argument("InjSet cannot contain the same injection twice.");
#endif*/
	return inj;
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

const Injection& InjRandSet::choose(unsigned id) const {
	if(id < container.size() - multiCount)
			return *container[id + multiCount];
	else{
		id -= container.size() - multiCount;
		auto it = container.begin();
		while(id > (*it)->count()){
			id -= (*it)->count();
			it++;
		}
		return **it;
	}
}

void InjRandSet::insert(CcInjection* inj,const state::State& state){
	inj->alloc(container.size());
	container.push_back(inj);
	if(inj->count() > 1){
		counter += inj->count();
		multiCount++;
	}
	else
		counter++;
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

FL_TYPE InjRandSet::sumInternal(const expressions::BaseExpression* aux_func,
			const map<string,two<small_id>>& aux_map, const state::State &state) const {
	expressions::AuxMap aux_values;
	auto func = [&](const CcInjection* inj) -> FL_TYPE {
		for(auto aux_coords : aux_map)
			aux_values[aux_coords.first] = inj->getEmbedding()[0][aux_coords.second.first].
				getInternalState(aux_coords.second.second).valueAs<FL_TYPE>();
		return aux_func->getValue(state, move(aux_values)).valueAs<FL_TYPE>();
	};
	FL_TYPE sum = 0;
	for(auto inj : container)
		sum += func(inj);
	return sum;
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

void InjRandTree::insert(CcInjection* inj,const state::State& state) {
	auto ccval_inj = static_cast<CcValueInj*>(inj);
	for(auto& rid_ccnode : roots){// for each rule that is using this pattern
		for(auto& cc_node : rid_ccnode.second){// for each CC in the LHS that use this pattern
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
			for(auto& aux_func : r.getReduction().aux_functions){
				if(aux_values.count(aux_func.first))//TODO remove when fix reductions (one per CC and not per aux)
					val *= aux_func.second->getValue(state, move(aux_values)).valueAs<FL_TYPE>();
			}
			cc_node.second->push(ccval_inj,val);//TODO static cast?
		}
	}
	counter += inj->count();
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

const Injection& InjRandTree::choose(unsigned id) const {
	return *(roots.begin()->second.begin()->second->choose(id).first);
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


FL_TYPE InjRandTree::sumInternal(const expressions::BaseExpression* aux_func,
			const map<string,two<small_id>>& aux_map, const state::State &state) const {
	expressions::AuxMap aux_values;
	function<FL_TYPE (const CcValueInj*)> func = [&](const CcValueInj* inj) -> FL_TYPE {
		for(auto aux_coords : aux_map)
			aux_values[aux_coords.first] = inj->getEmbedding()[0][aux_coords.second.first].
				getInternalState(aux_coords.second.second).valueAs<FL_TYPE>();
		return aux_func->getValue(state, move(aux_values)).valueAs<FL_TYPE>();
	};
	return roots.begin()->second.begin()->second->sumInternal(func);
}


}

