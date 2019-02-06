/*
 * BaseExpression.h
 *
 *  Created on: Oct 26, 2018
 *      Author: naxo100
 */

#ifndef SRC_EXPRESSIONS_BASEEXPRESSION_H_
#define SRC_EXPRESSIONS_BASEEXPRESSION_H_

#include <unordered_map>
//#include "../util/params.h"
#include "SomeValue.h"
#include <vector>

namespace state {
	class State;
}

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
		std::vector<FL_TYPE> constant;
		std::vector<BaseExpression*> factor_vars;
		std::vector<BaseExpression*> aux;
		BaseExpression* factorized_expression;
	};

	struct DeleteAux{
		BaseExpression* expression;
		bool deleted;
	};
	enum AlgebraicOp {
		SUM, MINUS, MULT, DIV, POW, MODULO, MAX, MIN
	};
	enum BoolOp {
		AND, OR, GREATER, SMALLER, EQUAL, DIFF
	};
	enum Unary {
		SQRT, EXPONENT, LOG, SINE, COSINE, TAN, ATAN, ABS, COIN, RAND_N, NOT
	};
	enum Nullary {
		TRUE, FALSE, RAND_1
	};

	virtual ~BaseExpression() = 0;
	const virtual Type getType() const;
	//template <Type n> struct TypeDef { typedef float t;};
	template<typename T>
	struct EnumType {
		static const Type t = FLOAT;
	};

	virtual SomeValue getValue(
			const std::unordered_map<std::string, int> *aux_values = nullptr) const = 0;
	virtual SomeValue getValue(const state::State& state,
			const AuxMap&& aux_values = AuxMap()) const = 0;

	/** \brief Return an int vector that represents this expression
	 * as an equation on auxiliars.
	 *
	 */
	virtual FL_TYPE auxFactors(
			std::unordered_map<std::string, FL_TYPE> &factor) const = 0;

	virtual Reduction factorizeRate() const = 0;
	virtual BaseExpression* clone() const = 0;
	virtual DeleteAux deleteElement(std::string exp) const = 0;


	//virtual std::set<std::string> getAuxiliars() const = 0;
	virtual bool operator==(const BaseExpression& exp) const = 0;
	virtual bool operator!=(const BaseExpression& exp) const;

	template<bool isBool>
	static BaseExpression* makeBinaryExpression(const BaseExpression *ex1,
			const BaseExpression *ex2, const int op);

	static BaseExpression* makeUnaryExpression(const BaseExpression *ex, const int func);

	static BaseExpression* makeNullaryExpression(const int func);

	virtual std::string toString() const;

protected:
	//BaseExpression();
	Type t;

};

}

#endif /* SRC_EXPRESSIONS_BASEEXPRESSION_H_ */
