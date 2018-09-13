/*
 * InjRandSet.cpp
 *
 *  Created on: Jun 26, 2018
 *      Author: naxo100
 */

#include "InjRandSet.h"
#include "../state/Node.h"

namespace matching {

InjRandSet::~InjRandSet(){};

/********* MultiInjSet ********************/

MultiInjSet::MultiInjSet(const pattern::Mixture::Component& _cc):
		counter(0),multiCount(0),cc(_cc) {
	container.reserve(100);
}

MultiInjSet::~MultiInjSet(){
	for(auto inj : freeInjs)
		delete inj;
	for(auto inj : container)
		delete inj;
}

//TODO better way to count?
FL_TYPE MultiInjSet::count() const {
	unsigned c = 0;
	for(size_t i = 0; i < multiCount; i++)
		c += container[i]->count();
	return c + container.size() - multiCount;
}

//TODO
const Injection& MultiInjSet::chooseRandom(default_random_engine& randGen) const{
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

void MultiInjSet::insert(CcInjection* inj){
	inj->alloc(container.size());
	container.push_back(inj);
}

Injection* MultiInjSet::emplace(Node& node,
		two<std::list<state::Internal*> > &port_lists,small_id root) {
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

Injection* MultiInjSet::emplace(Injection* base_inj,map<Node*,Node*>& mask){
	CcInjection* inj;
	if(freeInjs.empty()){
		inj = static_cast<CcInjection*>(base_inj->clone(mask));
		freeInjs.push_back(inj);
	}
	else
		inj = freeInjs.front();

	inj->copy(container[base_inj->getAddress()],mask);
	//container[inj->address];
	insert(inj);
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

void MultiInjSet::erase(Injection* inj){
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


/*vector<CcInjection*>::iterator InjRandSet::begin(){
	return container.begin();
}

vector<CcInjection*>::iterator InjRandSet::end(){
	return container.end();
}
*/


/************ InjTree *******************/

template <class Cont,typename T>
InjTree<Cont,T>::InjTree(const pattern::Mixture::Component& _cc) :
		cc(_cc){
	for(auto agent : cc){
		for(auto& id_site : *agent){

		}
	}
}























}

