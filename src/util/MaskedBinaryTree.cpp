/*
 * MaskedBinaryTree.cpp
 *
 *  Created on: Apr 19, 2016
 *      Author: naxo
 */

#include "MaskedBinaryTree.h"

namespace util {

MaskedBinaryTree::MaskedBinaryTree(int n) :
		size(n),layers(0),isConsistent(false)
{
	nodes = new Node[n+1];
}

MaskedBinaryTree::~MaskedBinaryTree() {
	// TODO Auto-generated destructor stub
}

} /* namespace state */
