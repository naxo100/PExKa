/*
 * UnaryOperation.h
 *
 *  Created on: Jan 15, 2019
 *      Author: lukas
 */

#ifndef SRC_EXPRESSIONS_UNARYOPERATION_H_
#define SRC_EXPRESSIONS_UNARYOPERATION_H_

#include "AlgExpression.h"


namespace expressions {


template<typename R, typename T>
struct UnaryOperations {
	static R (*funcs[])(T);
};


template<typename R, typename T>
class UnaryOperation: public AlgExpression<R> {
	AlgExpression<T>* exp;
	R (*func)(T);
	const char op;
public:
	R evaluate(const VarVector &consts,
			const std::unordered_map<std::string, int> *aux_values = nullptr) const
					override;
	R evaluate(const state::State& state, const AuxMap& aux_values) const
			override;
	FL_TYPE auxFactors(std::unordered_map<std::string, FL_TYPE> &factor) const
				override;
	//std::set<std::string> getAuxiliars() const override;
	BaseExpression::Reduction factorize(const std::map<std::string,small_id> &aux_cc) const override;
	BaseExpression* reduce(VarVector &vars) override;
	BaseExpression* clone() const override;
	bool operator==(const BaseExpression& exp) const override;
	virtual void setAuxCoords(const std::map<std::string,std::tuple<int,small_id,small_id>>& aux_coords) override;
	virtual ~UnaryOperation();
	UnaryOperation(BaseExpression *ex,
			const short op);

	virtual std::string toString() const override;
};

} /* namespace expression */



#endif /* SRC_EXPRESSIONS_UNARYOPERATION_H_ */
