/*
 * Function.h
 *
 *  Created on: Dec 30, 2019
 *      Author: naxo100
 */

#ifndef SRC_EXPRESSIONS_FUNCTION_H_
#define SRC_EXPRESSIONS_FUNCTION_H_

#include "AlgExpression.h"

//#include <vector>
//#include <list>


namespace expressions {


template<typename T>
struct FunctionPointers {
	static T (*funcs[])(const std::vector<SomeValue>&,const state::State& state);
};


template <typename T>
class Function : public AlgExpression<T> {
	BaseExpression::Funcs f_name;
	T (*func) (const std::vector<SomeValue>&,const state::State& state);
	std::list<BaseExpression*> args;
public:
	Function(BaseExpression::Funcs f,const std::list<BaseExpression*>& l);
	virtual ~Function();

	T evaluate(const VarVector &consts,
				const std::unordered_map<std::string, int> *aux_values = nullptr) const
						override;
	T evaluate(const state::State& state, const AuxMap& aux_values) const
			override;
	FL_TYPE auxFactors(std::unordered_map<std::string, FL_TYPE> &factor) const
			override;
	BaseExpression::Reduction factorize(const std::map<std::string,small_id> &aux_cc) const override;
	BaseExpression* clone() const override;

	BaseExpression* reduce(VarVector& vars);

	//std::set<std::string> getAuxiliars() const override;
	bool operator==(const BaseExpression& exp) const override;

	//void getNeutralAuxMap(
	//		std::unordered_map<std::string, FL_TYPE>& aux_map) const;

	void setAuxCoords(const std::map<std::string,std::tuple<int,small_id,small_id>>& aux_coords) override;

	//return a flag of VarDep
	char getVarDeps() const override;

	std::string toString() const override;

};

} /* namespace pattern */

#endif /* SRC_EXPRESSIONS_FUNCTION_H_ */
