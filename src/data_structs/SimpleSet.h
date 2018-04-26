/*
 * SimpleSet.h
 *
 *  Created on: Jan 30, 2018
 *      Author: naxo
 */

#ifndef SRC_DATA_STRUCTS_SIMPLESET_H_
#define SRC_DATA_STRUCTS_SIMPLESET_H_

#include <vector>

namespace data_structs {
using namespace std;

template <typename T>
class SimpleSet {
	T* container;
	unsigned char valid_size;
	unsigned char real_size;
public:
	SimpleSet(size_t init = 25);
	~SimpleSet();

	size_t erase(T elem);
	void erase(T* it);
	void emplace(T elem);

	//typename vector<T>::iterator begin();
	//typename vector<T>::iterator end();

	T* begin();
	T* end();

};

} /* namespace data_structs */

#endif /* SRC_DATA_STRUCTS_SIMPLESET_H_ */
