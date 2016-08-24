/*
 * AlgExpression.h
 *
 *  Created on: Jul 22, 2016
 *      Author: naxo
 */

#ifndef STATE_ALGEXPRESSION_H_
#define STATE_ALGEXPRESSION_H_

namespace state {

class BaseExpression {
public:
	enum Type {FLOAT,INT,BOOL};
	enum AlgebraicOp {SUM,MINUS,MULT,DIV,POW,MODULO,MAX,MIN};
	enum BoolOp {AND,OR,GREATER,SMALLER,EQUAL,DIFF};
	enum Unary {SQRT,EXPONENT,LOG,SINUS,COSINUS,TAN,ATAN,ABS,
			COIN,RAND_N,NOT};
	enum Nullary {TRUE,FALSE,RAND_1};

	virtual ~BaseExpression() = 0;
	const virtual Type getType() const;
	template <Type n> struct TypeDef { typedef float t;};
	template <typename T>
	struct EnumType {static const Type t = FLOAT;};

	//template <> struct TypeDef<INT>;
	//template <> struct TypeDef<BOOL>;
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
class AlgExpression : public BaseExpression{
public:
	AlgExpression();
	virtual ~AlgExpression() = 0;
	virtual T evaluate(/*const State &state*/) const = 0;
};

struct SomeAlgExpression {
	union {
		AlgExpression<float>* floatExp;
		AlgExpression<int>* intExp;
		AlgExpression<bool>* boolExp;
	};
	BaseExpression::Type t;
	template <bool isBool>
	static SomeAlgExpression makeBinaryOperation
			(const SomeAlgExpression &ex1,const SomeAlgExpression &ex2,
				const int op);
};

class SomeValue {
	union {
		float fVal;
		int iVal;
		bool bVal;
	};
	BaseExpression::Type t;

public:
	SomeValue(float f);
	SomeValue(int i);
	SomeValue(bool b);

};

template<typename T>
class Constant : public AlgExpression<T> {
	T val;
public:
	Constant(T v);
	T evaluate(/*const State &state*/) const;
};

template<typename R,typename T1,typename T2>
class BinaryOperation : public AlgExpression<R> {
	const AlgExpression<T1>* exp1;
	const AlgExpression<T2>* exp2;
	//static R (*operations[]) (T1,T2);
	R (*func) (T1,T2);
public:
	R evaluate(/*const State &state*/) const;
public:
	~BinaryOperation();
	BinaryOperation
			(const AlgExpression<T1> *ex1,const AlgExpression<T2> *ex2,const int op);
};

template <typename R>
class Var : public AlgExpression<R> {
	int id;

	R evaluate() const;
};


} /* namespace state */

#endif /* STATE_ALGEBRAICEXPRESSION_H_ */
