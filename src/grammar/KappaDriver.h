/*
 * KappaDriver.h
 *
 *  Created on: Jan 8, 2016
 *      Author: naxo
 */

#ifndef GRAMMAR_KAPPADRIVER_H_
#define GRAMMAR_KAPPADRIVER_H_


#include <string>
#include <list>
#include <vector>
#include <fstream>
#include "ast/KappaAst.h"
#include "location.hh"
#include "KappaLexer.h"
#include "KappaParser.hpp"

using namespace std;
/*#define YY_DECL \
  yy::KappaParser::symbol_type yylex (ast::KappaDriver& driver)
// ... and declare it for the parser's sake.
YY_DECL;*/
namespace grammar {

class KappaDriver {
	vector<string> files;
	vector<string>::iterator fit;
	yy::KappaParser parser;
	yy::KappaLexer lexer;
	yy::location loc;
	ast::KappaAst syntax_tree;

public:
	KappaDriver(const vector<string> &files);
	KappaDriver();
	virtual ~KappaDriver();

	int parse();
	yy::KappaParser::symbol_type getNextToken();

	string* getCurrentFileName() const;
	yy::location* getLocation();

	ast::KappaAst getAst();
};

} /* namespace ast */

#endif /* GRAMMAR_KAPPADRIVER_H_ */
