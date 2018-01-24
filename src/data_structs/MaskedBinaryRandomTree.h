/*
 * MaskedBinaryRandomTree.h
 *
 *  Created on: Sep 5, 2017
 *      Author: naxo
 */

#ifndef SRC_DATA_STRUCTS_MASKEDBINARYRANDOMTREE_H_
#define SRC_DATA_STRUCTS_MASKEDBINARYRANDOMTREE_H_

#include "RandomTree.h"
#include <stack>
#include <set>
#include <map>
#include <random>

namespace data_structs {
using namespace std;

template<template <typename,typename...> class Container>
class MaskedBinaryRandomTree: public RandomTree {
	int size;						///<Numero de sub-nodos lineales.
	int sizetree;					///<Numero de nodos arbol binario.
	FL_TYPE* weights;					///<Probabilidad de cada sub-nodo lineal.
	FL_TYPE* nodes;					///<Probabilidad Acumulada de cada sub-nodo lineal dentro del nodo binario (sub_nodo_1=weight_1, sub_nodo_2=sub_nodo_1+weight_2).
	FL_TYPE* subtrees;				///<Suma Probabilidad de los 2 sub-nodos lineales mas todos los sub-nodos de sus decendientes binarios.
	int fresh_id;					///<Id del arbol disponible para ser ocupada por una nueva regla.
	map<int, int> mask;				///<Mapa entre la Id externa de una regla y la Id interna (Posicion) del Arbol binario.
	int* unmask;					///<Mapa opuesto, la posicion en este array indica la Id interna y el valor indica la Id externa.
	set<int> inf_list;				///<Lista de nodos (ids internas) cuyas Probabilidades son infinitas.
	int* layer;						///<Capa a la que pertenece cada nodo binario.
	void initLayer(int, int, int);	///<Inicia los valores de layer.

	bool isConsistent;				///<Indica si el arbol esta o no esta actualizado.
	bool* unbalanced_events;		///<Indica que nodos binarios han sido agregados a "unbalanced_subtrees".
	Container<int>* unbalanced_subtrees;///<Array de Pilas (una por cada capa del arbol binario) que almacena las nodos del arbol binario (posiciones del array subtrees) que deben ser actualizadas.
	bool* unbalanced_events2;		///<Array que indica cuales nodos del arbol binario deben tambien actualizar sus sub-nodos lineales (array nodes)

	int mask_id(int i);				///<Transforma una id externa en una id interna
	int unmask_id(int i) const;		///<Transforma una id interna en una id externa

	FL_TYPE weight_of_subtree(int i) const;	///<Valor de subtrees[i], nulo en caso de no existir el nodo "i".

	void declare_unbalanced(int i);			///<Agrega el nodo "i" a la correpondiente fila (capa) de la matriz "unbalanced_events_by_layer".

	default_random_engine& generator;					///<Generador Base de Numeros Randoms
	//uniform_real_distribution<double> random_real;		///<Generador de Numeros Randoms

public:
	/** \brief Create a new chain with n nodes.
	 */
	MaskedBinaryRandomTree(std::size_t n,default_random_engine& gen);
	~MaskedBinaryRandomTree();

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



} /* namespace simulation */



#endif /* SRC_DATA_STRUCTS_MASKEDBINARYRANDOMTREE_H_ */
