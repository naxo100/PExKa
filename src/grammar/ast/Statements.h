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

namespace grammar {
namespace ast {

/** \brief Base class for statements that have %use context.
 *
 * Statements that depends on %use are rules, %var, %obs & %mod.	*/
class Statement {
	short useId;
public:
	Statement();
	short getUseId() const;
};

/** \brief Variable declarations and Observable.
 *
 * AST of a %var and %obs. It's evaluated as a state::Variable. */
class Declaration: public Node, public Statement {
public:
	/** \brief The var types: Algebraic, Kappa, or Auxiliar Expression for internal values distributions.*/
	enum VarType{ALG,KAPPA,AUX_EXPR};
	Declaration();
	Declaration(const location &l,const Id &lab,const Expression *e);
	Declaration(const location &l,const Id &lab,const Mixture &m);
	Declaration(const location &l,const Id &lab,
			pair<BaseExpression::N_ary,const Expression*> e,const Mixture &m);
	Declaration& operator=(const Declaration &d);
	Declaration(const Declaration &d);
	/*Declaration(const Declaration &&d);
	Declaration& operator=(const Declaration &&d);
*/
	~Declaration();

	/** Evaluate a %const declaration.
	 *
	 * %const are evaluated before %vars because dependencies.
	 * Consts have a fixed value before the simulation begins.
	 * @returns a pointer to a state::ConstantVar object. 			*/
	Variable* evalConst(pattern::Environment &env,
			Expression::VAR &vars) const;

	/** Evaluate a %var declaration.
	 *
	 * Vars should be evaluated in the order they appear in the kappa files because dependencies.
	 * And they should be deleted in reverse order.
	 * @returns a pointer to a state::Variable object.		*/
	Variable* evalVar(pattern::Environment &env,
				Expression::VAR &vars) const;
	bool isKappa() const;
	bool isConstant() const;
	void setConstant(bool b);
	bool isObservable() const;
	void setObservable(bool v = true);


	void show( string tabs = "" );

	//Declaration():label(Id("",yy::location())){}
protected:
	Id name;
	VarType type;
	bool constant;
	bool observable;

	const Expression *expr;
	const Mixture *mixture;

	BaseExpression::N_ary op;
};

/** \brief Initialization of agent/token instances (%init). */
class Init : public Node, public Statement {
	enum InitType {MIXTURE,TOKEN} type;	///< The type of initialization (Agents or Tokens).
	const Expression *alg;
	//union {
		Mixture mixture;
		Id token;
	//};
public:
	Init();
	Init(const location &l,const Expression *e, const Mixture &mix);
	Init(const location &l,const Expression *e, const Id &tok);

	Init(const Init &init);
	Init& operator=(const Init &init);
	~Init();

	/** \brief Evaluate the %init ASTs to add agents and tokens into the Simulation object.  */
	void eval(const pattern::Environment &env,
			const Expression::VAR &vars,simulation::Simulation &sim);

};





} /* namespace ast */
}

#endif /* GRAMMAR_AST_STATEMENTS_H_ */
