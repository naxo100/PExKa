/*
 * Function.h
 *
 *  Created on: Dec 30, 2019
 *      Author: naxo100
 */

#ifndef SRC_EXPRESSIONS_FUNCTION_H_
#define SRC_EXPRESSIONS_FUNCTION_H_

#include "AlgExpression.h"


namespace expressions {

template <typename T>
class Function : AlgExpression<T> {
	list<BaseExpression*> args;
public:
	Function();
	virtual ~Function();
};

} /* namespace pattern */

#endif /* SRC_EXPRESSIONS_FUNCTION_H_ */
