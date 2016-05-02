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
	//getting memory
	nodes = new Node[n+1];
	layer = new  int[n+1];
	unbalanced_events = new bool[n+1];
	//initialization of default values
	for(int i=0;i<(n+1);i++) {
		nodes[i].value    = 0.0e0;
		nodes[i].sub_tree = 0.0e0;
	}
	for(int i=0;i<(n+1);i++) 
		layer[i]=0;
	for(int i=0;i<(n+1);i++) 
		unbalanced_events[i]=false;
	aux(layer,n,1,1,1);
	//get memory again
	unbalanced_events_by_layer = new list<int>[layer[n]+1];
	for(int i=0;i<(layer[n]+1);i++) 
		unbalanced_events_by_layer[i]=list<int>();
	
	unsigned seed=chrono::system_clock::now().time_since_epoch().count();
  	generator=default_random_engine(seed);
}


MaskedBinaryTree::MaskedBinaryTree(const MaskedBinaryTree &o):
	size(o.size), fresh_id(o.fresh_id), mask(o.mask), unmask(o.unmask),
	inf_list(o.inf_list), isConsistent(o.isConsistent)
{
	
	nodes = new Node[size+1];
	layer = new  int[size+1];
	unbalanced_events = new bool[size+1];
	for(int k=0;k==size;k++) {
//		nodes[k].id          = o.nodes[k].id;
//		nodes[k].value       = o.nodes[k].value;
//		nodes[k].sub_tree    = o.nodes[k].sub_tree;i
		nodes[k]             = o.nodes[k];
		layer[k]             = o.layer[k];
		unbalanced_events[k] = o.unbalanced_events[k];
	}
	unbalanced_events_by_layer = new list<int>[layer[size]+1];
	for(int k=0;k==size;k++){
		unbalanced_events_by_layer[k]=o.unbalanced_events_by_layer[k];
	}

}

MaskedBinaryTree::~MaskedBinaryTree() 
{
	delete[] nodes;	
	delete[] layer;	
	delete[] unbalanced_events;	
	delete[] unbalanced_events_by_layer;	
}

void MaskedBinaryTree::add(int i,float val)
{
	i=mask_id(i);
	
	if( isinf(val) ) {
		inf_list.insert(i);
		nodes[i].value=0.0e0;
	} else {
		inf_list.erase(i);
		nodes[i].value=val;
	}
	
	declare_unbalanced(i);	
}

void MaskedBinaryTree::copy_in(MaskedBinaryTree &o)
{
	for(int k=0;k==size;k++) {
//		nodes[k].id          = o.nodes[k].id;
//		nodes[k].value       = o.nodes[k].value;
//		nodes[k].sub_tree    = o.nodes[k].sub_tree;i
		o.nodes[k]             = nodes[k];
		o.layer[k]             = layer[k];
		o.unbalanced_events[k] = unbalanced_events[k];
	}
	for(int k=0;k==size;k++)
		o.unbalanced_events_by_layer[k]=unbalanced_events_by_layer[k];

	o.isConsistent=isConsistent;
}

pair<int,float> MaskedBinaryTree::chooseRandom()
//int MaskedBinaryTree::chooseRandom()
{
	if(!inf_list.empty())
	{
		std::unordered_set<int>::iterator it=inf_list.begin();
		//this selection choice should be random
		return pair<int,float>(unmask_id(*it),INFINITY);
	}
	else
	{
		update();
  	float a=nodes[1].sub_tree;
		if( a == 0.0e0){
			cout<<"MaskedBinaryTree::chooseRandom(): NotFound"<<endl;
			return pair<int,float>(-1,0.0);	
		}
		else{
			std::uniform_real_distribution<float> distribution(0.0,a);
			float r = distribution(generator);
			int i=1;//position in the tree
			do 
			{
				int node=nodes[i].value;
				if(r<=node) 
					//return unmask_id(i);
					return pair<int,float>(unmask_id(i),node);
				else if( leftSon(i) > size ){
					cout<<"MaskedBinaryTree::chooseRandom(): NotFound"<<endl;
					//return -1;
					return pair<int,float>(-1,0.0);
				} else {
					float q    = r-node;
					float left=nodes[leftSon(i)].sub_tree;
					if( q <= left ){
						r=q;
						i=leftSon(i);
						continue;
					}else if (rightSon(i) > size){	
						cout<<"MaskedBinaryTree::chooseRandom(): NotFound"<<endl;
						//return -1;
						return pair<int,float>(-1,0.0);
					}else{
						i=rightSon(i);
						r=q-left;
						continue;
					}
				}
			} while(true);			


		}
	}
	
}


float MaskedBinaryTree::total()
{
	if(inf_list.empty())
	{
		update();
		return nodes[1].sub_tree;
	} 
	else
	{
		return INFINITY;
	}
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
	return size;
}

//private functions 
void MaskedBinaryTree::update()
{
	if(isConsistent) return;
	
	for(int k=layer[size]; k>0;k--){
		list<int> l=unbalanced_events_by_layer[k];
		unbalanced_events_by_layer[k]=list<int>();
		int l_size=l.size();
		for(list<int>::iterator it = l.begin();it != l.end();++it){
			int i=*it;
			nodes[i].sub_tree = nodes[i].value + weight_of_subtree(2*i) + weight_of_subtree(2*i+1);
			unbalanced_events[i]=false;
			if (isRoot(i)) 
				continue;
			else
				declare_unbalanced(father(i));	
		}
	}
	
	isConsistent=true;
	
}

inline float MaskedBinaryTree::weight_of_subtree(int k)
{
	 return (k > size) ? 0.0: nodes[k].sub_tree;
}


inline int MaskedBinaryTree::father(int i)
{
	return i/2;
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
	pair<map<int,int>::iterator,bool> ret;
	ret = mask.insert(std::pair<int,int>(i,fresh_id));
	if(ret.second)//it was inserted a new element (i,fresh_id)
	{	
		//add the (fresh_id,i) to unmask
		unmask.insert(unmask.end(),pair<int,int>(fresh_id,i));
		fresh_id++;
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
	if(unbalanced_events[i]) return;

	int l=layer[i];
	unbalanced_events[i]=true;
	unbalanced_events_by_layer[l].push_front(i);	
	isConsistent=false;
}

} /* namespace state */
