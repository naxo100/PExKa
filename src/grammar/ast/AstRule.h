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
	list<ast::Agent> lhs;
	list<ast::Agent> rhs;
	ast::Arrow arrow;
	ast::Num	rate;

	ast::Id label;


public:
	AstRule();
	virtual ~AstRule();
};

#endif /* GRAMMAR_AST_ASTRULE_H_ */
