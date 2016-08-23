/*
 * KappaAst.cpp
 *
 *  Created on: Jan 27, 2016
 *      Author: naxo
 */

#include "KappaAst.h"
#include "../../pattern/Environment.h"

namespace ast {

KappaAst::KappaAst() : decls() {
	// TODO Auto-generated constructor stub

}

KappaAst::~KappaAst() {
	// TODO Auto-generated destructor stub
}

void KappaAst::evaluateVariables(pattern::Environment &env){
	cout << "evaluating variables" << endl;
	for(list<Declaration>::iterator it = decls.begin();it != decls.end(); it++){
		//delete &(*it);
		Variable* var = it->eval(env);
		cout << "Variable " << var->getName() << ": " ;
		//env.declareVariable(var->getName(),it->isKappa());
	}
}


void KappaAst::add(const Declaration &d){
	decls.push_back(d);
}

/*void KappaAst::add(Perturbation p){
	perts.push_back(p);
}*/

} /* namespace ast */
