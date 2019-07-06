/*
 * BinaryOperation.h
 *
 *  Created on: Oct 29, 2018
 *      Author: naxo100
 */

#ifndef SRC_EXPRESSIONS_BINARYOPERATION_H_
#define SRC_EXPRESSIONS_BINARYOPERATION_H_

#include "AlgExpression.h"
#include <vector>


namespace simulation {
	class Perturbation;
}
namespace expressions {


template<typename R, typename T1, typename T2>
struct BinaryOperations {
	static R (*funcs[])(T1, T2);
};

template<typename T1, typename T2>
struct BinaryOperations<bool, T1, T2> {
	static bool (*funcs[])(T1, T2);
};


template<typename R, typename T1, typename T2>
class BinaryOperation: public AlgExpression<R> {
	AlgExpression<T1>* exp1;
	AlgExpression<T2>* exp2;
	//static R (*operations[]) (T1,T2);
	R (*func)(T1, T2);
	const char op;

	friend class simulation::Perturbation;
public:
	R evaluate(const VarVector &consts,
			const std::unordered_map<std::string, int> *aux_values = nullptr) const
					override;
	R evaluate(const state::State& state, const AuxMap& aux_values) const
			override;
	FL_TYPE auxFactors(std::unordered_map<std::string, FL_TYPE> &factor) const
			override;
	BaseExpression::Reduction factorize() const override;
	BaseExpression* clone() const override;

	BaseExpression* reduce(VarVector& vars);

	//std::set<std::string> getAuxiliars() const override;
	bool operator==(const BaseExpression& exp) const override;
	~BinaryOperation();
	BinaryOperation(BaseExpression *ex1, BaseExpression *ex2,
			const short op);

	void getNeutralAuxMap(
			std::unordered_map<std::string, FL_TYPE>& aux_map) const;

	//return a flag of VarDep
	char getVarDeps() const override;

	std::string toString() const override;
};

} /* namespace expression */

#endif /* SRC_EXPRESSIONS_BINARYOPERATION_H_ */
