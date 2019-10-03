/*
 * InjRandSet.cpp
 *
 *  Created on: Jun 26, 2018
 *      Author: naxo100
 */

#include "InjRandSet.h"
#include "../state/Node.h"
//#include "../simulation/Rule.h"
#include "../state/State.h"
#include "../pattern/Environment.h"
#include "../pattern/mixture/Component.h"
#include "../pattern/mixture/Agent.h"

namespace matching {

InjRandContainer::InjRandContainer(const pattern::Mixture::Component& _cc) : cc(_cc){}

InjRandContainer::~InjRandContainer(){
	for(auto inj_it = freeInjs.begin(); inj_it != freeInjs.end(); inj_it++){
#ifdef DEBUG
		for(auto inj2_it = next(inj_it); inj2_it != freeInjs.end(); inj2_it++)
			if(*inj_it == *inj2_it)
				throw invalid_argument("inj is repeated in freeinjs!");
#endif
		delete *inj_it;
	}
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
	insert(inj,state);
	freeInjs.pop_front();

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
#ifdef DEBUG
	if(container.empty())
		throw out_of_range("InjRandSet is empty, what injection are you trying to delete?");
#endif
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

void InjRandSet::clear() {
	multiCount = 0;
	counter = 0;
	for(auto inj : container){
		inj->alloc(size_t(-1));
		freeInjs.push_back(inj);
	}
	container.clear();
}


CcInjection* InjRandSet::newInj() const{
	return new CcInjection(cc);
}


FL_TYPE InjRandSet::partialReactivity() const {
	return count();
}

FL_TYPE InjRandSet::sumInternal(const expressions::BaseExpression* aux_func,
			const map<string,two<small_id>>& aux_map, const state::State &state) const {
	expressions::AuxNames aux_values;
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

void InjRandSet::fold(const function<void (const Injection*)> func) const{
	for(auto inj : container)
		func(inj);
}


/*vector<CcInjection*>::iterator InjRandSet::begin(){
	return container.begin();
}

vector<CcInjection*>::iterator InjRandSet::end(){
	return container.end();
}
*/


/************ InjRandTree *******************/

const float InjRandTree::MAX_INVALIDATIONS = 0.03;//3%

InjRandTree::Root::Root() : tree(new distribution_tree::Node<CcValueInj>(1.0)),
		second_moment(-1.,0.),is_valid(false) {}

InjRandTree::InjRandTree(const pattern::Mixture::Component& _cc,const vector<simulation::Rule::Rate>& rates) :
		InjRandContainer(_cc),counter(0),invalidations(0),selected_root(0,0){
	auto& deps = cc.getRateDeps();
	//distribution_tree::Node<CcValueInj>* pointer = nullptr;
	for(auto r_cc : deps){//
		auto r_id = r_cc.first.getId();
		auto rid_cc = make_pair(r_id,r_cc.second);
		if(r_cc.first.getRate().getVarDeps() & expressions::BaseExpression::AUX){
			auto faux_ridcc = rates[r_id].base.aux_functions.at(r_cc.second);
			for(auto& root : roots)//dont create another root if there is one assoc. with the same aux.func.
				if(*faux_ridcc == *(rates[root.first.first].base.aux_functions.at(root.first.second)) ){
					roots[rid_cc] = root.second;
					break;
				}
			if(!roots.count(rid_cc)){
				roots[rid_cc] = new Root();
				roots_to_push[rid_cc] = roots[rid_cc];
			}
		}
		//else
		//	roots[r_cc.first.getId(),r_cc.second] = nullptr;//no need to make another tree for this root
	}

}

InjRandTree::~InjRandTree(){
	for(auto inj : infList)
		delete inj;
	//roots.begin()->second->tree->deleteContent();//delete only injs inside tree
	set<Root*> deleted;
	for(auto root : roots_to_push){
		root.second->tree->deleteContent();//delete only injs inside treeif(root.second && !deleted.count(root.second)){
		delete root.second;
	}
}

void InjRandTree::insert(CcInjection* inj,const state::State& state) {
	auto ccval_inj = static_cast<CcValueInj*>(inj);
	for(auto& ridcc_tree : roots_to_push){// for each (rid,cc_index) that is using this cc-pattern
		auto& r = state.getEnv().getRules()[ridcc_tree.first.first];
		state::AuxNames aux_values;
		for(auto& aux : r.getLHS().getAux()){
			if(get<0>(aux.second) == ridcc_tree.first.second)// to select proper CC
				aux_values[aux.first] =
						inj->getEmbedding()[get<1>(aux.second)]->
						getInternalState(get<2>(aux.second)).valueAs<FL_TYPE>();
			/*else
				aux_values[aux.first] = 1.0;//default factor-aux values TODO non factor-aux*/
		}
		FL_TYPE val = 1.0;
		auto ccaux_func = state.getRuleRate(r.getId()).base.aux_functions.at(ridcc_tree.first.second);
		val *= ccaux_func->getValue(state, move(aux_values)).valueAs<FL_TYPE>();
		if(val < 0.)
			throw invalid_argument("Partial reactivities cannot be negative.");
		ridcc_tree.second->tree->push(ccval_inj,val);//TODO static cast?
	}
	counter += inj->count();
	invalidations++;
}


const Injection& InjRandTree::chooseRandom(default_random_engine& randGen) const {
	distribution_tree::DistributionTree<CcValueInj> *tree = nullptr;
	if(roots.count(selected_root)){
		tree = roots.at(selected_root)->tree;
	}

	if(tree){
		auto uni = uniform_real_distribution<FL_TYPE>(0,partialReactivity());
		auto selection = uni(randGen);
		return tree->choose(selection);
		/*for(auto& cc_node : roots.at(selected_root.first)){
		if(selection > cc_node.second->total())
			selection -= cc_node.second->total();
		else
			return cc_node.second->choose(selection);*/
	}
	else{
		auto uni = uniform_int_distribution<unsigned int>(0,count());
		auto selection = uni(randGen);
		return *roots.begin()->second->tree->choose(selection).first;
	}
	//throw std::out_of_range("Selected injection out of range. [InjRandTree::chooseRandom()]");
}

const Injection& InjRandTree::choose(unsigned id) const {
	return *(roots.begin()->second->tree->choose(id).first);
}

void InjRandTree::erase(Injection* inj){
	auto val_inj = static_cast<CcValueInj*>(inj);
	val_inj->selfRemove();
	freeInjs.push_back(val_inj);
	counter -= inj->count();
	invalidations++;
}


void InjRandTree::clear() {
	counter = 0;
	invalidations = 0;
	list<CcValueInj*> frees;
	for(auto root : roots_to_push)
		root.second->tree->clear(&frees);
	freeInjs.insert(freeInjs.end(),frees.begin(),frees.end());
}

CcInjection* InjRandTree::newInj() const{
	return new CcValueInj(cc);
}

size_t InjRandTree::count() const {
	return roots.begin()->second->tree->total();
}


FL_TYPE InjRandTree::partialReactivity() const {
	distribution_tree::DistributionTree<CcValueInj> *tree = nullptr;
	if(roots.count(selected_root)){
		tree = roots.at(selected_root)->tree;
	}
	if(tree)
		return tree->total();
	else
		return count();
}

FL_TYPE InjRandTree::getM2() const {
	auto root = roots.at(selected_root);
	if(invalidations > MAX_INVALIDATIONS*count()){
		for(auto r : roots)
			r.second->is_valid = false;
		invalidations = 0;
	}
	if(!root->is_valid){
		root->second_moment = make_pair(root->tree->squares(),root->tree->total());
		root->is_valid = true;
	}
	auto val = sqrt(root->second_moment.first)*root->tree->total()/root->second_moment.second;
	return val*val;
}


void InjRandTree::selectRule(int rid,small_id cc) const {
	selected_root.first = rid;
	selected_root.second = cc;
};


FL_TYPE InjRandTree::sumInternal(const expressions::BaseExpression* aux_func,
			const map<string,two<small_id>>& aux_map, const state::State &state) const {
	expressions::AuxNames aux_values;
	function<FL_TYPE (const CcValueInj*)> func = [&](const CcValueInj* inj) -> FL_TYPE {
		for(auto& aux_coords : aux_map)
			aux_values[aux_coords.first] = inj->getEmbedding()[aux_coords.second.first]->
				getInternalState(aux_coords.second.second).valueAs<FL_TYPE>();
		return aux_func->getValue(state, move(aux_values)).valueAs<FL_TYPE>();
	};
	return roots.begin()->second->tree->sumInternal(func);
}

void InjRandTree::fold(const function<void (const Injection*)> func) const{
	roots.begin()->second->tree->fold(func);
}


}

