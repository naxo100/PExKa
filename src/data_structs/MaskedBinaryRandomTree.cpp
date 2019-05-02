/*
 * MaskedBinaryRandomTree.cpp
 *
 *  Created on: Sep 5, 2017
 *      Author: naxo
 */

#include "MaskedBinaryRandomTree.h"
#include <cmath>

namespace data_structs {



template<template <typename,typename...> class Container>
MaskedBinaryRandomTree<Container>::MaskedBinaryRandomTree(std::size_t n,default_random_engine& gen) :
		size(n),sizetree((n+1)/2),fresh_id(1),isConsistent(true),generator(gen)
{
	//initialization of node arrays with zero and false default values
	weights           = new FL_TYPE[sizetree*2+1]();
	nodes             = new FL_TYPE[sizetree*2+1]();
	unmask            = new int[sizetree*2+1]();
	subtrees          = new FL_TYPE[sizetree+1]();
	unbalanced_events = new  bool[sizetree+1]();
	unbalanced_events2 = new  bool[sizetree+1]();

	//layer has to be initializate with a special algorithm
	layer             = new   int[sizetree+1];
	initLayer(1,1,1);//fill the layer
	//unbalanced_subtrees has to be init after layer because need to know what is the bigger layer
	unbalanced_subtrees = new Container<int>[layer[sizetree]+1]();
}

template<template <typename,typename...> class Container>
MaskedBinaryRandomTree<Container>::~MaskedBinaryRandomTree() {
	delete[] weights;
	delete[] nodes;
	delete[] subtrees;
	delete[] layer;
	delete[] unbalanced_events;
	delete[] unbalanced_subtrees;
	delete[] unmask;
}

template<template <typename,typename...> class Container>
void MaskedBinaryRandomTree<Container>::initLayer(int k,int current_layer,int layer_end)
{
	if (k > size)
		return;
	else if (k>layer_end)
		initLayer(k,current_layer+1,2*layer_end+1);
	else
	{
		layer[k] =current_layer;
		initLayer(k+1,current_layer,layer_end);
	}
}

//Agrega o cambia el valor de probabilidad de nodo i (id_externa) por el valor "val"
template<template <typename,typename...> class Container>
void MaskedBinaryRandomTree<Container>::add(int i,FL_TYPE val)
{
	//obtenemos la id interna para modificar su data asociada
	i=mask_id(i);

	//revisamos si el nuevo valor es infinito
	if( std::isinf(val) ) {
		//en caso afirmativo, agregamos el nodo (id_interna) a la lista
		//de reglas con valor infinito
		inf_list.insert(i);
		//y dejamos nulo su valor asociado
		weights[i]=0.0e0;
	} else {
		//en caso de que el nuevo valor no sea infinitp,
		// nos aseguramos de sacarlo de la lista de valores infinitos
		inf_list.erase(i);
		//y actualizamos el valor de su probabilidad asociada
		weights[i]=val;
	}
	//Declaramos que el nodo se modifico y el arbol debe actualizarse
	declare_unbalanced((i+1)/2);
}

//Retorna la id y la probabilidad de una regla del arbol elejida de manera random segun su probabilidad.
template<template <typename,typename...> class Container>
pair<int,FL_TYPE> MaskedBinaryRandomTree<Container>::chooseRandom()
{
	//chequeamos en  "inf_list" si hay reglas con probabilidad infinita
	if(!inf_list.empty())
	{
		//si las hay, devolvemos la primera regla
		std::set<int>::iterator it=inf_list.begin();
		return pair<int,FL_TYPE>(unmask_id(*it),std::numeric_limits<FL_TYPE>::infinity());
	}
	else
	{
		//si no las hay, hacemos una busqueda binaria de la regla
		//Comenzamos por actualizar el arbol binario
		update();
		//obtenemos la probabilidad total
		FL_TYPE a=subtrees[1];
		//si es nula retornamos error -1
		if( a == 0.0e0){
			throw std::invalid_argument("MaskedBinaryTree::chooseRandom(): NotFound");//cout<<"MaskedBinaryTree::chooseRandom(): NotFound"<<endl;
			return pair<int,FL_TYPE>(-1,0.0);
		}
		//si no es nula, hacemos la busqueda
		else{
			//generamos un numero random "r"
			std::uniform_real_distribution<FL_TYPE> distribution(0.0,a);
			FL_TYPE r = a/2;//distribution(generator);
			//partimos de la posicion 1 (nodo root)  del arbol
			int i=1;//position in the tree
			do
			{
				//verificamos si el num random pertenece al intervalo de alguno de los nodos
				//lineales y retornamos en caso afirmativo
				if(r<=nodes[2*i-1])  return pair<int,FL_TYPE>(unmask[2*i-1],weights[2*i-1]);
				if(r<=nodes[2*i])    return pair<int,FL_TYPE>(unmask[2*i],weights[2*i]);
				//si no pertence, chequeamo si pertenece al intervalo de la rama (hijo) izquierda
				r = r-nodes[i];            //random number respecto al intervalo del hijo izquierdo.
				i=i*2;                     //indice rama hijo izquierdo. (cambiado de linea por la de arriba)
				FL_TYPE left = subtrees[i];  //longitud total del hijo izquierdo.
				//si pertenece a esta rama continuamos la busqueda en este intervalo.
				if( r <= left ) continue;
				//si no pertence, pasamos a la rama derecha y continuamos la busqueda
				r=r-left;    //random number respecto al intervalo hijo derecho
				i++;         //indice rama hijo derecho
			} while(true);


		}
	}

}

template<template <typename,typename...> class Container>
FL_TYPE MaskedBinaryRandomTree<Container>::total()
{
	if(inf_list.empty())
	{
		update();
		return subtrees[1];
	}
	else
	{
		return std::numeric_limits<FL_TYPE>::infinity();
	}
}


template<template <typename,typename...> class Container>
bool MaskedBinaryRandomTree<Container>::isInfinite(int i)
{
	return bool(inf_list.count(mask_id(i)));
}

template<template <typename,typename...> class Container>
FL_TYPE MaskedBinaryRandomTree<Container>::find(int i)
{
	return nodes[mask_id(i)];
}

template<template <typename,typename...> class Container>
int MaskedBinaryRandomTree<Container>::getSize() const
{
	return size;
}

//private functions
template<template <typename,typename...> class Container>
void MaskedBinaryRandomTree<Container>::update()
{
	//si el arbol ya esta balanceado, no hacemos nada
	if(isConsistent) return;
	//En caso contrario, actualizamos desde la ultima capa hasta la primera
	for(int k=layer[sizetree]; k>0;k--){
		//recorremos la pila de nodos desbalanceados por capa hasta que este vacia
		while(!unbalanced_subtrees[k].empty())
		{
			//obtenemos la informacion de los nodos desbalanceados
			int p=unbalanced_subtrees[k].top();  //indice binario nodo desbalanceado
			int i=p*2-1;                         //indice lineal sub-nodo asociado
			int q=p/2;                           //indice binario padre
			//limpiamos el elemento que acabamos de leer en la pila
			unbalanced_subtrees[k].pop();
			//Balanceamos los sub-nodos lineales si es es necesario
			if(unbalanced_events2[p]){
				nodes[i]     = weights[i];
				nodes[i+1]   = weights[i] + weights[i+1];
				//marcamos como balanceados estos sub-nodos lineales.
				unbalanced_events2[p]=false;
			}
			//actualizamos el nodo binario.
			subtrees[p]  = nodes[i+1] + weight_of_subtree(2*p) + weight_of_subtree(2*p+1);
			// lo establecemos como balanceado
			unbalanced_events[p]=false;
			// y chequeamos si su padre fue declarado como desbalanceado
			if(unbalanced_events[q]) continue;
			// en caso negativo, declaramos al padre desbalanceado
			unbalanced_events[q]=true;
			//y lo agregamos a la correspondiente pila de nodos desbalanceados
			unbalanced_subtrees[k-1].push(q);
		}
	}
	//indicamos que el arbol esta totalmente balanceado
	isConsistent=true;

}

template<template <typename,typename...> class Container>
inline FL_TYPE MaskedBinaryRandomTree<Container>::weight_of_subtree(int k) const
{
	 return (k > sizetree) ? 0.0: subtrees[k];
}

//trasnforma una id externa "i" en una id interna
//en caso de ser una nueva regla, la funcion
//nos devolvera una nueva de id interna dada por "fresh_id"
//y en caso de ser una regla vieja, nos devolvera su id interna
//ya asociada.
template<template <typename,typename...> class Container>
int MaskedBinaryRandomTree<Container>::mask_id(int i)
{
	//agrega la nueva id al mapa ("i","fresh_id")
	pair<map<int,int>::iterator,bool> ret;
	ret = mask.insert(std::pair<int,int>(i,fresh_id));

	if(ret.second)
	{
		//si la id "i" era nueva en el mapa
		//Se agrega tambien a unmask (mascara opuesta)
		unmask[fresh_id]=i;
		//Se actualiza la id para que apunte a una sin ocupar.
		fresh_id++;
		//Se devuelve la id interna recien agregada
		return fresh_id-1;
	}
	else
	{
		//En caso de que "i" ya se encontraba en el mapa
		//devolvemos el valor al que mapea.
		return ret.first->second;
	}

}

//transforma una id interna "m" en su correspondiente id externa
template<template <typename,typename...> class Container>
inline int MaskedBinaryRandomTree<Container>::unmask_id(int m) const
{
	//retorna el valor de la id externa almacenado en el array unmask.
	return unmask[m];
}

//Declara desbalanceado el nodo binario "i" actualizando
//la informacion relacionada al desbalanceo del arbol binario
template<template <typename,typename...> class Container>
void MaskedBinaryRandomTree<Container>::declare_unbalanced(int i)
{
	//Si el nodo ya fue declarado desbalanceado, retornamos
	if(unbalanced_events[i]) return;
	//en caso contrario,
	//marcamos el nodo binario desbalanceado
	unbalanced_events[i]=true;
	//indicamos que los sub-nodos lineales de este nodo binario
	//sean actualizados tambien.
	unbalanced_events2[i]=true;
	//agregamos el nodo binario a la pila correspondiente
	//(segun su capa) de nodos desbalanceados
	unbalanced_subtrees[layer[i]].push(i);
	//Indicamos que el arbol no esta balanceado
	isConsistent=false;
}

template class MaskedBinaryRandomTree<stack>;

} /* namespace data_structs */
