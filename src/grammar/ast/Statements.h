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

/** \brief Base class for statements that have %use context.
 *
 * Statements that depends on %use are rules, %var, %obs & %mod.
 */
class Statement {
	short use_id;
};

class Declaration: public Node, public Statement {
public:
	static int count;
	enum VarType{ALG,KAPPA};
	Declaration();
	Declaration(const location &l,const Id &lab,const Expression *e);
	Declaration(const location &l,const Id &lab,const Mixture &m);
	Declaration& operator=(const Declaration &d);
	Declaration(const Declaration &d);
	/*Declaration(const Declaration &&d);
	Declaration& operator=(const Declaration &&d);
*/
	~Declaration();

	Variable* eval(pattern::Environment &env,
			unordered_map<string,Variable*> &vars) const;
	bool isKappa();

	//Declaration():label(Id("",yy::location())){}
protected:
	Id name;
	VarType type;
	union {
		const Expression *expr;
		const Mixture *mixture;
	};
};

//The Number of Agents and the Agents at the start
class Init : public Node {
	enum InitType {MIXTURE,TOKEN} type;
	const Expression *alg;
	union {
		Mixture mixture;
		Id token;
	};
public:
	Init();
	Init(const location &l,const Expression *e, const Mixture &mix);
	Init(const location &l,const Expression *e, const Id &tok);

	Init(const Init &init);
	Init& operator=(const Init &init);
	~Init();

};





} /* namespace ast */

#endif /* GRAMMAR_AST_STATEMENTS_H_ */
