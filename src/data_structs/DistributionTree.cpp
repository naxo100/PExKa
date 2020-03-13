/*
 * DistributionTree.cpp
 *
 *  Created on: Jun 19, 2018
 *      Author: naxo100
 */

#include "DistributionTree.h"
#include "../matching/Injection.h"
#include <iostream>
#include <string>
#include <algorithm>


namespace distribution_tree {

template <typename T>
DistributionTree<T>::DistributionTree(Node<T>* _parent,FL_TYPE val) : parent(_parent),
		level(_parent ?_parent->level+1 : 0),value(val),sum(0.0){}

template <typename T>
FL_TYPE DistributionTree<T>::total() const {
	return sum;
}

template <typename T>
DistributionTree<T>::~DistributionTree(){
	//if(parent)
	//	delete parent;
}

template <typename T>
void DistributionTree<T>::treeBalance(DistributionTree<T>*& parent_pointer) {
	return;
}

/*********** class InjRandTree::Node **********/

template <typename T>
Node<T>::Node(FL_TYPE val) : DistributionTree<T>(nullptr,val),counter(0) {
	smaller = /*val != 0.0 ? */new Leaf<T>(this); //: nullptr;
	greater = new Leaf<T>(this);
}

//this leaf is always smaller
template <typename T>
Node<T>::Node(Leaf<T>* leaf,FL_TYPE val) : DistributionTree<T>(leaf->parent),counter(leaf->count()){
	this->value = val;
	this->sum = leaf->total();
	leaf->parent = this;
	leaf->level = this->level+1;
	smaller = leaf;
	auto _greater = new Leaf<T>(this);
	greater = _greater;
	leaf->share(_greater,this->value);
}

template <typename T>
Node<T>::~Node(){
	delete greater;
	if(smaller)
		delete smaller;
}

template <typename T>
void Node<T>::deleteContent(){
	greater->deleteContent();
	if(smaller)
		smaller->deleteContent();
	for(auto inj : injs)
		if(inj->removeContainer(*this))
			delete inj;
	for(auto inj : multi_injs)
		if(inj->removeContainer(*this))
			delete inj;
}


template <typename T>
void Node<T>::push(T* inj,FL_TYPE val){
	if(val > this->value){
		try{
			greater->push(inj,val);
		}
		catch(bad_alloc &e){
			leafBalance(static_cast<Leaf<T>*>(greater),smaller);
			this->push(inj, val);
			return;
		}
	}
	else if(val < this->value){
		try{
			smaller->push(inj,val);
		}
		catch(bad_alloc &e){
			leafBalance(static_cast<Leaf<T>*>(smaller),greater);
			this->push(inj, val);
			return;
		}
	}
	else{
		if(inj->count() > 1){
			//inj->alloc(multi_injs.size());//not useful
			inj->addContainer(*this,-multi_injs.size()-1);//negative address for multi injs
			multi_injs.emplace_back(inj);
		}
		else{
			//inj->alloc(injs.size());//not useful
			inj->addContainer(*this,injs.size());
			injs.emplace_back(inj);
		}
	}
	counter += inj->count();
	this->sum += val*inj->count();
}

template <typename T>
void Node<T>::erase(T* elem){
	throw invalid_argument("dont call this method: Node<T>::erase(T* elem).");
	if(elem->count() != 1){
		multi_injs.back()->addContainer(*this,elem->getAddress());
		multi_injs[elem->getAddress()] = multi_injs.back();
		multi_injs.pop_back();
	}
	else{
		injs.back()->addContainer(*this,elem->getAddress());
		injs[elem->getAddress()] = injs.back();
		injs.pop_back();
	}
	elem->alloc(size_t(-1));
	this->decrease(this->value*elem->count(),elem->count());
}

template <typename T>
T* Node<T>::erase(int address){
	T* elem;
	if(address < 0){
		address = -address + 1;
		elem = multi_injs[address];
		multi_injs.back()->addContainer(*this,address);
		multi_injs[address] = multi_injs.back();
		multi_injs.pop_back();
	}
	else{
		elem = injs[address];
		injs.back()->addContainer(*this,address);
		injs[address] = injs.back();
		injs.pop_back();
	}
	//elem->alloc(size_t(-1));
	this->decrease(this->value*elem->count(),elem->count());
	return elem;
}

template <typename T>
void Node<T>::clear(list<T*>* free){
	smaller->clear(free);
	greater->clear(free);
	for(auto inj : multi_injs)
		if(inj->removeContainer(*this) && free)
			free->push_back(inj);
	multi_injs.clear();
	for(auto inj : injs)
		if(inj->removeContainer(*this) && free)
			free->push_back(inj);
	injs.clear();
	counter = 0;
	this->sum = 0;
}

template <typename T>
void Node<T>::decrease(FL_TYPE val,unsigned n){
	this->sum -= val;
	counter -= n;
	if(this->parent)
		this->parent->decrease(val,n);
}

template <typename T>
const T& Node<T>::choose(FL_TYPE sel) const {
	if(smaller){
		if(sel < smaller->total())
			return smaller->choose(sel);
		else
			sel -= smaller->total();
	}
	//auto s = value*count();
	auto r_node = (counter - smaller->count()-greater->count())*this->value;
	if(sel < r_node){
		for(auto inj : multi_injs){
			auto r_multi = inj->count()*this->value;
			if(sel < r_multi)
				return *inj;
			else
				sel -= r_multi;
		}
		return *injs[int(injs.size() * sel / (injs.size()*this->value))];
	}

	return greater->choose(sel - r_node);
}

template <typename T>
const pair<T*,FL_TYPE>& Node<T>::choose(unsigned sel) const {
	throw std::invalid_argument("Node<T>::choose() not available");
}

template <typename T>
unsigned Node<T>::count() const{
	return counter;
}



template <typename T>
void Node<T>::leafBalance(Leaf<T>* full,DistributionTree<T>* n) {
	if(injs.size() || multi_injs.size()){//TODO size > 5
		full->sort();
		auto new_node = new Node<T>(full,full->choose(full->count()/2).second);
		if(smaller == full)
			smaller = new_node;
		else
			greater = new_node;
		return;
	}

	auto leaf = static_cast<Leaf<T>*>(n);
	if( leaf && leaf->count() < (Leaf<T>::MAX_LVL0 << this->level)){//second node is leaf and not half-full
		leaf->sort(true);
		full->sort();//this let invalid containers of CcValueInj
		auto median = (leaf->count() + full->count())/2;
		if(full == smaller){
			this->value = smaller->choose(median).second;
			full->share(leaf,this->value);//this should validate containers
		}
		else{
			this->value = greater->choose(median-leaf->count()).second;
			full->share(leaf,- this->value);
		}

	}
	else{
		full->sort();
		auto new_node = new Node<T>(full,full->choose(full->count()/2).second);
		if(smaller == full)
			smaller = new_node;
		else
			greater = new_node;
		return;
	}
}

template <typename T>
FL_TYPE Node<T>::sumInternal(const function<FL_TYPE (const T*)> &func) const {
	FL_TYPE sum = 0;
	for(auto inj : injs){
		sum += func(inj);
	}
	for(auto inj : multi_injs){
		sum += func(inj)*inj->count();
	}
	return sum + smaller->sumInternal(func)+greater->sumInternal(func);
}

template <typename T>
void Node<T>:: fold(const function<void (const T*)> func) const {
	smaller->fold(func);
	for(auto& inj : injs)
		func(inj);
	for(auto& inj : multi_injs)
		for(unsigned i = 0; i < inj->count(); i++)
			func(inj);
	greater->fold(func);
}


template <typename T>
FL_TYPE Node<T>::squares() {
	return this->value*this->value*counter + smaller->squares() + greater->squares();
}



template <typename T>
float Node<T>::treeHeight() const {
	return (smaller->treeHeight() + greater->treeHeight() )/2.0;
}

template <typename T>
bool Node<T>::testBalance() const {
	return smaller->treeHeight() >= 3*greater->treeHeight() ? true :
			smaller->treeHeight()*3 <= greater->treeHeight()? true : false;
}


template <typename T>
void Node<T>::treeBalance(DistributionTree<T>*& parent_pointer) {
	smaller->treeBalance(smaller);
	greater->treeBalance(greater);

	if(testBalance())
		nodeBalance(parent_pointer);
}

template <typename T>
void Node<T>::nodeBalance(DistributionTree<T>*& parent_pointer) {
	throw invalid_argument("this method Node<T>::nodeBalance() is not ready.");
	if(smaller->treeHeight() > greater->treeHeight()){
		auto count_buff = this->count() - smaller->count();
		auto sum_buff = this->total() - smaller->total();

		smaller->parent = this->parent;
		smaller->level--;
		auto smaller_node = dynamic_cast<Node<T>*>(smaller);
		smaller_node->greater->parent = this;
		smaller_node->counter += count_buff;
		smaller_node->sum += sum_buff;

		this->parent = smaller_node;
		this->smaller = smaller_node->smaller;
		this->level++;
		this->counter -= count_buff;
		this->sum -= sum_buff;
		parent_pointer = smaller_node;
	}
	else{
		auto count_buff = this->count() - greater->count();
		auto sum_buff = this->total() - greater->total();

		greater->parent = this->parent;
		greater->level--;
		auto greater_node = dynamic_cast<Node<T>*>(greater);
		greater_node->smaller->parent = this;
		greater_node->counter += count_buff;
		greater_node->sum += sum_buff;

		this->parent = greater_node;
		this->greater = greater_node->greater;
		this->level++;
		this->counter -= count_buff;
		this->sum -= sum_buff;
		parent_pointer = greater_node;
	}

}


/*********** class InjRandTree::Leaf **********/

template <typename T>
Leaf<T>::Leaf(Node<T>* _parent) : DistributionTree<T>(_parent) {}

template <typename T>
Leaf<T>::~Leaf(){}

template <typename T>
void Leaf<T>::deleteContent(){
	for(auto inj : injs)
		if(inj.first->removeContainer(*this))
			delete inj.first;
}

template <typename T>
void Leaf<T>::share(Leaf* sister,FL_TYPE val){//negative val for smaller sister
	auto abs_val = fabs(val);
	int i = 0;
	for(auto it = injs.begin(); it != injs.end();){
		if(val < 0 ? it->second < abs_val : it->second > abs_val){
			it->first->removeContainer(*this);
			sister->push(it->first, it->second);
			this->sum -= it->second;
			it = injs.erase(it);//TODO problems with erase in for?
		}
		else if(it->second == abs_val){
			it->first->removeContainer(*this);
			this->parent->push(it->first, it->second);
			this->sum -= it->second;
			this->parent->sum -= it->second;
			this->parent->counter --;
			it = injs.erase(it);//TODO problems with erase in for?
		}
		else{
			it->first->addContainer(*this,i);
			it++;i++;
		}
	}
}

template <typename T>
void Leaf<T>::sort(bool revalidate){
	static auto is_less = [](const pair<T*,FL_TYPE> &a,const pair<T*,FL_TYPE> &b) {return a.second < b.second;};
	std::sort(injs.begin(),injs.end(),is_less);
	if(revalidate)
		for(unsigned i = 0; i < injs.size(); i++)
			injs[i].first->addContainer(*this,i);
}


template <typename T>
void Leaf<T>::push(T* inj,FL_TYPE val){
	if(inj->count() > 1){// TODO BUUUUGGG HERE (cannot set node child)
		auto new_node = new Node<T>(this,val);
		if(this == this->parent->smaller)
			this->parent->smaller = new_node;
		else
			this->parent->greater = new_node;
		new_node->push(inj,val);
		return;
	}
	if(injs.size() == (this->MAX_LVL0 << this->level) )
		throw bad_alloc();
	else{
		//inj->alloc(injs.size());//not useful
		inj->addContainer(*this,injs.size());
		injs.emplace_back(inj,val);//TODO maybe ordered??
		this->sum += val;
	}
}

//dont need to ask if parent is null
template <typename T>
void Leaf<T>::decrease(FL_TYPE val,unsigned n){
	this->sum -= val;
	this->parent->decrease(val);
}


template <typename T>
void Leaf<T>::erase(T* elem){
	throw invalid_argument("dont call this method: Leaf<T>::erase(T*).");
	this->decrease(injs[elem->getAddress()].second);
	injs.back().first->addContainer(*this,elem->getAddress());
	injs[elem->getAddress()] = injs.back();
	injs.pop_back();
}
template <typename T>
T* Leaf<T>::erase(int address){
	if(address > injs.size())
		throw std::out_of_range("This injection is not here! (Leaf<T>::erase(int))");
	pair<T*,FL_TYPE> elem_val = injs[address];
	this->decrease(elem_val.second);
	injs.back().first->addContainer(*this,address);
	injs[address] = injs.back();
	injs.pop_back();

	return elem_val.first;
}

template <typename T>
void Leaf<T>::clear(list<T*>* free){
	for(auto inj : injs)
		if(inj.first->removeContainer(*this) && free)
			free->push_back(inj.first);
	injs.clear();
	this->sum = 0;
}

template <typename T>
const T& Leaf<T>::choose(FL_TYPE sel) const {
	/*auto p = injs[int(count() * sel / this->sum)].first;
	if(p == nullptr){
		std::cout << "BAD!!!" << std::endl;
	}*/
	return *injs[int(count() * sel / this->sum)].first;
}



template <typename T>
const pair<T*,FL_TYPE>& Leaf<T>::choose(unsigned i) const {
	return injs[i];
}

template <typename T>
unsigned Leaf<T>::count() const{
	return injs.size();
}

template <typename T>
FL_TYPE Leaf<T>::sumInternal(const function<FL_TYPE (const T*)> &func) const {
	FL_TYPE sum = 0;
	for(auto& inj : injs){
		sum += func(inj.first);
	}
	return sum;
}

template <typename T>
void Leaf<T>:: fold(const function<void (const T*)> func) const {
	for(auto& inj : injs)
		func(inj.first);
}


template <typename T>
FL_TYPE Leaf<T>::squares() {
	if(injs.size() == 0)
		return 0;
	this->sort(true);
	auto a = injs.front().second,b = injs.back().second;
	return (a*a + a*b + b*b) / 3.0;//second-moment of uniform distr
}


template <typename T>
float Leaf<T>::treeHeight() const{
	return this->level+1;
}

template <typename T>
bool Leaf<T>::testBalance() const {
	return false;
}




template class DistributionTree<matching::CcValueInj>;
template class Node<matching::CcValueInj>;
template class Leaf<matching::CcValueInj>;

}

