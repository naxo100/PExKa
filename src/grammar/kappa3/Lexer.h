
#ifndef KAPPA3_LEXER_H
#define KAPPA3_LEXER_H


/**
 * Generated Flex class name is yyFlexLexer by default. If we want to use more flex-generated
 * classes we should name them differently. See scanner.l prefix option.
 *
 * Unfortunately the implementation relies on this trick with redefining class name
 * with a preprocessor macro. See GNU Flex manual, "Generating C++ Scanners" section
 */
#if ! defined(yyFlexLexerOnce)
#undef yyFlexLexer
#define yyFlexLexer kappa3_FlexLexer // the trick with prefix; no namespace here :(
#include <FlexLexer.h>
#endif



#include "Parser.hpp" // this is needed for symbol_type

namespace grammar {
class KappaDriver;
}

namespace kappa3 {

class Lexer : public yyFlexLexer {

public:
	Lexer(grammar::KappaDriver &driver_arg):driver(driver_arg) {};
	//KappaLexer() {};
	virtual ~Lexer() {}
	virtual kappa3::Parser::symbol_type get_next_token();

private:
	grammar::KappaDriver &driver;
};

}

#endif
