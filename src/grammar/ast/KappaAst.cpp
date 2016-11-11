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

void KappaAst::evaluateSignatures(pattern::Environment &env){

	for(list<Agent>::iterator it = signatures.begin();it != signatures.end(); it++){
		pattern::Signature* sig = it->eval(env);
	}
}

pair< vector<KappaVar>, vector<Variable> >* KappaAst::evaluateVariables(pattern::Environment &env){
	cout << "evaluating variables" << endl;
	Expression::VAR vars;
	for(list<Declaration>::iterator it = decls.begin();it != decls.end(); it++){
		//delete &(*it);
		Variable* var = it->eval(env,vars);
		cout << "Variable " << var->getName() << ": " ;
		//env.declareVariable(var->getName(),it->isKappa());
	}
	return nullptr;
}


void KappaAst::add(const Declaration &d){
	decls.push_back(d);
}
void KappaAst::add(const Agent &a){
	signatures.push_back(a);
}

/*void KappaAst::add(Perturbation p){
	perts.push_back(p);
}*/

} /* namespace ast */
