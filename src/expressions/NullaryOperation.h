/*
 * NullaryOperation.h
 *
 *  Created on: Jan 15, 2019
 *      Author: lukas
 */

#ifndef SRC_EXPRESSIONS_NULLARYOPERATION_H_
#define SRC_EXPRESSIONS_NULLARYOPERATION_H_

#include "AlgExpression.h"


namespace expressions {


template<typename R>
struct NullaryOperations {
	static R (*funcs[])();
};


template<typename R>
class NullaryOperation: public AlgExpression<R> {
	R (*func)();
	const char op;
public:
	R evaluate(
			const std::unordered_map<std::string, int> *aux_values = nullptr) const
					override;
	R evaluate(const state::State& state, const AuxMap& aux_values) const
			override;
	FL_TYPE auxFactors(std::unordered_map<std::string, FL_TYPE> &factor) const
				override;
	//std::set<std::string> getAuxiliars() const override;
	bool operator==(const BaseExpression& exp) const override;
	~NullaryOperation();
	NullaryOperation(const short op);

	virtual std::string toString() const override;
};

} /* namespace expression */



#endif /* SRC_EXPRESSIONS_NULLARYOPERATION_H_ */