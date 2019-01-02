/*
 * BaseExpression.cpp
 *
 *  Created on: Oct 26, 2018
 *      Author: naxo100
 */

#include "BaseExpression.h"
#include "BinaryOperation.h"
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
BaseExpression* BaseExpression::makeBinaryExpression(const BaseExpression *ex1,
		const BaseExpression *ex2, const int op) {
	Type type1 = ex1->getType();
	Type type2 = ex2->getType();
	typedef typename std::conditional<isBool, bool, FL_TYPE>::type BoolOrFloat;
	typedef typename std::conditional<isBool, bool, int>::type BoolOrInt;
	BaseExpression* bin_op = nullptr;

	switch (type1) {
	case FLOAT:
		switch (type2) {
		case FLOAT:
			bin_op = new BinaryOperation<BoolOrFloat, FL_TYPE, float>(ex1, ex2,
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
		const BaseExpression *ex1, const BaseExpression *ex2, const int op);
template BaseExpression* BaseExpression::makeBinaryExpression<false>(
		const BaseExpression *ex1, const BaseExpression *ex2, const int op);



std::string BaseExpression::toString() const {
	return std::string("BaseExpression");
}


} //namespace


