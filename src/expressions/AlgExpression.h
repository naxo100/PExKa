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
#include <list>
#include <string>
#include <set>
#include "BaseExpression.h"
//#include "../util/params.h"

namespace expressions {

struct Reduction {
	FL_TYPE constant;
	BaseExpression* factor_vars;
	BaseExpression* aux;
};

template<typename T>
class AlgExpression: public virtual BaseExpression {
public:
	AlgExpression();
	virtual ~AlgExpression() = 0;
	virtual T evaluate(const VarVector& consts,
			const std::unordered_map<std::string, int> *aux_values = nullptr) const = 0;
	virtual T evaluate(const state::State& state,
			const AuxMap& aux_values) const = 0;
	virtual FL_TYPE auxFactors(
			std::unordered_map<std::string, FL_TYPE> &factor) const
					override = 0;
	virtual SomeValue getValue(const VarVector &consts,
			const std::unordered_map<std::string, int> *aux_values = nullptr) const
					override;
	virtual SomeValue getValue(const state::State& state,
			const AuxMap&& aux_values = AuxMap()) const override;
	virtual bool operator==(const BaseExpression& exp) const override = 0;

	//virtual void getNeutralAuxMap(
	//		std::unordered_map<std::string, FL_TYPE>& aux_map) const = 0;
	//virtual Reduction reduce(const state::State& state,const AuxMap&& aux_values = AuxMap()) const;
	//virtual T auxPoly(std::list<AlgExpression<T>>& poly) const = 0;
};

} /* namespace expression */


#endif /* STATE_ALGEBRAICEXPRESSION_H_ */

