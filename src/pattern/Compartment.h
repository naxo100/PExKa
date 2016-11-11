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
	static int TOTAL_CELLS;
	std::string name;
	std::vector<int> dimensions;//[dims];
	int cellsCount;
	state::BaseExpression* volume;

	int firstCell;

public:
	Compartment(const std::string &name,const std::vector<int> &dimensions,state::BaseExpression* volume);
	~Compartment();

	short* getCellIndex(int cell_id);
	int getCellId(const short* cell_index);

	int getFirstCellId();
	int getLastCellId();
};


class CompartmentExpr {
	const Compartment& comp;
	const std::list<state::BaseExpression*> cellExpr;
	const std::string* varOrder;
	boost::numeric::ublas::matrix<short> inverseA;
	boost::numeric::ublas::vector<short> b;
public:
	CompartmentExpr(const Compartment& c,const std::list<state::BaseExpression*> &expr);

	/** \brief Replace var_values in expression and return a (list of) cell(s)
	 *
	 *
	 */
	std::list<int*> getCells(const std::unordered_map<std::string,int> &var_values);

	/** \brief Solve the implicit system for auxiliars in this expression
	 * and return a map with the values for the auxiliars.
	 *
	 */
	void solve(const std::vector<short> &cell_index,std::unordered_map<std::string,int> & vars);

};

} /* namespace pattern */

#endif /* PATTERN_COMPARTMENT_H_ */
