/*
 * Compartment.cpp
 *
 *  Created on: Sep 7, 2016
 *      Author: naxo
 */

#include "Compartment.h"

namespace pattern {

/*****************************************
 *********** Class Compartment ***********
 ****************************************/
int Compartment::TOTAL_CELLS = 0;
//template <short dims>
Compartment::Compartment(const std::string &nme,const std::vector<int> &dims,state::BaseExpression* vol)
		:name(nme),dimensions(dims),volume(vol) {
	firstCell = cellsCount;
	cellsCount = 1;
	for(unsigned int i = 0; i < dimensions.size(); i++)
		cellsCount *= dimensions[i];
	TOTAL_CELLS += cellsCount;
}

//template <short dims>
Compartment::~Compartment() {
}

short* Compartment::getCellIndex(int cell_id){
	short* index = new short[dimensions.size()];
	int factor = cellsCount;
	for(unsigned int i = dimensions.size()-1; i >= 0 ;i--){
		factor /= dimensions[i];
		index[i] = cell_id/factor;
		cell_id %= factor;
	}
	return index;
}

int Compartment::getCellId(const short* cell_index){
	int factor = 1, id = 0;
	for(unsigned int i = 0; i < dimensions.size(); i++){
		id += factor*cell_index[i];
		factor *= dimensions[i];
	}
	return id;
}



using namespace boost::numeric::ublas;

//needed for CompExpression::solve()
 /* Matrix inversion routine.
 Uses lu_factorize and lu_substitute in uBLAS to invert a matrix */
template<class T>
bool InvertMatrix(const matrix<T>& input, matrix<T>& inverse);


/************** CompartmentExpr ***********/

CompartmentExpr::CompartmentExpr(const Compartment& c,const std::list<state::BaseExpression*> &expr)
	: comp(c),cellExpr(expr)
{

}

void CompartmentExpr::solve(const std::vector<short> &cell_index,std::unordered_map<std::string,int>& var_values){
	try{
		vector<short,std::vector<short> > cell(cellExpr.size(),cell_index);
		vector<short> result = cell + prod(b,inverseA);
		for(unsigned int i = 0; i < result.size(); i++)
			var_values[varOrder[i]] = result[i];
	}
	catch(std::exception &e){

	}

}


template<class T>
bool InvertMatrix(const matrix<T>& input, matrix<T>& inverse)
{
	typedef permutation_matrix<std::size_t> pmatrix;

	// create a working copy of the input
	matrix<T> A(input);

	// create a permutation matrix for the LU-factorization
	pmatrix pm(A.size1());

	// perform LU-factorization
	int res = lu_factorize(A, pm);
	if (res != 0)
		return false;

	// create identity matrix of "inverse"
	inverse.assign(identity_matrix<T> (A.size1()));

	// backsubstitute to get the inverse
	lu_substitute(A, pm, inverse);

	return true;
}


} /* namespace pattern */
