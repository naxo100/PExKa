/*
 * DistributionTree.cpp
 *
 *  Created on: Jun 19, 2018
 *      Author: naxo100
 */

#include "DistributionTree.h"
#include "../matching/Injection.h"

namespace distribution_tree {

template <typename T>
DistributionTree<T>::DistributionTree(DistributionTree<T>* _parent,FL_TYPE val) : parent(_parent),
		level(_parent ?_parent->level+1 : 0),value(val),sum(0.0){}

template <typename T>
FL_TYPE DistributionTree<T>::total() const {
	return sum;
}

template <typename T>
void DistributionTree<T>::decrease(FL_TYPE val){
	sum -= val;
	if(parent)
		parent->decrease(val);
}

template <typename T>
DistributionTree<T>::~DistributionTree(){
	if(parent)
		delete parent;
}

/*********** class InjRandTree::Node **********/

template <typename T>
Node<T>::Node(FL_TYPE val) : DistributionTree<T>(nullptr,val),counter(0) {
	smaller = val != 0.0 ? new Leaf<T>(this) : nullptr;
	greater = new Leaf<T>(this);
}

template <typename T>
Node<T>::Node(Leaf<T>* leaf) : DistributionTree<T>(leaf->parent),counter(0){
	auto node_parent = static_cast<Node<T>*>(this->parent);
	if(node_parent->value > this->value)
		node_parent->smaller = this;
	else
		node_parent->greater = this;
	this->sum = leaf->total();
	leaf->parent = this;
	this->value = leaf->sum / leaf->injs.size();
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
	for(auto inj : injs)
		delete inj;
	for(auto inj : multi_injs)
		delete inj;
}

template <typename T>
void Node<T>::push(T* inj,FL_TYPE val){
	if(val > this->value)
		greater->push(inj,val);
	else if(val < this->value)
		smaller->push(inj, val);
	else{
		if(inj->count() > 1){
			inj->alloc(multi_injs.size());
			multi_injs.emplace_back(inj);
			counter += inj->count();
		}
		else{
			inj->alloc(injs.size());
			injs.emplace_back(inj);
			counter++;
		}
		inj->setContainer(this);
	}
	this->sum += val*inj->count();
}

template <typename T>
void Node<T>::erase(T* elem){
	if(elem->count() != 1){
		multi_injs.back()->alloc(elem->getAddress());
		multi_injs[elem->getAddress()] = multi_injs.back();
		multi_injs.pop_back();
	}
	else{
		injs.back()->alloc(elem->getAddress());
		injs[elem->getAddress()] = injs.back();
		injs.pop_back();
	}
	elem->alloc(size_t(-1));
	this->decrease(this->value);
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
	auto r_node = this->count()*this->value;
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
unsigned Node<T>::count() const{
	return counter;
}


/*********** class InjRandTree::Leaf **********/

template <typename T>
Leaf<T>::Leaf(DistributionTree<T>* _parent) : DistributionTree<T>(_parent) {}

template <typename T>
Leaf<T>::~Leaf(){
	for(auto inj : injs)
		delete inj.first;
}

template <typename T>
void Leaf<T>::share(Leaf* great,FL_TYPE val){
	for(auto it = injs.begin(); it != injs.end();){
		if(it->second > val){
			great->push(it->first, it->second);
			this->sum -= it->second;
			it = injs.erase(it);//TODO problems with erase in for?
		}
		else if(it->second == val){
			this->parent->push(it->first, it->second);
			this->sum -= it->second;
			it = injs.erase(it);//TODO problems with erase in for?
		}
		else
			it++;
	}
}



template <typename T>
void Leaf<T>::push(T* inj,FL_TYPE val){
	if(injs.size() == MAX_PER_LVL*this->level || inj->count() > 1){
		auto new_node = new Node<T>(this);
		new_node->push(inj,val);
	}
	else{
		inj->alloc(injs.size());
		injs.emplace_back(inj,val);//TODO maybe ordered??
		inj->setContainer(this);
		this->sum += val;
	}
}

template <typename T>
void Leaf<T>::erase(T* elem){
	this->decrease(injs[elem->getAddress()].second);
	injs.back().first->alloc(elem->getAddress());
	injs[elem->getAddress()] = injs.back();
	injs.pop_back();
}

template <typename T>
const T& Leaf<T>::choose(FL_TYPE sel) const {
	return *injs[int(count() * sel / this->sum)].first;
}

template <typename T>
unsigned Leaf<T>::count() const{
	return injs.size();
}


template class DistributionTree<matching::CcValueInj>;
template class Node<matching::CcValueInj>;
template class Leaf<matching::CcValueInj>;

}

