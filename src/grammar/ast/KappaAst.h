/*
 * KappaAst.h
 *
 *  Created on: Jan 8, 2016
 *      Author: naxo
 */

#ifndef GRAMMAR_AST_KAPPAAST_H_
#define GRAMMAR_AST_KAPPAAST_H_

#include "../KappaParser.hpp"
#include <string>
#include <list>

/*#undef YY_DECL
#define YY_DECL \
  yy::KappaParser::symbol_type yylex (ast::KappaAst& driver)
// ... and declare it for the parser's sake.
YY_DECL;*/
namespace ast {

class KappaAst {
	list<std::string> files;
	yy::KappaParser parser;

public:
	KappaAst(char* fls[],int flsc);
	KappaAst();
	virtual ~KappaAst();

	int parse();
};

} /* namespace ast */

#endif /* GRAMMAR_AST_KAPPAAST_H_ */
