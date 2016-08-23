/*
 * Statements.h
 *
 *  Created on: May 12, 2016
 *      Author: naxo
 */

#ifndef GRAMMAR_AST_STATEMENTS_H_
#define GRAMMAR_AST_STATEMENTS_H_

#include "AlgebraicExpressions.h"
#include "Basics.h"
#include "Dynamics.h"
#include "Spatial.h"

namespace ast {

class Declaration: public Node{
public:
	static int count;
	enum VarType{ALG,KAPPA};
	Declaration();
	Declaration(const location &l,const Id &lab,const Expression *e);
	Declaration(const location &l,const Id &lab,const list<Agent> &m);
	Declaration& operator=(const Declaration &d);
	Declaration(const Declaration &d);
	/*Declaration(const Declaration &&d);
	Declaration& operator=(const Declaration &&d);
*/
	~Declaration();

	Variable* eval(pattern::Environment &env);
	bool isKappa();

	//Declaration():label(Id("",yy::location())){}
protected:
	Id name;
	VarType type;
	union {
		const Expression *expr;
		const MultipleMixture *mixture;
	};
};





} /* namespace ast */

#endif /* GRAMMAR_AST_STATEMENTS_H_ */
