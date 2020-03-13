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

/** \brief All the classes needed for the distribution container. */
namespace distribution_tree {
using namespace std;

template <typename T>
class Node;


/** \brief A container tree-structure to store elements sorted by a float value. */
template <typename T>
class DistributionTree {
	friend class Node<T>;
protected:
	Node<T> *parent;	///< The parent node of this tree-element.
	unsigned level;		///< The distance to the root of the tree.
	FL_TYPE value;		///< For a Node, is the value of all the elements it contains.
	FL_TYPE sum;		///< The total sum of value associated with the elements in and behind this node.

	/** \brief Returns true if this tree-element is unbalanced. */
	virtual bool testBalance() const = 0;
public:
	static const int MAX_LVL0 = 2;	///< The max amount of elements a leaf with level 0 can have.
	//DistributionTree();

	/** \brief Initialize a new instance of a tree-element.
	 *
	 * @param leaf The new tree-element will be constructed as a replace for this leaf.
	 * @param val The value for this new tree-element. 				*/
	DistributionTree(Node<T>* leaf = nullptr,FL_TYPE val = 1.0);
	virtual ~DistributionTree();

	/** \brief Remove all the elements in this tree and delete the element if it's not stored anywhere else. */
	virtual void deleteContent() = 0;

	/** \brief Returns the sum of activity in node and children. */
	virtual FL_TYPE total() const;
	/** \brief Decrease element counter 'n' units and sum by n*val. */
	virtual void decrease(FL_TYPE val,unsigned n = 1) = 0;

	/** \brief Returns an element of the tree corresponding with the value of r. */
	virtual const T& choose(FL_TYPE r) const = 0;
	/** \brief Returns the i-th element in the tree. */
	virtual const pair<T*,FL_TYPE>& choose(unsigned i) const = 0;
	/** \brief Returns the number of elements in this subtree. */
	virtual unsigned count() const = 0;
	/** \brief Adds the element inj to the subtree based on val. */
	virtual void push(T* inj,FL_TYPE val) = 0;
	/** \brief Finds and erases the element inj from the subtree. */
	virtual void erase(T* inj) = 0;
	/** \brief Erases the address-th element in the subtree. */
	virtual T* erase(int address) = 0;
	/** \brief Empty the subtree. Elements not stored in other
	 * container are pushed to free list.*/
	virtual void clear(list<T*> *free = nullptr) = 0;

	/** \brief Apply func to each element in the tree and sum its results. */
	virtual FL_TYPE sumInternal(const function<FL_TYPE (const T*)> &func) const = 0;
	/** \brief Apply func to each element in the tree. */
	virtual void fold(const function<void (const T*)> func) const = 0;
	/** \brief Returns the sum of the squares of each element. */
	virtual FL_TYPE squares() = 0;

	/** \brief Attempts to balance this tree. */
	virtual void treeBalance(DistributionTree<T>*& parent_pointer);
	/** \brief Return an average of the leafs levels in the tree. */
	virtual float treeHeight() const = 0;
};

template <typename T>
class Leaf;

/** \brief A value-associated node of a DistributionTree. It stores the elements with
 * the same value.
 *
 * Elements with lower or higher values pushed into this node will be pushed down to its children. */
template <typename T>
class Node : public DistributionTree<T> {
	/* sum stores the value of all injs inside and down
	 * smaller stores injs < value. */
	friend class DistributionTree<T>;
	friend class Leaf<T>;
	vector<T*> injs;		///< Stores common elements with the same node value.
	vector<T*> multi_injs;	///< Stores collection elements with the same node value.
	DistributionTree<T> *smaller,*greater;	///< subtrees to store elements with lower/higher values.
	unsigned counter;		///< Counter of the total elements in this tree.

	/** @deprecated or not complete. */
	virtual void nodeBalance(DistributionTree<T>*& parent_pointer);
	virtual bool testBalance() const override;
public:
	Node(FL_TYPE val = 0.0);
	/** \brief Constructs a new Node with value 'val', and use 'leaf' as one of its children.
	 *
	 * The elements of 'leaf' are redistributed. */
	Node(Leaf<T>* leaf,FL_TYPE val = 0.0);
	virtual ~Node();
	virtual void deleteContent() override;
	virtual const T& choose(FL_TYPE r) const override;
	virtual const pair<T*,FL_TYPE>& choose(unsigned i) const override;
	virtual unsigned count() const override;

	virtual void push(T* inj,FL_TYPE val) override;
	virtual void erase(T* inj) override;
	virtual T* erase(int address) override;
	virtual void clear(list<T*> *free = nullptr) override;
	virtual void decrease(FL_TYPE val,unsigned n = 1) override;

	/** \brief Redistributes the elements of a full child leaf.
	 *
	 * This method will decide whether to create a new Node or not.
	 * @param full The full child leaf.
	 * @param n The other child. */
	virtual void leafBalance(Leaf<T>* full,DistributionTree<T>* n);
	virtual void treeBalance(DistributionTree<T>*& parent_pointer) override;
	virtual float treeHeight() const override;

	FL_TYPE sumInternal(const function<FL_TYPE (const T*)> &func) const override;
	virtual void fold(const function<void (const T*)> func) const;
	virtual FL_TYPE squares() override;
};

/** \brief The last node level in the DistributionTree. It stores every valued element
 * within a range defined by its parents. */
template <typename T>
class Leaf : public DistributionTree<T> {
protected:
	/*sum stores the sum of all injs
	 * value is zero */
	friend class DistributionTree<T>;
	friend class Node<T>;
	static const unsigned MAX_COUNT;
	vector<pair<T*,FL_TYPE>> injs;	///< Elements in the leaf. They can be (partially) sorted.

	/** \brief Redistributes inner elements to the sister Leaf or the parent Node depending on val's value.
	 *
	 * @param sister the sister leaf.
	 * @param val A negative value means that sister will store smaller values than this. */
	void share(Leaf *sister,FL_TYPE val);

	/** \brief Sort the elements in this Leaf.
	 *
	 * @param revalidate Will update container information of elements if true. */
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
