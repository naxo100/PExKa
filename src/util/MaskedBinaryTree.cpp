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
		size(n),layers(0),isConsistent(false)
{
	nodes = new Node[n+1];
}

MaskedBinaryTree::~MaskedBinaryTree() 
{
	// TODO Auto-generated destructor stub
	
}

void MaskedBinaryTree::add(int i,float val)
{
	float w;
	i=mask_id(i);
	
	if (std::isinf(val)) {
		inf_list.insert(i);
		w=0.0e0;
	} else{
		inf_list.remove(i);
		w=val;
	}
	
	nodes[i]=w;
	declare_unbalanced(i);	
}

pair<int,float> MaskedBinaryTree::chooseRandom()
{
	if(~inf_list.empty())
	{
		
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
	return nodes[mask_id(i)];
}



int getSize()
{
	
}

//private functions 
int MaskedBinaryTree::mask_id(int i)
{
	std::pair<std::map<int,int>::iterator,bool> it;
	it=mask.insert(std::pair<int,int>(i,fresh_id));
	if(it.second)//the id "i" doesnt exist in the map
	{
		it=unmask.insert(unmask.end(),std::pair<int,int>(fresh_id,i));
		fresh_id++;
		return fresh_id;
	}
	else//the id "i" already exist in the map
	{
		return ret.first->second;
	}

}

int MaskedBinaryTree::unmask_id(int m)
{
	it = unmask.find(m);
	if(it != unmask.end())//'m' was found
	{
		return it->second;
	}
	else//'m' doesnt exist
	{
		cout<<"invalid arg: MaskedBinaryTree incoherent hash"<<endl;
		return 0; 
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
