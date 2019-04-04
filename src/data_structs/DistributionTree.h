/*
 * DistributionTree.h
 *
 *  Created on: Jun 19, 2018
 *      Author: naxo100
 */

#ifndef SRC_DATA_STRUCTS_DISTRIBUTIONTREE_H_
#define SRC_DATA_STRUCTS_DISTRIBUTIONTREE_H_

#include <vector>

#include "../util/params.h"


namespace distribution_tree {
using namespace std;

template <typename T>
class Node;

template <typename T>
class DistributionTree {
protected:
	Node<T> *parent;
	unsigned level;
	FL_TYPE value;
	FL_TYPE sum;

public:
	static const int MAX_LVL0 = 2;
	//DistributionTree();
	DistributionTree(Node<T>* leaf = nullptr,FL_TYPE val = 0.0);
	virtual ~DistributionTree();
	//return the sum of activity in node and children
	virtual FL_TYPE total() const;
	virtual void decrease(FL_TYPE val,unsigned n = 1) = 0;

	virtual const T& choose(FL_TYPE r) const = 0;
	virtual const pair<T*,FL_TYPE>& choose(unsigned i) const = 0;
	//return number of element in the node
	virtual unsigned count() const = 0;
	virtual void push(T* inj,FL_TYPE val) = 0;
	virtual void erase(T* inj) = 0;
	virtual void erase(int address) = 0;
};

template <typename T>
class Leaf;

template <typename T>
class Node : public DistributionTree<T> {
	friend class DistributionTree<T>;
	friend class Leaf<T>;
	vector<T*> injs; //equals
	vector<T*> multi_injs; //equals
	DistributionTree<T> *smaller,*greater;
	unsigned counter;
public:
	Node(FL_TYPE val = 0.0);
	Node(Leaf<T>* leaf,FL_TYPE val = 0.0);
	virtual ~Node();
	virtual const T& choose(FL_TYPE r) const override;
	virtual const pair<T*,FL_TYPE>& choose(unsigned i) const override;
	virtual unsigned count() const override;

	virtual void push(T* inj,FL_TYPE val) override;
	virtual void erase(T* inj) override;
	virtual void erase(int address) override;
	virtual void decrease(FL_TYPE val,unsigned n = 1) override;

	virtual void balance(Leaf<T>* full,DistributionTree<T>* n);
};

template <typename T>
class Leaf : public DistributionTree<T> {
protected:
	/*sum stores the sum of all injs
	 * value is zero
	 */
	friend class DistributionTree<T>;
	friend class Node<T>;
	static const unsigned MAX_COUNT;
	vector<pair<T*,FL_TYPE>> injs; //equals
	//float sum;
	void share(Leaf *greater,FL_TYPE val);
	void sort();
public:
	Leaf(Node<T>* _parent);
	virtual ~Leaf();
	const T& choose(FL_TYPE r) const override;
	virtual const pair<T*,FL_TYPE>& choose(unsigned r) const override;
	virtual unsigned count() const override;

	virtual void push(T* elem,FL_TYPE val) override;
	virtual void erase(T* elem) override;
	virtual void erase(int address) override;


	virtual void decrease(FL_TYPE val, unsigned n = 1) override;
};


}


#endif /* SRC_DATA_STRUCTS_DISTRIBUTIONTREE_H_ */
