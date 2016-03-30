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

namespace grammar {

class KappaDriver {
	vector<string> files;
	vector<string>::iterator fit;
	yy::KappaParser parser;
	yy::KappaLexer lexer;
	yy::location loc;
	ast::KappaAst syntax_tree;

public:
	/** \brief Construct a KappaDriver object with a vector of file names.
	 *
	 * Construct a KappaDriver object with a vector of file names. parse()
	 * function will open and parse each file and save the kappa AST internally.
	 */
	KappaDriver(const vector<string> &files);

	/** \brief Construct an empty KappaDriver object.
	 *
	 * Construct an empty KappaDriver object that will read a kappa model from
	 * stdin and save the kappa AST internally.
	 */
	KappaDriver();

	/** \brief Destroy this object.
	 *
	 */
	virtual ~KappaDriver();

	/** \brief Parse a kappa model.
	 *
	 * Parse a kappa model from files or stdin. Generate a kappa AST internally.
	 */
	int parse();

	yy::KappaParser::symbol_type getNextToken();

	string* getCurrentFileName() const;

	yy::location* getLocation();

	/** \brief Return the kappa AST generated by parse().
	 *
	 */
	ast::KappaAst getAst();
};

} /* namespace ast */

#endif /* GRAMMAR_KAPPADRIVER_H_ */
