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

	list<ast::Perturbation> perts;
	list<ast::Declaration> decls;

public:
	KappaDriver(const vector<string> &files);
	KappaDriver();
	virtual ~KappaDriver();

	int parse();
	yy::KappaParser::symbol_type getNextToken();

	void addDeclaration(ast::Declaration d);
	string* getCurrentFileName() const;
	yy::location* getLocation();
};

} /* namespace ast */

#endif /* GRAMMAR_KAPPADRIVER_H_ */
