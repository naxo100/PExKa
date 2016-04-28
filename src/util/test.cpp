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
		t1.add(34,1.0);

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator (seed);
	std::uniform_real_distribution<double> distribution(0.0,10.0);	
	for (int i=0; i<20; ++i) {
    	cout<<distribution(generator)<<endl;
  	}
	return 0;	
}

