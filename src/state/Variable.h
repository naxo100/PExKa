/*
 * Variable.h
 *
 *  Created on: Apr 29, 2016
 *      Author: naxo
 */

#ifndef STATE_VARIABLE_H_
#define STATE_VARIABLE_H_

namespace state {

class Variable {
	union {
		struct {
			Variable *var1;
			Variable *var2;
		} operation;
		float f_val;
		int i_val;
	};
public:
	Variable();
	virtual ~Variable();

	virtual float evaluate(State state);
};

class Constant : Variable {
	float val;
};

} /* namespace state */

#endif /* STATE_VARIABLE_H_ */
