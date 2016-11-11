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

	pattern::CompartmentExpr* eval(pattern::Environment& env,
			const unordered_map<string,state::Variable*> &vars,
			bool allowAux);

	list<int> evalDimensions(pattern::Environment &env,
			const vector<Variable*> &vars);
	short evalName(pattern::Environment& env,bool declare=false);

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
			map<string,int*> *aux_values);


protected:
	Id name;
	list<const Expression*> index;
};

class Compartment : public Node {
	CompExpression comp;
	Expression* volume;
public:
	Compartment(const location& l,const CompExpression& comp_exp,
			Expression* exp);
	short eval(pattern::Environment &env,const vector<Variable*> &vars);
};

class Channel : public Node {
	Id name;
	CompExpression source,target;
	bool bidirectional;
	Expression* filter;//bool expr?
	Expression* delay;
public:
	Channel(const location& l,const Id& nme, const CompExpression& src,
			const CompExpression& trgt, const Expression* where,
			const Expression* delay);
	pattern::Channel* eval(pattern::Environment &env,
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
