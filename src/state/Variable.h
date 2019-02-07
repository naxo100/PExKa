/*
 * Variable.h
 *
 *  Created on: Apr 29, 2016
 *      Author: naxo
 */

#ifndef STATE_VARIABLE_H_
#define STATE_VARIABLE_H_

#include <string>

//#include "../expressions/AlgExpression.h"
#include "../expressions/Constant.h"
#include "../pattern/mixture/Mixture.h"
//#include "State.h"

namespace state {

using namespace expressions;
using namespace std;
/** \brief Declared variables using '%var:' in Kappa
 *
 *
 *
 */
class Variable : public virtual BaseExpression {
	const short id;
	const std::string name;//TODO &??
	bool isObservable;
	bool updated;

public:
	Variable(const short id, const std::string &nme,const bool is_obs = false);
	virtual ~Variable() = 0;

	//template <typename T>
	//virtual SomeValue getValue(/*const State &state*/) const = 0;
	//virtual int getValue() const = 0;
	//virtual bool getValue() const = 0;
	//const std::string& getName() const;
	virtual std::string toString() const override;

};

template <typename T>
class AlgebraicVar : public Variable, public AlgExpression<T> {
	const AlgExpression<T>* expression;
public:
	AlgebraicVar(const short var_id, const std::string &nme,const bool is_obs,
			const AlgExpression<T> *exp);

	virtual FL_TYPE auxFactors(std::unordered_map<std::string,FL_TYPE> &factor) const override;

	BaseExpression::Reduction factorizeRate() const override;
	BaseExpression* clone() const override;
	BaseExpression::DeleteAux deleteElement(std::string exp) const override;

	virtual T evaluate(const VarVector& consts,const unordered_map<string,int> *aux_values = nullptr) const override;
	virtual T evaluate(const state::State& state,const AuxMap& aux_values) const override;

	virtual bool operator==(const BaseExpression& exp) const override;
};

template <typename T>
class ConstantVar : public Variable, public Constant<T> {
public:
	ConstantVar(const short var_id, const std::string &name, const AlgExpression<T> *exp);

	std::string toString() const override;

	//FL_TYPE auxFactors(std::unordered_map<std::string,FL_TYPE> &factor) const override;
	//T evaluate(const std::unordered_map<std::string,int> *aux_values = nullptr) const override;
	//T evaluate(const state::State& state,const AuxMap& aux_values) const override;
};

class KappaVar : public AlgExpression<int>, public Variable {
	const pattern::Mixture& mixture;
public:
	KappaVar(const short var_id, const std::string &nme,const bool is_obs,
			const pattern::Mixture &kappa);

	FL_TYPE auxFactors(std::unordered_map<std::string,FL_TYPE> &factor) const override;
	
	BaseExpression::Reduction factorizeRate() const override;
	BaseExpression* clone() const override;
	BaseExpression::DeleteAux deleteElement(std::string exp) const override;
	
	int evaluate(const VarVector &consts,const unordered_map<string,int> *aux_values ) const override;
	int evaluate(const state::State& state,const AuxMap& aux_values) const override;


	virtual bool operator==(const BaseExpression& exp) const override;
	const pattern::Mixture& getMix() const;

};

//template <typename FL_TYPE>
class RateVar : public Variable, public AlgExpression<FL_TYPE> {
	const AlgExpression<FL_TYPE>* expression;
	list<int> cc_deps;
public:
	RateVar(const short var_id, const std::string &nme,const bool is_obs,
			const AlgExpression<FL_TYPE> *exp);

	virtual FL_TYPE auxFactors(std::unordered_map<std::string,FL_TYPE> &factor) const override;
	virtual FL_TYPE evaluate(const VarVector &consts,const unordered_map<string,int> *aux_values = nullptr) const override;
	virtual FL_TYPE evaluate(const state::State& state,const AuxMap& aux_values) const override;

	virtual bool operator==(const BaseExpression& exp) const override;
};

class TokenVar: public AlgExpression<FL_TYPE> {
	unsigned id;
public:
	TokenVar(unsigned _id);
	FL_TYPE evaluate(const VarVector &consts,
			const unordered_map<string, int> *aux_values) const override;
	FL_TYPE evaluate(const state::State& state, const AuxMap& aux_values) const
			override;
	FL_TYPE auxFactors(std::unordered_map<std::string, FL_TYPE> &factor) const
			override;

	bool operator==(const BaseExpression& exp) const override;

	//virtual void getNeutralAuxMap(
	//		std::unordered_map<std::string, FL_TYPE>& aux_map) const;
};

} /* namespace state */

#endif /* STATE_VARIABLE_H_ */
