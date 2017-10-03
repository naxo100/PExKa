/*
 * MaskedBinaryRandomTree.h
 *
 *  Created on: Sep 5, 2017
 *      Author: naxo
 */

#ifndef SRC_DATA_STRUCTS_MASKEDBINARYRANDOMTREE_H_
#define SRC_DATA_STRUCTS_MASKEDBINARYRANDOMTREE_H_

#include "RandomTree.h"

namespace data_structs {

template <typename Container>
class MaskedBinaryRandomTree : public RandomTree {
public:
	/** \brief Create a new chain with n nodes.
	 */
	MaskedBinaryRandomTree(std::size_t n);
	~MaskedBinaryRandomTree();

	/** Update all the nodes.
	*/
	void update() override;


	/** \brief Number of nodes of the Chain.
	 */
	int getSize() const override;

	/** \brief Calculate total sum of nodes.
	 */
	float total() const override;

	/** \brief Add or change a value of a node in the chain.
	 */
	void add(int id, float val) override;

	/** \brief Choose random node from tree with probability = weight/total().
	 */
	std::pair<int,float> chooseRandom() const override;

	/** \brief Check if the rule "i" has an infinity probability.
	*/
	bool is_infinite(int i) const override;

	/** \brief Return the probability of the rule "i"
	*/
	float find(int i) const override;
};

} /* namespace simulation */

#endif /* SRC_DATA_STRUCTS_MASKEDBINARYRANDOMTREE_H_ */
