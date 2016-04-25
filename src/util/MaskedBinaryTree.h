/*
 * MaskedBinaryTree.h
 *
 *  Created on: Apr 19, 2016
 *      Author: naxo
 */

#ifndef UTIL_MASKEDBINARYTREE_H_
#define UTIL_MASKEDBINARYTREE_H_

#include <map>
#include <set>
#include <math.h>

using namespace std;

namespace util {


/** \brief Masked implementation of a binary tree of floats*.
 *
 * Binary tree implementation using an array of nodes. Parent
 * of Node i is i/2. Mask and unmask maps are used to correspond
 * external and internal ids of nodes.
 * Computation of total() and chooseRandom are optimized.
 *
 */
class MaskedBinaryTree {
	int size;
	struct Node {
		int id;
		float value;
		float sub_tree;
	};
	const int           fresh_id;
	Node*               nodes;
	map<int,int>        mask;
	map<int,int>        unmask;
	unordered_set<int>  inf_list;///<the list of infinite-rate rule
	bool*               unbalanced_events;
	list<int>*          unbalanced_events_by_layer;

	int* layer;
	bool isConsistent;

	void update();
	void leftSon(int i);
	void rightSon(int i);

	void declare_unbalanced(int i);


public:
	/** \brief Create a new tree with n nodes.
	 */
	MaskedBinaryTree(int n);
	~MaskedBinaryTree();

	int getSize();

	/** \brief Calculate total sum of nodes.
	 */
	float total();

	/** \brief Add or change a value of a node in tree.
	 */
	void add(int id, float val);

	/** \brief Choose random node from tree with probability = weight/total()
	 */
	int chooseRandom();

	/** \brief adas
	*/
	int mask(int i);

	/** \brief gdgfd
	*/
	int unmask(int i);

	/** \brief gdgfd
	*/
	bool is_infinite(int i);

	/** \brief gdgfd
	*/
	float find(int i);
};

} /* namespace state */

#endif /* UTIL_MASKEDBINARYTREE_H_ */
