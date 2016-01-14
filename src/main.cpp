/*
 * main.cpp
 *
 *  Created on: Dec 16, 2015
 *      Author: naxo
 */

#include "grammar/KappaLexer.h"
#include "grammar/KappaParser.hpp"
#include "grammar/ast/KappaAst.h"


//using namespace std;
int main(int argc, char* argv[]){
	ast::KappaAst ast_model;
	ast_model.parse();
	return 0;
}

