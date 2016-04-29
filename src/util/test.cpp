#include <iostream>
#include <random>
#include <chrono>
#include "MaskedBinaryTree.h"


using namespace std;
using namespace util;

int main(){
	int size;
	cout<<"Ingrese el tamano del arbol binario:"<<endl;
	cin>>size;
	MaskedBinaryTree t1(size);
	for(int i=0;i<size;i++)
		t1.add(2*i+3,1.0);
	
	pair<int,float> p;
	for(int i=0;i<size;i++){
		p=t1.chooseRandom();
		cout<<"id  : "<<p.first<<endl;
		cout<<"rate: "<<p.second<<endl;
	}

	return 0;	
}

