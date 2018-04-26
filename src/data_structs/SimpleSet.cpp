/*
 * SimpleSet.cpp
 *
 *  Created on: Jan 30, 2018
 *      Author: naxo
 */

#include "SimpleSet.h"
#include "../matching/Injection.h"

namespace data_structs {

template<typename T>
SimpleSet<T>::SimpleSet(size_t init) : valid_size(0),real_size(init){
	container = new T[init];
}

template<typename T>
SimpleSet<T>::~SimpleSet(){}

template<typename T>
size_t SimpleSet<T>::erase(T elem){
	for(size_t i = 0; i < valid_size; i++){
		if(elem == container[i]){
			//delete &container[i];
			container[i] = container[valid_size-1];
			valid_size--;
			return 1;
		}
	}
	return 0;
}

template <typename T>
void SimpleSet<T>::erase(T* it){
	//container.erase(it);
	(*it) = container[valid_size-1];
	valid_size--;
}

template<typename T>
void SimpleSet<T>::emplace(T elem){
	container[valid_size] = elem;
	valid_size++;
}

/*template <typename T>
typename vector<T>::iterator SimpleSet<T>::begin(){
	return container.begin();
}
template <typename T>
typename vector<T>::iterator SimpleSet<T>::end(){
	return container.end();
}*/

template <typename T>
T* SimpleSet<T>::begin() {
	return container;
}
template <typename T>
T* SimpleSet<T>::end() {
	return container+valid_size;
}

template class SimpleSet<matching::Injection*>;

} /* namespace simulation */
