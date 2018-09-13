/*
 * MyMaskedBynaryRandomTree.cpp
 *
 *  Created on: Jun 6, 2018
 *      Author: naxo100
 */

#include "MyMaskedBinaryRandomTree.h"


namespace data_structs {

template<template <typename,typename...> class Container>
MyMaskedBinaryRandomTree<Container>::MyMaskedBinaryRandomTree(std::size_t n,default_random_engine& gen) :
		size(n),isConsistent(true),generator(gen)
{
	//initialization of node arrays with zero and false default values
	wNodes          = new FL_TYPE[size+1]();
	wSubtrees             = new FL_TYPE[size+1]();
	unmask            = new int[size+1]();
	layer          = new int[size+1]();

	unbalancedEvents = new  bool[size+1]();

	//layer has to be initializate with a special algorithm
	initLayer(1,1,1);//fill the layer
	//unbalanced_subtrees has to be init after layer because need to know what is the bigger layer

	unbalancedByLayer = new Container<int>[layer[size]+1]();

	//unsigned seed=1;//=chrono::system_clock::now().time_since_epoch().count();
  	generator=default_random_engine(time(nullptr));
}

template<template <typename,typename...> class Container>
MyMaskedBinaryRandomTree<Container>::~MyMaskedBinaryRandomTree() {
	delete[] wSubtrees;
	delete[] wNodes;
	delete[] unmask;
	delete[] layer;
	delete[] unbalancedEvents;
	delete[] unbalancedByLayer;
}

template<template <typename,typename...> class Container>
void MyMaskedBinaryRandomTree<Container>::initLayer(int k,int current_layer,int layer_end)
{
	if (k > size)
		return;
	else if (k > layer_end)
		initLayer(k,current_layer+1,2*layer_end+1);
	else
	{
		layer[k] =current_layer;
		initLayer(k+1,current_layer,layer_end);
	}
}

//Agrega o cambia el valor de probabilidad de nodo i (id_externa) por el valor "val"
template<template <typename,typename...> class Container>
void MyMaskedBinaryRandomTree<Container>::add(int i,FL_TYPE val)
{
	//obtenemos la id interna para modificar su data asociada
	i=maskId(i);

	//revisamos si el nuevo valor es infinito
	if( std::isinf(val) ) {
		//en caso afirmativo, agregamos el nodo (id_interna) a la lista
		//de reglas con valor infinito
		infList.emplace(i);
		//y dejamos nulo su valor asociado
		wNodes[i] = 0.0e0;
	} else {
		//en caso de que el nuevo valor no sea infinito,
		// nos aseguramos de sacarlo de la lista de valores infinitos
		infList.erase(i);
		//y actualizamos el valor de su probabilidad asociada
		wNodes[i] = val;
	}
	//Declaramos que el nodo se modifico y el arbol debe actualizarse
	declareUnbalanced(i);
}

//Retorna la id y la probabilidad de una regla del arbol elejida de manera random segun su probabilidad.
template<template <typename,typename...> class Container>
pair<int,FL_TYPE> MyMaskedBinaryRandomTree<Container>::chooseRandom()
{
	//chequeamos en  "inf_list" si hay reglas con probabilidad infinita
	if(!infList.empty()) {
		//si las hay, devolvemos la primera regla
		return pair<int,FL_TYPE>(unmaskId(*infList.begin()),std::numeric_limits<FL_TYPE>::infinity());
	}
	else
	{
		//si no las hay, hacemos una busqueda binaria de la regla
		//Comenzamos por actualizar el arbol binario
		update();
		//obtenemos la probabilidad total
		FL_TYPE a=wSubtrees[1];
		//si es nula retornamos error -1
		if( a == 0.0e0)
			throw std::invalid_argument("MaskedBinaryTree::chooseRandom(): NotFound");//cout<<"MaskedBinaryTree::chooseRandom(): NotFound"<<endl;
		//si no es nula, hacemos la busqueda
		else{
			//generamos un numero random "r"
			std::uniform_real_distribution<FL_TYPE> distribution(0.0,a);
			FL_TYPE r = distribution(generator);
			//partimos de la posicion 1 (nodo root)  del arbol
			int i=1;//position in the tree
			do
			{
				//verificamos si el num random pertenece al intervalo de alguno de los nodos
				//lineales y retornamos en caso afirmativo
				if(r < wNodes[i])
					return pair<int,FL_TYPE>(unmask[i],wNodes[i]);
				//si no pertence, chequeamo si pertenece al intervalo de la rama (hijo) izquierda
				r = r-wNodes[i];            //random number respecto al intervalo del hijo izquierdo.
				i=i*2;                     //indice rama hijo izquierdo.
				FL_TYPE left = wSubtrees[i];  //longitud total del hijo izquierdo.
				//si pertenece a esta rama continuamos la busqueda en este intervalo.
				if( r < wSubtrees[i] )
					continue;
				//si no pertence, pasamos a la rama derecha y continuamos la busqueda
				r = r-left;		//random number respecto al intervalo hijo derecho
				i++;			//indice rama hijo derecho
			} while(true);
		}
	}
	throw std::invalid_argument("impossible to reach this code!");
}

template<template <typename,typename...> class Container>
FL_TYPE MyMaskedBinaryRandomTree<Container>::total()
{
	if(infList.empty())
	{
		update();
		return wSubtrees[1];
	}
	else
	{
		return std::numeric_limits<FL_TYPE>::infinity();
	}
}


template<template <typename,typename...> class Container>
bool MyMaskedBinaryRandomTree<Container>::isInfinite(int i)
{
	return infList.count(maskId(i));
}

template<template <typename,typename...> class Container>
FL_TYPE MyMaskedBinaryRandomTree<Container>::find(int i)
{
	return wNodes[maskId(i)];
}

template<template <typename,typename...> class Container>
int MyMaskedBinaryRandomTree<Container>::getSize() const
{
	return size;
}

//private functions
template<template <typename,typename...> class Container>
void MyMaskedBinaryRandomTree<Container>::update()
{
	//si el arbol ya esta balanceado, no hacemos nada
	if(isConsistent)
		return;
	//En caso contrario, actualizamos desde la ultima capa hasta la primera
	for(int k = layer[size]; k > 0; k--){
		//recorremos la pila de nodos desbalanceados por capa hasta que este vacia
		while(!unbalancedByLayer[k].empty())
		{
			//obtenemos la informacion de los nodos desbalanceados
			int p = unbalancedByLayer[k].top();  //indice binario nodo desbalanceado
			//limpiamos el elemento que acabamos de leer en la pila
			unbalancedByLayer[k].pop();
			//actualizamos el nodo binario.
			wSubtrees[p]  = wNodes[p] + wSubtree(2*p) + wSubtree(2*p+1);
			// lo establecemos como balanceado
			unbalancedEvents[p] = false;
			//if is root
			if(p == 1)
				continue;

			int q=p/2;                           //indice binario padre
			// y chequeamos si su padre fue declarado como desbalanceado
			if(unbalancedEvents[q])
				continue;
			// en caso negativo, declaramos al padre desbalanceado
			unbalancedEvents[q]=true;
			//y lo agregamos a la correspondiente pila de nodos desbalanceados
			unbalancedByLayer[k-1].push(q);
		}
	}
	//indicamos que el arbol esta totalmente balanceado
	isConsistent=true;

}

template<template <typename,typename...> class Container>
inline FL_TYPE MyMaskedBinaryRandomTree<Container>::wSubtree(int k) const
{
	 return (k > size) ? 0.0: wSubtrees[k];
}

//trasnforma una id externa "i" en una id interna
//en caso de ser una nueva regla, la funcion
//nos devolvera una nueva de id interna dada por "fresh_id"
//y en caso de ser una regla vieja, nos devolvera su id interna
//ya asociada.
template<template <typename,typename...> class Container>
int MyMaskedBinaryRandomTree<Container>::maskId(int i)
{
	//agrega la nueva id al mapa ("i","fresh_id")
	auto& ret = mask[i];

	if(!ret){	//0 if new
		ret = mask.size();
		unmask[ret]=i;
	}
	return ret;

}

//transforma una id interna "m" en su correspondiente id externa
template<template <typename,typename...> class Container>
inline int MyMaskedBinaryRandomTree<Container>::unmaskId(int m) const
{
	//retorna el valor de la id externa almacenado en el array unmask.
	return unmask[m];
}

//Declara desbalanceado el nodo binario "i" actualizando
//la informacion relacionada al desbalanceo del arbol binario
template<template <typename,typename...> class Container>
void MyMaskedBinaryRandomTree<Container>::declareUnbalanced(int i)
{
	//Si el nodo ya fue declarado desbalanceado, retornamos
	if(unbalancedEvents[i])
		return;
	//en caso contrario,
	//marcamos el nodo binario desbalanceado
	unbalancedEvents[i]=true;
	//indicamos que los sub-nodos lineales de este nodo binario
	//sean actualizados tambien.
	//unbalanced_events2[i]=true;
	//agregamos el nodo binario a la pila correspondiente
	//(segun su capa) de nodos desbalanceados
	unbalancedByLayer[layer[i]].push(i);
	//Indicamos que el arbol no esta balanceado
	isConsistent=false;
}

template class MyMaskedBinaryRandomTree<stack>;

}
