/*
 * MaskedBinaryTree.h
 *
 *  Created on: Apr 19, 2016
 *      Author: naxo
 */

#ifndef UTIL_MASKEDBINARYTREE_H_
#define UTIL_MASKEDBINARYTREE_H_

#include <iostream>
#include <map>
#include <set>
#include <list>
#include <cmath>
#include <cfloat>
#include <random>
#include <chrono>


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
	int                 fresh_id;
	Node*               nodes;
	map<int,int>        mask;
	map<int,int>        unmask;
	set<int>            inf_list;///<the list of infinite-rate rule
	bool*               unbalanced_events;
	list<int>*          unbalanced_events_by_layer;

	int mask_id(int i);
	int unmask_id(int i);


	int* layer;
	bool isConsistent;

	void  update();
	//inline  functions
	float weight_of_subtree(int i);
	int   pere(int i);
	int   father(int i);//same that pere
	int   leftSon(int i);
	int   rightSon(int i);
	
	bool isLeftSon(int i);
	bool isRightSon(int i);
	bool isRoot(int i);

	void declare_unbalanced(int i);

	void aux(int*,int,int,int,int);
	
	default_random_engine             generator;
	uniform_real_distribution<double> random_real;
	
public:
	/** \brief Create a new tree with n nodes.
	 */
	MaskedBinaryTree(int n);
	MaskedBinaryTree(const MaskedBinaryTree &o);//copy constructor
	~MaskedBinaryTree();

	/** \brief Calculate total sum of nodes.
	 */
	void copy_in(MaskedBinaryTree &o);

	int getSize();

	/** \brief Calculate total sum of nodes.
	 */
	float total();

	/** \brief Add or change a value of a node in tree.
	 */
	void add(int id, float val);

	/** \brief Choose random node from tree with probability = weight/total()
	 */
	std::pair<int,float> chooseRandom();

	/** \brief gdgfd
	*/
	bool is_infinite(int i);

	/** \brief gdgfd
	*/
	float find(int i);
};

} /* namespace state */

#endif /* UTIL_MASKEDBINARYTREE_H_ */
