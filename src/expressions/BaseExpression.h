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
#include "../util/Warning.h"
#include "SomeValue.h"
#include <vector>
#include <map>


namespace state {
	class State;
	class Variable;
}

typedef std::vector<state::Variable*> VarVector;

namespace expressions {

//typedef std::unordered_map<std::string, FL_TYPE> AuxMap;
//typedef std::unordered_map<two<small_id>,FL_TYPE> AuxCoords;

template <typename T> class Auxiliar;

template <typename T>
class AuxValueMap {
public:
	virtual ~AuxValueMap();
	virtual FL_TYPE& operator[](const Auxiliar<T>& a) = 0;
	virtual FL_TYPE at(const Auxiliar<T>& a) const = 0;
	virtual void clear() = 0;
	virtual size_t size() const = 0;
};

typedef AuxValueMap<FL_TYPE> AuxMap;



/** \brief Base class for algebraic and every number-evaluated expression.
 *
 *
 *
 */
class BaseExpression {
public:
	struct Reduction {
		// factorized expression = multiplications of all elements in factors * multiplications of all elements in aux_functions
		BaseExpression* factor; // constants and variables of the expression
		std::map<small_id, BaseExpression*> aux_functions; // funcs on aux for each cc-index
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
	enum N_ary {
		SUMATORY,AVERAGE
	};

	//RUN_ID always first of INT op!!
	enum Nullary {
		RAND_1,SIM_TIME,CPUTIME,ACTIVITY,RUN_ID,SIM_EVENT,NULL_EVENT,PROD_EVENT
	};
	enum VarDep {
		CONSTS = 1,RUN = 2,AUX = 4,SPATIAL = 8,TIME = 16,EVENT = 32,RAND = 64,VARDEP = 128
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
			const AuxMap& aux_values) const = 0;
	virtual SomeValue getValue(const state::State& state) const;

	/** \brief Return an int vector that represents this expression
	 * as an equation on auxiliars.
	 *
	 */
	virtual FL_TYPE auxFactors(
			std::unordered_map<std::string, FL_TYPE> &factor) const = 0;


	/** \brief Returns a factorized expression
	 *
	 */
	virtual Reduction factorize(const std::map<std::string,small_id> &aux_cc) const = 0;
	virtual BaseExpression* clone() const = 0;
	virtual BaseExpression* reduce(VarVector& vars) = 0;
	virtual Reduction reduceAndFactorize(const std::map<std::string,small_id> &aux_cc) const;

	virtual void setAuxCoords(const std::map<std::string,std::tuple<int,small_id,small_id>>& aux_coords);

	//virtual std::set<std::string> getAuxiliars() const = 0;
	virtual bool operator==(const BaseExpression& exp) const = 0;
	virtual bool operator!=(const BaseExpression& exp) const;

	template<bool isBool>
	static BaseExpression* makeBinaryExpression(BaseExpression *ex1,
			BaseExpression *ex2, typename std::conditional<isBool,
			BaseExpression::BoolOp,BaseExpression::AlgebraicOp>::type op);

	static BaseExpression* makeUnaryExpression(BaseExpression *ex, const int func);

	static BaseExpression* makeNullaryExpression(const int func);


	//return a flag of VarDep
	virtual char getVarDeps() const;

	virtual bool isAux() const;

	virtual std::string toString() const;

protected:
	//BaseExpression();
	Type t;


	class Unfactorizable : public std::invalid_argument {
	public:
		Unfactorizable(const std::string &msg);
	};

};

}

#endif /* SRC_EXPRESSIONS_BASEEXPRESSION_H_ */
