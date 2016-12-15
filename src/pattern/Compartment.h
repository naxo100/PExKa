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
	std::vector<short> dimensions;//[dims];
	int cellsCount;
	state::BaseExpression* volume;

	int firstCell;

public:
	Compartment(const std::string &name,const std::vector<short> &dimensions,state::BaseExpression* volume);
	~Compartment();

	const std::string& getName() const;

	void getCellIndex(int cell_id,std::vector<short> &index) const;
	int getCellId(const std::vector<short> &cell_index) const;

	int getFirstCellId();
	int getLastCellId();

	bool nextCell(std::vector<short>& cell) const;
	const std::vector<short>& getDimensions() const;


	//DEBUG methods
	std::string toString() const;
	std::string cellIdToString(int cell_id) const;
	static std::string cellToString(const std::vector<short>& cell);
};


struct Cell {
	const Compartment &comp;
	const int cellId;

	Cell(const Compartment &c,int n);

	std::string toString() const;
};


class CompartmentExpr {
	const Compartment& comp;
	const std::list<state::BaseExpression*> cellExpr;
	std::vector<std::string> varOrder;
	boost::numeric::ublas::matrix<float> inverseA,transA;
	boost::numeric::ublas::vector<float> b;

	void cellIds(std::list<int>& cell_list,
			std::list<short> *cell_values,std::vector<short> &cell_index,unsigned int dim) const;
public:
	CompartmentExpr(const Compartment& c,const std::list<state::BaseExpression*> &expr);


	//bool nextCell(std::vector<short>& cell) const;
	const Compartment& getCompartment() const;

	/** \brief Replace var_values in expression and return a (list of) cell(s)
	 *
	 *
	 */
	std::list<int> getCells(const std::unordered_map<std::string,int> &var_values) const;

	/** \brief Solve the implicit system for auxiliars in this expression
	 * and return a map with the values for the auxiliars.
	 *
	 */
	void solve(const std::vector<short> &cell_index,std::unordered_map<std::string,int> & vars) const;

	void setEquation();


	std::string toString() const;
};

} /* namespace pattern */

#endif /* PATTERN_COMPARTMENT_H_ */
