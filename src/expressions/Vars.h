/*
 * Vars.h
 *
 *  Created on: Nov 12, 2018
 *      Author: naxo100
 */

#ifndef SRC_EXPRESSIONS_VARS_H_
#define SRC_EXPRESSIONS_VARS_H_

#include "AlgExpression.h"

namespace expressions {

template<typename R>
class VarLabel: public AlgExpression<R> {
	AlgExpression<R>* var;

public:
	VarLabel(BaseExpression* expr);
	R evaluate(std::unordered_map<std::string, int> *aux_values = nullptr) const
			override;
	R evaluate(const state::State& state, const AuxMap& aux_values) const
			override;
	int auxFactors(std::unordered_map<std::string, int> &factor) const override;
	bool operator==(const BaseExpression& exp) const override;
	//AlgExpression<R>* reduce(const state::State& state,const AuxMap&& aux_values = AuxMap()) const ;
	std::string toString() const override;
};


template<typename R>
class Auxiliar: public AlgExpression<R> {
	std::string name;
public:
	Auxiliar(const std::string &nme);
	~Auxiliar();
	R evaluate(const std::unordered_map<std::string, int> *aux_values) const
			override;
	R evaluate(const state::State& state, const AuxMap& aux_values) const
			override;
	FL_TYPE auxFactors(std::unordered_map<std::string, FL_TYPE> &factor) const
			override;
	BaseExpression::Reduction factorizeRate() const
			override;
	BaseExpression* clone() const
			override;
	BaseExpression::DeleteAux deleteElement(std::string exp) const
			override;
	bool operator==(const BaseExpression& exp) const;
	//std::set<std::string> getAuxiliars() const override;
	AlgExpression<R>* reduce(const state::State& state,
			const AuxMap&& aux_values = AuxMap()) const;

	//virtual void getNeutralAuxMap(
	//		std::unordered_map<std::string, FL_TYPE>& aux_map) const;

	std::string toString() const override;
};

} /* namespace expressio */

#endif /* SRC_EXPRESSIONS_VARS_H_ */
