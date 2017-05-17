/*
 * Spatial.h
 *
 *  Created on: May 12, 2016
 *      Author: naxo
 */

#ifndef GRAMMAR_AST_SPATIAL_H_
#define GRAMMAR_AST_SPATIAL_H_

#include <string>
#include <set>

#include "AlgebraicExpressions.h"


namespace ast {
using namespace std;


/** \brief Representation of a compartment expression.
 *
 * Stores the data of a compartment expression. It includes the name
 * and a list of integer alg. expressions and can represent a compartment
 * declaration or an expression of a set of cells using auxiliars.
 */
class CompExpression: public Node {
public:
	/** \brief Empty constructor.
	 * Just needed by the parser.
	 */
	CompExpression();
	/** \brief CompExpression constructor.
	 * @param l location of whole expression
	 * @param id Id of the compartment
	 * @param indexlist set of index expressions of the compartment.
	 */
	CompExpression(const location &l,const Id &id,const list<const Expression*> &indexlist);

	pattern::CompartmentExpr* eval(pattern::Environment& env,
			const unordered_map<string,state::Variable*> &vars,
			bool allowAux);

	/** \brief Evaluate this expression as if every index expression is a
	 * constant int.
	 *
	 * Force alg. expression of each index to have int values and return
	 * a vector of short ints representing the index list.
	 * @param env the environment of the simulation.
	 * @param consts the variable vector (only constant values can be used).
	 * @return a short int for every index as a vector.
	 */
	vector<short> evalDimensions(const pattern::Environment &env,
			const vector<Variable*> &consts) const;

	/** \brief Tests if this compartment is declared and returns its Id.
	 *
	 * Tests if this compartment is declared only if 'declare' is false.
	 * then returns the Id.
	 * @param env the environment of the simulation.
	 * @param declare true if this compartment has not ben declared.
	 * @returns The Id of this compExpression.
	 */
	const Id& evalName(const pattern::Environment& env,bool declare=false) const;

	/** \brief Evaluate this expression as it can represent several cells.
	 *
	 *	Evaluate alg. expressions for each index of compartment and returns
	 * a list of BaseExpression* for index.
	 * @param env the environment of the simulation.
	 * @param consts the variable vector (only constants vars can be accessed).
	 * @return a list of BaseExpression* for each index of compExpression by copy.
	 */
	list<const state::BaseExpression*> evalExpression(const pattern::Environment &env,
			const vector<state::Variable*> &consts) const;


protected:
	Id name;
	list<const Expression*> indexList;
};

class Compartment : public Node {
	CompExpression comp;
	Expression* volume;
public:
	Compartment(const location& l,const CompExpression& comp_exp,
			Expression* exp);
	void eval(pattern::Environment &env,const vector<Variable*> &vars);
};

class Channel : public Node {
	Id name;
	CompExpression source,target;
	bool bidirectional;
	const Expression* filter;//bool expr?
	const Expression* delay;
public:
	Channel(const location& l,const Id& nme, const CompExpression& src,
			const CompExpression& trgt, bool bckwrds, const Expression* where=nullptr,
			const Expression* delay=nullptr);
	void eval(pattern::Environment &env,
			const vector<Variable*> &vars);

};

class Use : public Node {
	static unsigned short count;
	const unsigned short id;
	list<CompExpression> compartments;
	const Expression* filter;

public:
	static unsigned short getCount();
	Use(const location &l,
		const list<CompExpression> &comps = list<CompExpression>(),
		const Expression* where = nullptr);
	Use(const Use& u);
	~Use();

	void eval(pattern::Environment &env,const Expression::VAR &consts) const;
};



} /* namespace ast */

#endif /* GRAMMAR_AST_SPATIAL_H_ */
