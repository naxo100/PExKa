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

using namespace std;

template<typename R>
class VarLabel: public AlgExpression<R> {
	AlgExpression<R>* var;

public:
	VarLabel(BaseExpression* expr);
	R evaluate(const VarVector &consts,unordered_map<string, int> *aux_values = nullptr)
			const override;
	R evaluate(const state::State& state, const AuxMap& aux_values)
			const override;
	int auxFactors(std::unordered_map<std::string, int> &factor) const override;
	bool operator==(const BaseExpression& exp) const override;
	char getVarDeps() const override;
	//AlgExpression<R>* reduce(const state::State& state,const AuxMap&& aux_values = AuxMap()) const ;
	std::string toString() const override;
};


template<typename R>
class Auxiliar: public AlgExpression<R> {
	std::string name;
public:
	Auxiliar(const std::string &nme);
	~Auxiliar();
	R evaluate(const VarVector &consts,const unordered_map<string, int> *aux_values)
			const override;
	R evaluate(const state::State& state, const AuxMap& aux_values)
			const override;
	FL_TYPE auxFactors(std::unordered_map<std::string, FL_TYPE> &factor) const
			override;
	BaseExpression::Reduction factorize() const
			override;
	BaseExpression* clone() const
			override;
	BaseExpression* deleteElement(BaseExpression* exp) const
			override;
	bool operator==(const BaseExpression& exp) const;
	//std::set<std::string> getAuxiliars() const override;
	AlgExpression<R>* reduce(const state::State& state,
			const AuxMap&& aux_values = AuxMap()) const;

	char getVarDeps() const;

	std::string toString() const override;
};

} /* namespace expressio */

#endif /* SRC_EXPRESSIONS_VARS_H_ */
