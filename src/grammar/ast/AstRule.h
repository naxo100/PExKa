/*
 * AstRule.h
 *
 *  Created on: Dec 28, 2015
 *      Author: naxo
 */

#ifndef GRAMMAR_AST_ASTRULE_H_
#define GRAMMAR_AST_ASTRULE_H_

#include "AstStructs.h"

class AstRule : public Statement {
protected:
	list<Agent> lhs;
	list<Agent> rhs;
	Arrow arrow;
	Num	rate;

	Id label;


public:
	AstRule();
	virtual ~AstRule();
};

#endif /* GRAMMAR_AST_ASTRULE_H_ */
