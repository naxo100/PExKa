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


class CompExpression: public Node {
public:
	CompExpression();
	CompExpression(const location &l,const Id &id,const list<const Expression*> &dim);

	list<int> evalDimensions(pattern::Environment &env,
			const unordered_map<string,state::Variable*> &vars);
	string evalName(pattern::Environment& env,bool declare=false);

	/** \brief Evaluate compExpression using auxiliars.
	 *
	 * Evaluate this CompExpression //using the values for auxiliars defined
	 * in aux_values. If the auxiliar name is not in aux_values, it is added.
	 * An evaluated expression is returned for every dimension in
	 * CompExpression (as a list).
	 *
	 */
	list<state::BaseExpression*> evalExpression(const pattern::Environment &env,
			const unordered_map<string,state::Variable*> &vars,
			unordered_map<string,int*> *aux_values);


protected:
	Id name;
	list<const Expression*> index;
};

class Compartment : public Node {
	CompExpression comp;
	Expression* volume;
};

class Channel : public Node {
	Id name;
	CompExpression source,target;
	Expression* filter;//bool expr?
	Expression* delay;
public:
	void eval(pattern::Environment &env,
			const unordered_map<string,state::Variable*> &vars);

};

class Use : public Node {
	static unsigned short count;
	const unsigned short id;
	list<CompExpression> compartments;
	Expression* filter;

public:
	//static unsigned short getCount();
	Use(const location &l,const list<CompExpression> &comps);
};



} /* namespace ast */

#endif /* GRAMMAR_AST_SPATIAL_H_ */
