/*
 * BaseExpression.cpp
 *
 *  Created on: Oct 26, 2018
 *      Author: naxo100
 */

#include "BaseExpression.h"
#include "BinaryOperation.h"
#include "UnaryOperation.h"
#include "NullaryOperation.h"
#include "../util/Exceptions.h" //semanticError

namespace expressions {

template<> struct BaseExpression::EnumType<int> {
	static const Type t = INT;
};
template<> struct BaseExpression::EnumType<bool> {
	static const Type t = BOOL;
};

/****** BaseExpression *******/
const Type BaseExpression::getType() const {
	return t;
}

bool BaseExpression::operator!=(const BaseExpression& exp) const {
	return !((*this) == exp);
}

/*std::set<std::string> BaseExpression::getAuxiliars() const {
 return std::set<std::string>();
 }*/

BaseExpression::~BaseExpression() {
}
;

/*SomeValue BaseExpression::getValue(const std::unordered_map<std::string,int> *aux_values = nullptr) const{
 throw std::invalid_argument("Cannot call this expression without state.");
 }
 SomeValue BaseExpression::getValue(const state::State& state) const{
 throw std::invalid_argument("Cannot call this expression without auxiliars.");
 }*/

template<bool isBool>
BaseExpression* BaseExpression::makeBinaryExpression(BaseExpression *ex1,
		BaseExpression *ex2, const int op) {
	Type type1 = ex1->getType();
	Type type2 = ex2->getType();
	typedef typename std::conditional<isBool, bool, FL_TYPE>::type BoolOrFloat;
	typedef typename std::conditional<isBool, bool, int>::type BoolOrInt;
	BaseExpression* bin_op = nullptr;

	switch (type1) {
	case FLOAT:
		switch (type2) {
		case FLOAT:
			bin_op = new BinaryOperation<BoolOrFloat, FL_TYPE, FL_TYPE>(ex1, ex2,
					op);
			break;
		case INT:
			bin_op = new BinaryOperation<BoolOrFloat, FL_TYPE, int>(ex1, ex2,
					op);
			break;
		case BOOL:
			bin_op = new BinaryOperation<BoolOrFloat, FL_TYPE, bool>(ex1, ex2,
					op);
			break;
		default:
			SemanticError("Not a valid value for a binary operation",
					yy::location());
		}
		break;
	case INT:
		switch (type2) {
		case FLOAT:
			bin_op = new BinaryOperation<BoolOrFloat, int, FL_TYPE>(ex1, ex2,
					op);
			break;
		case INT:
			bin_op = new BinaryOperation<BoolOrInt, int, int>(ex1, ex2, op);
			break;
		case BOOL:
			bin_op = new BinaryOperation<BoolOrInt, int, bool>(ex1, ex2, op);
			break;
		default:
			SemanticError("Not a valid value for a binary operation",
					yy::location());
		}
		break;
	case BOOL:
		switch (type2) {
		case FLOAT:
			bin_op = new BinaryOperation<BoolOrFloat, bool, FL_TYPE>(ex1, ex2,
					op);
			break;
		case INT:
			bin_op = new BinaryOperation<BoolOrInt, bool, int>(ex1, ex2, op);
			break;
		case BOOL:
			bin_op = new BinaryOperation<bool, bool, bool>(ex1, ex2, op);
			break;
		default:
			SemanticError("Not a valid value for a binary operation",
					yy::location());
		}
		break;
	default:
		SemanticError("Not a valid value for a binary operation",
				yy::location());
	}

	return bin_op;
}
template BaseExpression* BaseExpression::makeBinaryExpression<true>(
		BaseExpression *ex1, BaseExpression *ex2, const int op);
template BaseExpression* BaseExpression::makeBinaryExpression<false>(
		BaseExpression *ex1, BaseExpression *ex2, const int op);


BaseExpression* BaseExpression::makeUnaryExpression(BaseExpression *ex,
		const int func) {
	Type type = ex->getType();

	BaseExpression* un_op = nullptr;

	switch (type) {
	case FLOAT:
		un_op = new UnaryOperation<FL_TYPE, FL_TYPE>(ex, func);
		break;
	case INT:
		un_op = new UnaryOperation<int, int>(ex, func);
		break;
	default:
		SemanticError("Not a valid value for a unary operation",
				yy::location());
	}

	return un_op;
}

BaseExpression* BaseExpression::makeNullaryExpression(const int func) {
	BaseExpression* null_op = nullptr;

	if(func <= Nullary::ACTIVITY)
		null_op = new NullaryOperation<FL_TYPE>(func);
	else
		null_op = new NullaryOperation<int>(func - Nullary::ACTIVITY);
	//SemanticError("Not a valid value for a nullary operation",
	//		yy::location());
	return null_op;
}


char BaseExpression::getVarDeps() const{
	return '\0';
}

bool BaseExpression::isAux() const {
	return false;
}

std::string BaseExpression::toString() const {
	return std::string("BaseExpression");
}


} //namespace


