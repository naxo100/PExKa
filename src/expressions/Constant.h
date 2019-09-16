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
	virtual ~Constant();
	T evaluate(const VarVector &consts,const std::unordered_map<std::string, int> *aux_values = nullptr) const override;
	T evaluate(const state::State& state, const AuxMap& aux_values) const override;
	FL_TYPE auxFactors(std::unordered_map<std::string, FL_TYPE> &factor) const override;
	BaseExpression::Reduction factorize(const std::map<std::string,small_id> aux_cc) const override;
	virtual BaseExpression* reduce(VarVector& vars) override;
	BaseExpression* clone() const override;
	bool operator==(const BaseExpression& exp) const override;

	std::string toString() const override;

};

extern const BaseExpression* INF_EXPR;
extern const BaseExpression* NEG_INF_EXPR;
extern const BaseExpression* MAX_FL_EXPR;
extern const BaseExpression* MIN_FL_EXPR;
extern const BaseExpression* MAX_INT_EXPR;
extern const BaseExpression* MIN_INT_EXPR;
extern const BaseExpression* ONE_FL_EXPR;
extern const BaseExpression* ZERO_FL_EXPR;

} /* namespace expressio */

#endif /* SRC_EXPRESSIONS_CONSTANT_H_ */
