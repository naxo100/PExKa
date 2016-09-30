/*
 * Spatial.cpp
 *
 *  Created on: May 12, 2016
 *      Author: naxo
 */

#include "Spatial.h"

namespace ast {

/****** Class CompExpression *********/
CompExpression::CompExpression(){}
CompExpression::CompExpression(const location &l,const Id &id,const list<const Expression*> &dim):
	Node(l),name(id), index(dim) {};

list<int> CompExpression::evalDimensions(pattern::Environment &env,
		const unordered_map<string,state::Variable*> &vars){
	list<int> ret;
	for(list<const Expression*>::iterator it = index.begin();
			it != index.end(); it++){
		try{
			int n;
			((*it)->eval(env,vars))->value(n);
			ret.push_back(n);
		}catch(exception &e){
			//TODO
		}
	}
	return ret;
}




/****** Class Compartment ***********/





/****** Class Channel ***************/
void Channel::eval(pattern::Environment &env,
		const unordered_map<string,state::Variable*> &vars){
	unordered_map<string,int*> aux_pvalues;
	list<state::BaseExpression*> src_index;
	src_index = source.evalExpression(env,vars,&aux_pvalues);
}








unsigned short Use::count = 0;

} /* namespace ast */
