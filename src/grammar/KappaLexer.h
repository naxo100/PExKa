
#ifndef KAPPALEXER_H
#define KAPPALEXER_H


/**
 * Generated Flex class name is yyFlexLexer by default. If we want to use more flex-generated
 * classes we should name them differently. See scanner.l prefix option.
 *
 * Unfortunately the implementation relies on this trick with redefining class name
 * with a preprocessor macro. See GNU Flex manual, "Generating C++ Scanners" section
 */
#if ! defined(yyFlexLexerOnce)
#undef yyFlexLexer
#define yyFlexLexer kappa_FlexLexer // the trick with prefix; no namespace here :(
#include <FlexLexer.h>
#endif

// Scanner method signature is defined by this macro. Original yylex() returns int.
// Since Bison 3 uses symbol_type, we must change returned type. We also rename it
// to something sane, since you cannot overload return type.
#undef YY_DECL
#define YY_DECL yy::KappaParser::symbol_type yy::KappaLexer::get_next_token()

#include "KappaParser.hpp" // this is needed for symbol_type

namespace grammar {
class KappaDriver;
}

namespace yy {

class KappaLexer : public yyFlexLexer {
public:
	KappaLexer(grammar::KappaDriver &driver_arg):driver(driver_arg) {};
	//KappaLexer() {};
	virtual ~KappaLexer() {}
	virtual yy::KappaParser::symbol_type get_next_token();

private:
	grammar::KappaDriver &driver;
};

}

#endif
