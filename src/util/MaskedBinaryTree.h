/*
 * MaskedBinaryTree.h
 *
 *  Created on: Apr 19, 2016
 *      Author: naxo
 */

#ifndef UTIL_MASKEDBINARYTREE_H_
#define UTIL_MASKEDBINARYTREE_H_

#include <map>;

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
	const int fresh_id;
	Node* nodes;
	map<int,int> mask;
	map<int,int> unmask;

	int layers;
	bool isConsistent;

	void update();
	void leftSon(int i);
	void rightSon(int i);


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
	void add(int id, int val);

	/** \brief Choose random node from tree with probability = weight/total()
	 */
	int chooseRandom();


};

} /* namespace state */

#endif /* UTIL_MASKEDBINARYTREE_H_ */
