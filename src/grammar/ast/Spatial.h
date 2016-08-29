/*
 * Spatial.h
 *
 *  Created on: May 12, 2016
 *      Author: naxo
 */

#ifndef GRAMMAR_AST_SPATIAL_H_
#define GRAMMAR_AST_SPATIAL_H_

#include <string>

#include "AlgebraicExpressions.h"


namespace ast {
using namespace std;


class CompExpression: public Node {
public:
	CompExpression();
	CompExpression(const location &l,const Id &id,const list<const Expression*> &dim);
	CompExpression(const location &l,const Id &id,const list<const Expression*> &dim,const Expression* w);
protected:
	Id name;
	list<const Expression*> index;
	const Expression* where;
};

class Compartment : public Node {

};



} /* namespace ast */

#endif /* GRAMMAR_AST_SPATIAL_H_ */
