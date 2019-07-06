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
	int varId;
	string name;

public:
	VarLabel(int id);
	R evaluate(const VarVector &consts,const unordered_map<string, int> *aux_values)
			const override;
	R evaluate(const state::State& state, const AuxMap& aux_values)
			const override;
	FL_TYPE auxFactors(std::unordered_map<std::string, FL_TYPE> &factor) const override;

	BaseExpression::Reduction factorize() const override;
	BaseExpression* clone() const override;


	bool operator==(const BaseExpression& exp) const override;
	char getVarDeps() const override;
	BaseExpression* reduce(VarVector &vars) override;
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
	bool operator==(const BaseExpression& exp) const;
	//std::set<std::string> getAuxiliars() const override;
	BaseExpression* reduce(VarVector &vars) override;

	char getVarDeps() const;
	bool isAux() const override;

	std::string toString() const override;
};

} /* namespace expressio */

#endif /* SRC_EXPRESSIONS_VARS_H_ */
