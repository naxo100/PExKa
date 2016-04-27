/*
 * MaskedBinaryTree.cpp
 *
 *  Created on: Apr 19, 2016
 *      Author: naxo
 */

#include "MaskedBinaryTree.h"

namespace util {
//public functions
MaskedBinaryTree::MaskedBinaryTree(int n) :
		size(n),fresh_id(1),isConsistent(true)
{
	nodes = new Node[n+1];
	layer = new  int[n+1];
	unbalanced_events = new bool[n+1];
	//initialization of values
	for(int i=0;i<(n+1);i++) {
		nodes[i].value    = 0.0e0;
		nodes[i].sub_tree = 0.0e0;
	}
	for(int i=0;i<(n+1);i++) 
		layer[i]=0;
	for(int i=0;i<(n+1);i++) 
		unbalanced_events[i]=false;
	aux(layer,n,1,1,1);
	unbalanced_events_by_layer = new list<int>[layer[n]+1];
}

MaskedBinaryTree::~MaskedBinaryTree() 
{
	// TODO Auto-generated destructor stub
	
}

void MaskedBinaryTree::add(int i,float val)
{
	i=mask_id(i);
	
	if( isinf(val) ) {
		inf_list.insert(i);
		nodes[i]=0.0e0;
	} else {
		inf_list.erase(i);
		nodes[i]=val;
	}
	
	declare_unbalanced(i);	
}

pair<int,float> MaskedBinaryTree::chooseRandom()
{
	if(~inf_list.empty())
	{
		std::set<int>::iterator it=inf_list.begin();
		return pair<int,float>(*it,INFINITY);
	}
	else
	{
			
	}
	
}


float total()
{
	
}



bool MaskedBinaryTree::is_infinite(int i)
{
	return bool(inf_list.count(mask_id(i)));
}

float MaskedBinaryTree::find(int i)
{
	return nodes[mask_id(i)].value;
}



int MaskedBinaryTree::getSize()
{
	
}

//private functions 
void MaskedBinaryTree::update()
{
	if(isConsistent) return;
	
	for(int k=layer[size]; k>0;k--){
		list<int >l=unbalanced_events_by_layer[k];
		unbalanced_events_by_layer(k)=list<int>();
		l_size=l.size();
		for (std::vector<int>::iterator it = myvector.begin() ; it != myvector.end(); ++it){
			int i=*it
			nodes[i].sub_tree = nodes[i].value + weight_of_subtree(2*i) + weight_of_subtree(2*i+1);
			unbalanced_events[i]=false;
			if (is_root(i)) 
				return;
			else{
				int father = i / 2;
				declare_unbalanced(father);	
			}
		}
	}
	
	isConsistent=true;
	return;
}

inline void MaskedBinaryTree::weight_of_subtree(int k)
{
	 return (k > size) :0.0: weight_of_subtrees(k);
}


inline int MaskedBinaryTree::pere(int i)
{
	return i/2;
}

inline int MaskedBinaryTree::leftSon(int i)
{
	return i*2;
}


inline int MaskedBinaryTree::rightSon(int i)
{
	return i*2+1;
}

inline bool MaskedBinaryTree::isLeftSon(int i)
{
	return (i > 1) && (i%2 == 0);
}

inline bool MaskedBinaryTree::isRightSon(int i)
{
	return (i > 1) && (i%2 == 1);
}

inline bool MaskedBinaryTree::isRoot(int i)
{
	return i == 1;
}

void MaskedBinaryTree::aux(int* layer,int n, int k,int current_layer,int layer_end)
{
	if (k > n)
		return;
	else if (k>layer_end)
		aux(layer,n,k,current_layer+1,2*layer_end+1);
	else
	{
		layer[k] =current_layer;
		aux(layer,n,k+1,current_layer,layer_end);
	}	
}

int MaskedBinaryTree::mask_id(int i)
{
	int m;
	pair<map<int,int>::iterator,bool> ret;
	ret = mask.insert(std::pair<int,int>(i,fresh_id));
	if(ret.second)//it was inserted a new element (i,fresh_id)
	{	
		//add the (fresh_id,i) to unmask
		unmask.insert(unmask.end(),pair<int,int>(fresh_id,i));
		fresh_id;
		return fresh_id-1;
	}
	else//the key wasnt exist
	{
		return ret.first->second;
	}

}

int MaskedBinaryTree::unmask_id(int m)
{
	std::map<int,int>::iterator it = unmask.find(m);
	if(it != unmask.end())//'m' was found
	{
		return it->second;
	}
	else//'m' doesnt exist
	{
		cout<<"invalid arg: MaskedBinaryTree incoherent hash"<<endl;
		return -1; 
	}
}

void MaskedBinaryTree::declare_unbalanced(int i)
{
	//i=mask_id(i);
	if(unbalanced_events[i])
	{
		return;	
	} 
	else
	{
		int l=layer[i];
		unbalanced_events[i]=true;
		unbalanced_events_by_layer[l].push_front(i);
	}
	
	isConsistent=false;
}

} /* namespace state */
