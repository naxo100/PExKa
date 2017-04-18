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

#include "../../simulation/Simulation.h"

namespace ast {

/** \brief Base class for statements that have %use context.
 *
 * Statements that depends on %use are rules, %var, %obs & %mod.
 */
class Statement {
	short useId;
public:
	Statement();
	short getUseId() const;
};

class Declaration: public Node, public Statement {
public:
	//static int count;
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

	Variable* evalConst(pattern::Environment &env,
			Expression::VAR &vars) const;
	Variable* evalVar(pattern::Environment &env,
				Expression::VAR &vars) const;
	bool isKappa() const;
	bool isConstant() const;
	void setConstant(bool b);


	void show( string tabs = "" );

	//Declaration():label(Id("",yy::location())){}
protected:
	Id name;
	VarType type;
	bool constant;
	union {
		const Expression *expr;
		const Mixture *mixture;
	};
};

//The Number of Agents and the Agents at the start
class Init : public Node, public Statement {
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

	void eval(const pattern::Environment &env,
			const Expression::VAR &vars,simulation::Simulation &state);

};





} /* namespace ast */

#endif /* GRAMMAR_AST_STATEMENTS_H_ */
