/*
 * RandomTree.h
 *
 *  Created on: Sep 4, 2017
 *      Author: naxo
 *      Based on
 *      implementation
 *      of David Inostroza
 */

#ifndef SRC_DATA_STRUCTS_RANDOMTREE_H_
#define SRC_DATA_STRUCTS_RANDOMTREE_H_

#include <utility>

namespace data_structs {

class RandomTree {
public:

	/** \brief Create a new chain with n nodes.
	 */
	//RandomTree();

	/** \brief Destroy the Chain and free all memory locations.
	 */
	virtual ~RandomTree();


	/** Update all the nodes.
	*/
	virtual void update() = 0;

	/** \brief Number of nodes of the Chain.
	 */
	virtual int getSize() const = 0;

	/** \brief Calculate total sum of nodes.
	 */
	virtual float total() = 0;

	/** \brief Add or change a value of a node in the chain.
	 */
	virtual void add(int id, float val) = 0;

	/** \brief Choose random node from tree with probability = weight/total().
	 */
	virtual std::pair<int,float> chooseRandom() = 0;

	/** \brief Check if the rule "i" has an infinity probability.
	*/
	virtual bool isInfinite(int i) = 0;

	/** \brief Return the probability of the rule "i"
	*/
	virtual float find(int i) = 0;

};

} /* namespace simulation */

#endif /* SRC_UTIL_RANDOMTREE_H_ */
