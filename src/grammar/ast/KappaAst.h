/*
 * KappaAst.h
 *
 *  Created on: Jan 27, 2016
 *      Author: naxo
 */

#ifndef GRAMMAR_AST_KAPPAAST_H_
#define GRAMMAR_AST_KAPPAAST_H_

#include <list>
#include "AstStructs.h"

using namespace std;

namespace ast {

class KappaAst {
	//list<Perturbation> perts;
	list<Declaration> decls;
public:
	KappaAst();
	virtual ~KappaAst();

	void add(Declaration d);
	//void add(Perturbation p);

};

} /* namespace ast */

#endif /* GRAMMAR_AST_KAPPAAST_H_ */
