/*
 * AlgExpression.h
 *
 *  Created on: Jul 22, 2016
 *      Author: naxo
 */

#ifndef STATE_ALGEXPRESSION_H_
#define STATE_ALGEXPRESSION_H_

#include <unordered_map>
#include <vector>
#include <string>
#include <set>
#include "../util/params.h"

namespace state {

class SomeValue;

/** \brief Base class for algebraic and every number-evaluated expression.
 *
 *
 *
 */
class BaseExpression {
public:
	enum Type {FLOAT,INT,BOOL,SMALL_ID,SHORT_ID,STR};
	enum AlgebraicOp {SUM,MINUS,MULT,DIV,POW,MODULO,MAX,MIN};
	enum BoolOp {AND,OR,GREATER,SMALLER,EQUAL,DIFF};
	enum Unary {SQRT,EXPONENT,LOG,SINE,COSINE,TAN,ATAN,ABS,
			COIN,RAND_N,NOT};
	enum Nullary {TRUE,FALSE,RAND_1};

	virtual ~BaseExpression() = 0;
	const virtual Type getType() const;
	//template <Type n> struct TypeDef { typedef float t;};
	template <typename T>
	struct EnumType {static const Type t = FLOAT;};

	virtual const SomeValue getValue(const std::unordered_map<std::string,int> *aux_values = nullptr) const = 0;

	/** \brief Return an int vector that represents this expression
	 * as an equation on auxiliars.
	 *
	 */
	virtual float auxFactors(std::unordered_map<std::string,float> &factor) const = 0;

	virtual std::set<std::string> getAuxiliars() const;

	template <bool isBool>
	static BaseExpression* makeBinaryExpression(const BaseExpression *ex1,const BaseExpression *ex2,
			const int op);

protected:
	//BaseExpression();
	Type t;
};

template <typename R,typename T1,typename T2>
struct BinaryOperations {
	static R (*funcs[]) (T1,T2);
};

template <typename T1,typename T2>
struct BinaryOperations<bool,T1,T2> {
	static bool (*funcs[]) (T1,T2);
};

template <typename T>
class AlgExpression : public virtual BaseExpression{
public:
	AlgExpression();
	virtual ~AlgExpression() = 0;
	virtual T evaluate(const std::unordered_map<std::string,int> *aux_values = nullptr) const = 0;
	virtual float auxFactors(std::unordered_map<std::string,float> &factor) const override = 0;
	virtual const SomeValue getValue(const std::unordered_map<std::string,int> *aux_values = nullptr) const override;
};

/*
struct SomeAlgExpression : public BaseExpression {
	union {
		AlgExpression<float>* floatExp;
		AlgExpression<int>* intExp;
		AlgExpression<bool>* boolExp;
	};
	template <bool isBool>
	static SomeAlgExpression makeBinaryOperation
			(const BaseExpression &ex1,const BaseExpression &ex2,
				const int op);
};*/

class SomeValue {
private:
public:
	union {
		float fVal;
		int iVal;
		small_id smallVal;
		short_id shortVal;
		bool bVal;
		//std::string* sVal;
	};
	BaseExpression::Type t;

	SomeValue(float f);
	SomeValue(int i);
	SomeValue(short_id id);
	SomeValue(small_id id);
	SomeValue(bool b);
	//SomeValue(const std::string &s);
	//SomeValue(const std::string *s);

	template <typename T>
	void set(T val);

	template <typename T>
	void safeSet(T val);

	template <typename T>
	T valueAs() const;

	bool operator!=(const SomeValue&) const;


};

template<typename T>
class Constant : public AlgExpression<T> {
	T val;
public:
	Constant(T v);
	T evaluate(const std::unordered_map<std::string,int> *aux_values = nullptr) const override;
	float auxFactors(std::unordered_map<std::string,float> &factor) const override;
};

template<typename R,typename T1,typename T2>
class BinaryOperation : public AlgExpression<R> {
	const AlgExpression<T1>* exp1;
	const AlgExpression<T2>* exp2;
	//static R (*operations[]) (T1,T2);
	R (*func) (T1,T2);
	const char op;
public:
	R evaluate(const std::unordered_map<std::string,int> *aux_values = nullptr) const override;
	float auxFactors(std::unordered_map<std::string,float> &factor) const override;
	std::set<std::string> getAuxiliars() const override;
	~BinaryOperation();
	BinaryOperation
			(const BaseExpression *ex1,const BaseExpression *ex2,const short op);
};

template <typename R>
class VarLabel : public AlgExpression<R> {
	AlgExpression<R>* var;

public:
	VarLabel(BaseExpression* expr);
	R evaluate(std::unordered_map<std::string,int> *aux_values = nullptr) const override;
	int auxFactors(std::unordered_map<std::string,int> &factor) const override;
};


class Auxiliar : public AlgExpression<int> {
	std::string name;
public:
	Auxiliar(const std::string &nme);
	~Auxiliar();
	int evaluate(const std::unordered_map<std::string,int> *aux_values) const override;
	float auxFactors(std::unordered_map<std::string,float> &factor) const override;

	std::set<std::string> getAuxiliars() const override;
};


} /* namespace state */

#endif /* STATE_ALGEBRAICEXPRESSION_H_ */
