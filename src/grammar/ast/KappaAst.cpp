/*
 * KappaAst.cpp
 *
 *  Created on: Jan 27, 2016
 *      Author: naxo
 */

#include "KappaAst.h"
#include "../../pattern/Environment.h"

namespace ast {

KappaAst::KappaAst()  {
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

void KappaAst::evaluateCompartments(pattern::Environment &env,const vector<Variable*> &vars){
	for(list<Compartment>::iterator it = compartments.begin();it != compartments.end(); it++){
		it->eval(env,vars);
	}
}
void KappaAst::evaluateChannels(pattern::Environment &env,const vector<Variable*> &vars){
	for(list<Channel>::iterator it = channels.begin();it != channels.end(); it++){
		it->eval(env,vars);
	}
}

vector<Variable*> KappaAst::evaluateDeclarations(pattern::Environment &env,vector<Variable*> &var_vector, bool is_const){
	auto vars = is_const ? constants : variables;
	for(list<Declaration>::iterator it = vars.begin();it != vars.end(); it++){
		//delete &(*it);
		Variable* var;
		if(is_const)
			var = it->evalConst(env,var_vector);
		else
			var = it->evalVar(env,var_vector);
		var_vector.push_back(var);
		//env.declareVariable(var->getName(),it->isKappa());
	}
	return var_vector;
}

void KappaAst::add(const Declaration &d){
	if(d.isConstant())
		constants.push_back(d);
	else
		variables.push_back(d);
}
void KappaAst::add(const Agent &a){
	signatures.push_back(a);
}
void KappaAst::add(const Compartment &c){
	compartments.push_back(c);
}
void KappaAst::add(const Channel &c){
	channels.push_back(c);
}

/*void KappaAst::add(Perturbation p){
	perts.push_back(p);
}*/

void KappaAst::show(){
	cout << endl << "Showing variables:" << endl;
	for(list<Declaration>::iterator it = variables.begin();it != variables.end(); it++){
		it->show();
	}

	cout << endl << "Showing constants:" << endl;
	for(list<Declaration>::iterator it = constants.begin();it != constants.end(); it++){
		it->show();
	}

	cout << endl << "Showing agents:" << endl;
	for(list<Agent>::iterator it = signatures.begin();it != signatures.end(); it++){
		it->show();
	}

	cout << endl;
}

} /* namespace ast */
