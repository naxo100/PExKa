/*
 * Compartment.cpp
 *
 *  Created on: Sep 7, 2016
 *      Author: naxo
 */

#include "Compartment.h"

namespace pattern {

//template <short dims>
Compartment::Compartment() {
	// TODO Auto-generated constructor stub

}

//template <short dims>
Compartment::~Compartment() {
	// TODO Auto-generated destructor stub
}

using namespace boost::numeric::ublas;
using namespace std;

 /* Matrix inversion routine.
 Uses lu_factorize and lu_substitute in uBLAS to invert a matrix */
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




CompartmentExpr::CompartmentExpr(const Compartment& c,const std::list<state::BaseExpression*> &expr)
	: comp(c),cellExpr(expr)
{

}


} /* namespace pattern */
