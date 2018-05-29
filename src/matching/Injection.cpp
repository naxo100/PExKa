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

/*bool Injection::operator<(const Injection& inj) const {
	return this->getEmbedding() < inj.getEmbedding();
}*/

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

CcInjection::CcInjection(const pattern::Mixture::Component& _cc)
		: Injection(_cc),ccAgToNode(_cc.size(),nullptr) {
	//reuse(_cc,node,port_list,root);
}

bool CcInjection::reuse(const pattern::Mixture::Component& _cc,Node& node,
		two<std::list<state::Node::Internal*> >& port_list,small_id root) {
	//ccAgToNode.clear();
	for(auto& n : ccAgToNode)
		n = nullptr;
	//if(_cc.getAgent(root).getId() == node.getId()){
	ccAgToNode.resize(_cc.size(),nullptr);
	std::list<pair<small_id,Node*> > q;
	q.emplace_back(root,&node);
	const pattern::Mixture::Agent* ag;
	const Node* curr_node;
	pair<small_id,Node*> next_node(0,nullptr);
	//for(auto elem : _cc.getGraph())
	//	printf("(%d,%d)->(%d,%d); ",elem.first.first,elem.first.second,elem.second.first,elem.second.second);
	//cout << endl;
	while(!q.empty()){
		ag = &_cc.getAgent(q.front().first);
		curr_node = q.front().second;
		if(ag->getId() != curr_node->getId())
			return false;
		for(auto& id_site : *ag){
			if(curr_node->test(next_node.second,id_site,port_list)){
				if(next_node.second){
					next_node.first = _cc.follow(q.front().first,id_site.first).first;
					//need to check site of links?? TODO No
					if(ccAgToNode[next_node.first]){
						if(ccAgToNode[next_node.first] != next_node.second)
							return false;
					}
					else {
						q.emplace_back(next_node);
						ccAgToNode[next_node.first] = next_node.second;//todo review
					}
					next_node.second = nullptr;
				}
			}
			else
				return false;
		}
		ccAgToNode[q.front().first] = q.front().second;//case when agent mixture with no sites
		q.pop_front();
	}
	return true;
	//} else throw False();//TODO check_aditional_edges???
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

bool CcInjection::operator==(const Injection& inj) const{
	auto& cc_inj = static_cast<const CcInjection&>(inj);
	for(size_t i = 0;i < ccAgToNode.size(); i++)
		if(ccAgToNode[i] != cc_inj.ccAgToNode[i])
			return false;
	return true;
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
		inj = new CcInjection(cc);
		freeInjs.push_back(inj);
	}
	else {
		inj = freeInjs.front();
	}
	if(inj->reuse(cc,node,port_lists,root) == false)
		return nullptr;
	insert(inj);
	freeInjs.pop_front();
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
	else
		inj = freeInjs.front();

	inj->copy(container[base_inj->address],mask);
	container[inj->address];
	freeInjs.pop_front();

	if(inj->count() > 1){
		counter += inj->count();
		multiCount++;
	}
	else
		counter++;

/*#if DEBUG
	for(auto inj2 : container)
		if(inj != inj2 && *inj == *inj2)
			throw invalid_argument("InjSet cannot contain the same injection twice.");
#endif*/
	return inj;
}

void InjRandSet::erase(Injection* inj){
	//if(container.empty())
	//	throw invalid_argument("InjRandSet is empty, what injection are you trying to delete?");
	if(inj->address < multiCount)
		multiCount--;
	freeInjs.push_back(static_cast<CcInjection*>(inj));
	if(container.size() > 1){
		container.back()->alloc(inj->address);
		container[inj->address] = container.back();
	}
	inj->alloc(size_t(-1));//dealloc
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
