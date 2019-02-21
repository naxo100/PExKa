/*
 * BaseExpression.h
 *
 *  Created on: Oct 26, 2018
 *      Author: naxo100
 */

#ifndef SRC_EXPRESSIONS_BASEEXPRESSION_H_
#define SRC_EXPRESSIONS_BASEEXPRESSION_H_

#include <vector>
#include <unordered_map>
//#include "../util/params.h"
#include "SomeValue.h"
#include <vector>
#include <map>


namespace state {
	class State;
	class Variable;
}

typedef std::vector<state::Variable*> VarVector;

namespace expressions {

typedef std::unordered_map<std::string, FL_TYPE> AuxMap;

/** \brief Base class for algebraic and every number-evaluated expression.
 *
 *
 *
 */
class BaseExpression {
public:
	struct Reduction {
		std::vector<BaseExpression*> factors;
		std::map<std::string, BaseExpression*> aux_functions;
	};

	enum AlgebraicOp {
		SUM, MINUS, MULT, DIV, POW, MODULO, MAX, MIN
	};
	enum BoolOp {
		AND, OR, GREATER, SMALLER, EQUAL, NOT_EQUAL
	};
	enum Unary {
		SQRT, EXPONENT, LOG, SINE, COSINE, TAN, ATAN, ABS, COIN, RAND_N, NOT
	};
	enum Nullary {
		TRUE, FALSE, RAND_1
	};
	enum VarDep {
		CONSTS = 1,AUX = 2,SPATIAL = 4,TIME = 8,EVENT = 16, RAND = 32
	};

	virtual ~BaseExpression() = 0;
	const virtual Type getType() const;
	//template <Type n> struct TypeDef { typedef float t;};
	template<typename T>
	struct EnumType {
		static const Type t = FLOAT;
	};

	virtual SomeValue getValue(const VarVector &consts,
			const std::unordered_map<std::string, int> *aux_values = nullptr) const = 0;
	virtual SomeValue getValue(const state::State& state,
			const AuxMap&& aux_values = AuxMap()) const = 0;

	/** \brief Return an int vector that represents this expression
	 * as an equation on auxiliars.
	 *
	 */
	virtual FL_TYPE auxFactors(
			std::unordered_map<std::string, FL_TYPE> &factor) const = 0;

	/** \brief Returns a factorized expression
	 *
	 */
	virtual Reduction factorize() const = 0;
	virtual BaseExpression* clone() const = 0;

	//virtual std::set<std::string> getAuxiliars() const = 0;
	virtual bool operator==(const BaseExpression& exp) const = 0;
	virtual bool operator!=(const BaseExpression& exp) const;

	template<bool isBool>
	static BaseExpression* makeBinaryExpression(const BaseExpression *ex1,
			const BaseExpression *ex2, const int op);

	static BaseExpression* makeUnaryExpression(const BaseExpression *ex, const int func);

	static BaseExpression* makeNullaryExpression(const int func);


	//return a flag of VarDep
	virtual char getVarDeps() const;

	virtual std::string toString() const;

protected:
	//BaseExpression();
	Type t;

};

}

#endif /* SRC_EXPRESSIONS_BASEEXPRESSION_H_ */
