/*
 * DistributionTree.h
 *
 *  Created on: Jun 19, 2018
 *      Author: naxo100
 */

#ifndef SRC_DATA_STRUCTS_DISTRIBUTIONTREE_H_
#define SRC_DATA_STRUCTS_DISTRIBUTIONTREE_H_

#include <vector>
#include <functional>

#include "../util/params.h"
#include <list>


namespace distribution_tree {
using namespace std;

template <typename T>
class Node;

template <typename T>
class DistributionTree {
	friend class Node<T>;
protected:
	Node<T> *parent;
	unsigned level;
	FL_TYPE value;
	FL_TYPE sum;

	//virtual void nodeBalance(DistributionTree<T>*& parent_pointer) = 0;
	virtual bool testBalance() const = 0;
public:
	static const int MAX_LVL0 = 2;
	//DistributionTree();
	DistributionTree(Node<T>* leaf = nullptr,FL_TYPE val = 1.0);
	virtual ~DistributionTree();
	virtual void deleteContent() = 0;
	//return the sum of activity in node and children
	virtual FL_TYPE total() const;
	virtual void decrease(FL_TYPE val,unsigned n = 1) = 0;

	virtual const T& choose(FL_TYPE r) const = 0;
	virtual const pair<T*,FL_TYPE>& choose(unsigned i) const = 0;
	//return number of element in the node
	virtual unsigned count() const = 0;
	virtual void push(T* inj,FL_TYPE val) = 0;
	virtual void erase(T* inj) = 0;
	virtual T* erase(int address) = 0;
	virtual void clear(list<T*> *free = nullptr) = 0;

	virtual FL_TYPE sumInternal(const function<FL_TYPE (const T*)> &func) const = 0;
	virtual void fold(const function<void (const T*)> func) const = 0;
	virtual FL_TYPE squares() = 0;

	virtual void treeBalance(DistributionTree<T>*& parent_pointer);
	virtual float treeHeight() const = 0;
};

template <typename T>
class Leaf;

template <typename T>
class Node : public DistributionTree<T> {
	/* sum stores the value of all injs inside and down
	 * smaller stores injs < value
	 */
	friend class DistributionTree<T>;
	friend class Leaf<T>;
	vector<T*> injs; //equals
	vector<T*> multi_injs; //equals
	DistributionTree<T> *smaller,*greater;
	unsigned counter;

	virtual void nodeBalance(DistributionTree<T>*& parent_pointer);
	virtual bool testBalance() const override;
public:
	Node(FL_TYPE val = 0.0);
	Node(Leaf<T>* leaf,FL_TYPE val = 0.0);
	virtual ~Node();
	virtual void deleteContent();
	virtual const T& choose(FL_TYPE r) const override;
	virtual const pair<T*,FL_TYPE>& choose(unsigned i) const override;
	virtual unsigned count() const override;

	virtual void push(T* inj,FL_TYPE val) override;
	virtual void erase(T* inj) override;
	virtual T* erase(int address) override;
	virtual void clear(list<T*> *free = nullptr) override;
	virtual void decrease(FL_TYPE val,unsigned n = 1) override;

	virtual void leafBalance(Leaf<T>* full,DistributionTree<T>* n);
	virtual void treeBalance(DistributionTree<T>*& parent_pointer) override;
	virtual float treeHeight() const override;

	FL_TYPE sumInternal(const function<FL_TYPE (const T*)> &func) const override;
	virtual void fold(const function<void (const T*)> func) const;
	virtual FL_TYPE squares();
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
	void sort(bool revalidate = false);

	virtual bool testBalance() const override;
public:
	Leaf(Node<T>* _parent);
	virtual ~Leaf();
	virtual void deleteContent();
	const T& choose(FL_TYPE r) const override;
	virtual const pair<T*,FL_TYPE>& choose(unsigned r) const override;
	virtual unsigned count() const override;

	virtual void push(T* elem,FL_TYPE val) override;
	virtual void erase(T* elem) override;
	virtual T* erase(int address) override;
	virtual void clear(list<T*> *free) override;


	virtual void decrease(FL_TYPE val, unsigned n = 1) override;

	FL_TYPE sumInternal(const function<FL_TYPE (const T*)> &func) const override;
	virtual void fold(const function<void (const T*)> func) const;
	virtual FL_TYPE squares();


	virtual float treeHeight() const override;
};


}


#endif /* SRC_DATA_STRUCTS_DISTRIBUTIONTREE_H_ */
