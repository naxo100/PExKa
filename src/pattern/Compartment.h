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
#include "../state/Variable.h"

namespace pattern {

//template <short dims>
class Compartment {
	static unsigned int TOTAL_CELLS;
	std::string name;
	std::vector<short> dimensions;//[dims];
	int cellsCount;
	const expressions::BaseExpression* volume;

	int firstCell;

public:
	Compartment(const std::string &name);
	~Compartment();

	void setDimensions(const std::vector<short> &dims);
	void setVolume(const expressions::BaseExpression* vol);

	const std::string& getName() const;
	const expressions::BaseExpression& getVolume() const;

	void getCellIndex(int cell_id,std::vector<short> &index) const;
	int getCellId(const std::vector<short> &cell_index) const;

	int getFirstCellId() const;
	int getLastCellId() const;

	bool nextCell(std::vector<short>& cell) const;
	const std::vector<short>& getDimensions() const;

	static unsigned int getTotalCells();


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
	const std::list<const expressions::BaseExpression*> cellExpr;
	std::vector<std::string> varOrder;
	boost::numeric::ublas::matrix<FL_TYPE> A,inverseA,transA;
	boost::numeric::ublas::vector<FL_TYPE> b;

	void cellIds(std::list<int>& cell_list,
			std::list<short> *cell_values,std::vector<short> &cell_index,unsigned int dim) const;
public:
	CompartmentExpr(const Compartment& c,const std::list<const expressions::BaseExpression*> &expr);


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
	 * @param cell_index values for indexes of compartment. Will be used to fill
	 * the vector b.
	 * @param aux_values a map of var_name -> var_value. The values of auxiliars
	 * that solve the system will be set here. If an auxliar value was set before
	 * and is not equal to the solution of the system, an exception is thrown.
	 */
	void solve(const std::vector<short> &cell_index,std::unordered_map<std::string,int> & aux_values) const;

	void setEquation();


	std::string toString() const;
};

class UseExpression : std::vector<CompartmentExpr> {
	//short id;
	//std::list<CompartmentExpr> comps;
	//const state::BaseExpression* filter;
	const expressions::AlgExpression<bool> *filter;
	std::set<int> *cells;
	bool isComplete;
	static std::set<int> ALL_CELLS;
public:
	UseExpression(size_t comps_count,const expressions::BaseExpression* where = nullptr);
	~UseExpression();


	void evaluateCells(UseExpression::iterator it = UseExpression::iterator(),
			std::unordered_map<std::string,int> var_values = std::unordered_map<std::string,int>()
		);
	//void emplaceCompExpression(const CompartmentExpr &c);
	using std::vector<CompartmentExpr>::emplace_back;
	using std::vector<CompartmentExpr>::begin;
	//void emplace_back(const pattern::Compartment&,const list<const state::BaseExpression*>&);

	const std::set<int>& getCells() const;

};

} /* namespace pattern */

#endif /* PATTERN_COMPARTMENT_H_ */
