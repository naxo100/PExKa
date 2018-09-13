/*
 * MyMaskedBynaryRandomTree.h
 *
 *  Created on: Jun 6, 2018
 *      Author: naxo100
 */

#ifndef SRC_DATA_STRUCTS_MYMASKEDBINARYRANDOMTREE_H_
#define SRC_DATA_STRUCTS_MYMASKEDBINARYRANDOMTREE_H_

#include "RandomTree.h"
#include <random>
#include <unordered_map>
#include <set>
#include <list>
#include <stack>


namespace data_structs {
using namespace std;

template<template <typename,typename...> class Container>
class MyMaskedBinaryRandomTree : public RandomTree {
	unordered_map<int,int> mask;
	int* unmask;
	set<int> infList;
	int size;
	FL_TYPE *wNodes,*wSubtrees;
	Container<int>* unbalancedByLayer;
	bool* unbalancedEvents;
	int* layer;
	bool isConsistent;

	default_random_engine& generator;

	void initLayer(int, int, int);	///<Inicia los valores de layer.
	int maskId(int i);				///<Transforma una id externa en una id interna
	int unmaskId(int i) const;		///<Transforma una id interna en una id externa

	FL_TYPE wSubtree(int i) const;	///<Valor de subtrees[i], nulo en caso de no existir el nodo "i".

	void declareUnbalanced(int i);			///<Agrega el nodo "i" a la correpondiente fila (capa) de la matriz "unbalanced_events_by_layer".


public:
	MyMaskedBinaryRandomTree(std::size_t n,default_random_engine& gen);
	virtual ~MyMaskedBinaryRandomTree();

	/** \brief Update all the nodes.
	 */
	void update() override;

	/** \brief Number of nodes of the Chain.
	 */
	int getSize() const override;

	/** \brief Calculate total sum of nodes.
	 */
	FL_TYPE total() override;

	/** \brief Add or change a value of a node in the chain.
	 */
	void add(int id, FL_TYPE val) override;

	/** \brief Choose random node from tree with probability = weight/total().
	 */
	std::pair<int, FL_TYPE> chooseRandom() override;

	/** \brief Check if the rule "i" has an infinity probability.
	 */
	bool isInfinite(int i) override;

	/** \brief Return the probability of the rule "i"
	 */
	FL_TYPE find(int i) override;
};

}

#endif /* SRC_DATA_STRUCTS_MYMASKEDBINARYRANDOMTREE_H_ */
