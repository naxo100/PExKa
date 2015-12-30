/*
 * Statement.h
 *
 *  Created on: Dec 28, 2015
 *      Author: naxo
 */

#ifndef GRAMMAR_AST_STATEMENT_H_
#define GRAMMAR_AST_STATEMENT_H_

class Statement {
protected:
	FILE* file;
	int line;
	int character;
public:
	Statement();
	virtual ~Statement();
};

#endif /* GRAMMAR_AST_STATEMENT_H_ */
