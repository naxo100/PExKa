/*
 * Injection.cpp
 *
 *  Created on: Apr 11, 2017
 *      Author: naxo
 */

#include "Injection.h"
#include <queue>
#include <iostream>

namespace matching {

Injection::Injection(const pattern::Pattern& _ptrn) : ptrn(_ptrn),address(size_t(-1)) {}

Injection::~Injection() {}

bool Injection::operator<(const Injection& inj) const {
	return this->getEmbedding() < inj.getEmbedding();
}

void Injection::alloc(size_t addr){
	address = addr;
}


const pattern::Pattern& Injection::pattern() const{
	return ptrn;
}

bool Injection::isTrashed() const {
	static auto trshd = size_t(-1);
	return address == trshd;
}


/********** CcInjection **********/

CcInjection::CcInjection(const pattern::Pattern& _cc) : Injection(_cc) {};

CcInjection::CcInjection(const pattern::Mixture::Component& _cc,Node& node,
		two<std::list<state::Node::Internal*> >& port_list,small_id root)
		: Injection(_cc) {
	reuse(_cc,node,port_list,root);
}

void CcInjection::reuse(const pattern::Mixture::Component& _cc,Node& node,
		two<std::list<state::Node::Internal*> >& port_list,small_id root) {
	ccAgToNode.resize(_cc.size(),nullptr);
	std::queue<pair<small_id,Node&> > q;
	q.emplace(root,node);
	while(q.size()){
		auto& ag = _cc.getAgent(q.front().first);
		if(ag.getId() != node.getId())
			throw False();
		for(auto& id_site : ag){
			if(node.test(id_site,q,port_list)){
				auto lnk = _cc.follow(q.front().first,q.back().first);
				//need to check site of links?? TODO
				if(ccAgToNode[lnk.first]){
					if(ccAgToNode[lnk.first] == &(q.back().second))
						continue;
					else
						throw False();
				}
				else {
					q.back().first = lnk.first;
					ccAgToNode[lnk.first] = &(q.back().second);
				}
			}
		}
		ccAgToNode[q.front().first] = &(q.front().second);//case when agent mixture with no sites
		q.pop();
	}
	//TODO check_aditional_edges???
}

CcInjection::~CcInjection(){};

const vector<Node*>& CcInjection::getEmbedding() const {
	return ccAgToNode;
}

void CcInjection::codomain(Node* injs[],set<Node*> cod) const {
	int i = 0;
	for(auto node_p : ccAgToNode){
		if(cod.find(node_p) != cod.end())
			throw False();
		cod.emplace(node_p);
		injs[i] = node_p;
		i++;
	}
}

size_t CcInjection::count() const {
	return ccAgToNode[0]->getCount();//TODO empty cc ??
}


Injection* CcInjection::clone(const map<Node*,Node*>& mask) const {
	auto new_inj = new CcInjection(pattern());
	new_inj->copy(this,mask);
	return new_inj;
}

void CcInjection::copy(const CcInjection* inj,const map<Node*,Node*>& mask){
	ccAgToNode.resize(inj->ccAgToNode.size());
	for(size_t i = 0; i < ccAgToNode.size(); i++){
		ccAgToNode[i] = (mask.at(inj->ccAgToNode[i]));//safe
	}
}




/************ InjSet *****************/

//----------------------------------

/********* InjRandSet ********************/

InjRandSet::InjRandSet() : counter(0),multiCount(0) {
	container.reserve(100);
}

InjRandSet::~InjRandSet(){
	for(auto inj : freeInjs)
		delete inj;
	for(auto inj : container)
		delete inj;
}

//TODO better way to count?
size_t InjRandSet::count() const {
	unsigned c = 0;
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

void InjRandSet::insert(CcInjection* inj){
	inj->alloc(container.size());
	container.push_back(inj);
}

Injection* InjRandSet::emplace(const pattern::Mixture::Component& cc,Node& node,
		two<std::list<Node::Internal*> > &port_lists,small_id root) {
	CcInjection* inj;
	if(freeInjs.empty()){
		inj = new CcInjection(cc,node,port_lists,root);
		insert(inj);
	}
	else {
		inj = freeInjs.front();
		inj->reuse(cc,node,port_lists,root);
		insert(inj);
		freeInjs.pop_front();
	}
	if(inj->count() > 1){
		counter += inj->count();
		multiCount++;
	}
	else
		counter++;

	return inj;
}

Injection* InjRandSet::emplace(Injection* base_inj,map<Node*,Node*>& mask){
	CcInjection* inj;
	if(freeInjs.empty()){
		inj = static_cast<CcInjection*>(base_inj->clone(mask));
		insert(inj);
	}
	else {
		inj = freeInjs.front();
		inj->copy(container[base_inj->address],mask);
		container[inj->address];
		freeInjs.pop_front();
	}
	if(inj->count() > 1){
		counter += inj->count();
		multiCount++;
	}
	else
		counter++;

	return inj;
}

void InjRandSet::erase(const Injection* inj){
	if(!container.size())
		throw invalid_argument("InjRandSet is empty, what injection are you trying to delete?");
	if(inj->address < multiCount)
		multiCount--;
	freeInjs.push_back(container[inj->address]);
	if(container.size() > 1){
		container.back()->alloc(inj->address);
		container[inj->address] = container.back();
	}
	freeInjs.back()->alloc(size_t(-1));//dealloc
	container.pop_back();
	counter -= inj->count();
}


vector<CcInjection*>::iterator InjRandSet::begin(){
	return container.begin();
}

vector<CcInjection*>::iterator InjRandSet::end(){
	return container.end();
}


} /* namespace simulation */
