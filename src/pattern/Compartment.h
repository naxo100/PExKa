/*
 * Compartment.h
 *
 *  Created on: Sep 7, 2016
 *      Author: naxo
 */

#ifndef PATTERN_COMPARTMENT_H_
#define PATTERN_COMPARTMENT_H_

#include <string>
#include <list>
#include <unordered_map>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>
#include "../state/AlgExpression.h"

namespace pattern {

//template <short dims>
class Compartment {
	std::string name;
	int* dimensions;//[dims];
	int totalCells;
	float volume;

public:
	Compartment();
	~Compartment();

	int* getCellId(int id);
};


class CompartmentExpr {
	const Compartment& comp;
	const std::list<state::BaseExpression*> cellExpr;
	boost::numeric::ublas::matrix<int> inverseA;
	boost::numeric::ublas::vector<int> B;
public:
	CompartmentExpr(const Compartment& c,const std::list<state::BaseExpression*> &expr);

	std::list<int*> getCells(const std::unordered_map<std::string,int> &var_values);
	std::unordered_map<std::string,int> getVarValues(const short* cell);

};

} /* namespace pattern */

#endif /* PATTERN_COMPARTMENT_H_ */
