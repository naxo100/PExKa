/*
 * Constant.h
 *
 *  Created on: Nov 12, 2018
 *      Author: naxo100
 */

#ifndef SRC_EXPRESSIONS_CONSTANT_H_
#define SRC_EXPRESSIONS_CONSTANT_H_

#include "AlgExpression.h"

namespace expressions {

template<typename T>
class Constant: public AlgExpression<T> {
	T val;
public:
	Constant(T v);
	T evaluate(const std::unordered_map<std::string, int> *aux_values = nullptr) const override;
	T evaluate(const state::State& state, const AuxMap& aux_values) const override;
	FL_TYPE auxFactors(std::unordered_map<std::string, FL_TYPE> &factor) const override;
	BaseExpression::Reduction factorizeRate() const override;
	BaseExpression* clone() const override;
	BaseExpression::DeleteAux deleteElement(std::string exp) const override;
	bool operator==(const BaseExpression& exp) const override;

	std::string toString() const override;
};

} /* namespace expressio */

#endif /* SRC_EXPRESSIONS_CONSTANT_H_ */
