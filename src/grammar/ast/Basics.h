/*
 * Basics.h
 *
 *  Created on: Dec 28, 2015
 *      Author: naxo
 */

#ifndef GRAMMAR_AST_BASICS_H_
#define GRAMMAR_AST_BASICS_H_

#include <string>
#include <list>
#include "../location.hh"
#include "../../state/Variable.h"
#include "../../pattern/Environment.h"

#define DEBUG

using namespace std;

namespace ast {

class Node {
public:
	typedef yy::location location;
	Node();
	Node(const yy::location& l);
	location loc;
};



//Agent Name and Similars
class Id : public Node {
protected:
	string id;
public:
	//Id(const string &s): Node(),id(s){};
	Id();
	Id(const Node &l,const string &s);
	Id(const location &l,const string &s);

	string getString() const;
};


/** \brief Base Class for Math Algebraic Expression
 *
 */
class Expression : public Node {
public:
	Expression();
	Expression(const location &l);
	using Node::Node;
	virtual ~Expression()=0;

	virtual state::SomeAlgExpression eval(pattern::Environment&) const;
	virtual bool isConstant();
	virtual Expression* clone() const = 0;
};



//TODO class VarValue
class VarValue : public Node{
	Id var;
	const Expression *value;
public:
	VarValue();
	VarValue(const location &l,const Id &name,const Expression *exp);
};


/** \brief Strings and algebraic expressions concatenated (by a dot=".")
 *
 */
class StringExpression : public Node {
public:
	StringExpression();
	StringExpression(const location &l,const string &s);
	StringExpression(const location &l,const Expression *e);
	StringExpression(const location &l,const Expression *e, const StringExpression* nxt);
	StringExpression(const location &l,const string &s, const StringExpression* nxt);

	~StringExpression();

protected:
	enum Type {STR,ALG} t;
	union{
		const string *str;
		const Expression  *alg;
	};
	StringExpression* next;
};

/*
class StringOrPrint {
	bool isPrint;
	union {
		std::string str;
		PrintExpression printExpr;
	};
public:
	StringOrPrint();
};*/


}



#endif /* GRAMMAR_AST_BASICS_H_ */
