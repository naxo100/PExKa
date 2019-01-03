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
class DistributionTree {
protected:
	DistributionTree<T> *parent;
	unsigned level;
	FL_TYPE value;
	FL_TYPE sum;

public:
	//DistributionTree();
	DistributionTree(DistributionTree<T>* leaf = nullptr,FL_TYPE val = 0.0);
	virtual ~DistributionTree();
	//return the sum of activity in node and children
	virtual FL_TYPE total() const;
	virtual void decrease(FL_TYPE val);

	virtual const T& choose(FL_TYPE r) const = 0;
	//return number of element in the node
	virtual unsigned count() const = 0;
	virtual void push(T* inj,FL_TYPE val) = 0;
	virtual void erase(T* inj) = 0;
};

template <typename T>
class Leaf;

template <typename T>
class Node : public DistributionTree<T> {
	friend class DistributionTree<T>;
	vector<T*> injs; //equals
	vector<T*> multi_injs; //equals
	DistributionTree<T> *smaller,*greater;
	unsigned counter;
public:
	Node(FL_TYPE val = 0.0);
	Node(Leaf<T>* leaf);
	virtual ~Node();
	virtual const T& choose(FL_TYPE r) const override;
	virtual unsigned count() const override;

	virtual void push(T* inj,FL_TYPE val) override;
	virtual void erase(T* inj) override;
};

template <typename T>
class Leaf : public DistributionTree<T> {
protected:
	static const int MAX_PER_LVL = 20;
	friend class DistributionTree<T>;
	friend class Node<T>;
	static const unsigned MAX_COUNT;
	vector<pair<T*,FL_TYPE>> injs; //equals
	//float sum;
	void share(Leaf *greater,FL_TYPE val);
public:
	Leaf(DistributionTree<T>* _parent);
	virtual ~Leaf();
	virtual const T& choose(FL_TYPE r) const override;
	virtual unsigned count() const override;

	virtual void push(T* elem,FL_TYPE val) override;
	virtual void erase(T* elem) override;
};


}


#endif /* SRC_DATA_STRUCTS_DISTRIBUTIONTREE_H_ */
