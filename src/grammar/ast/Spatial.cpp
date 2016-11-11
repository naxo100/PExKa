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

pattern::CompartmentExpr* CompExpression::eval
		(pattern::Environment& env,
		const unordered_map<string,state::Variable*> &vars,bool isDeclaration){
	//short id =
	evalName(env,isDeclaration);

	return nullptr;
}


list<int> CompExpression::evalDimensions(pattern::Environment &env,
		const vector<Variable*> &vars){
	list<int> ret;
	for(list<const Expression*>::iterator it = index.begin();
			it != index.end(); it++){
		try{
			ret.push_back(((*it)->eval(env,vars))->getValue().iVal);
		}catch(exception &e){
			//TODO
		}
	}
	return ret;
}

short CompExpression::evalName(pattern::Environment &env,bool declare){
	short id;
	try{
		if(declare)
			id = env.declareChannel(name.getString());
		else
			id = env.getChannelId(name.getString());
	}
	catch(exception &e){

		throw e;
	}
	return id;
}
list<state::BaseExpression*> CompExpression::evalExpression(const pattern::Environment &env,
			const unordered_map<string,state::Variable*> &vars,
			map<string,int*> *aux_values){
	return list<state::BaseExpression*>();
}




/****** Class Compartment ***********/

Compartment::Compartment(const location& l,const CompExpression& comp_exp,
		Expression* exp) : Node(l), comp(comp_exp), volume(exp) {}

//TODO
short Compartment::eval(pattern::Environment &env,const vector<Variable*> &vars){
	//env.declareCompartment();
	return 0;
}


/****** Class Channel ***************/
//TODO
pattern::Channel* Channel::eval(pattern::Environment &env,
		const unordered_map<string,state::Variable*> &vars){

	//short id =
	env.declareChannel(name.getString());
	short src_id,trgt_id;
	src_id = source.evalName(env);
	trgt_id = target.evalName(env);
	map<string,int*> aux_pvalues;
	list<state::BaseExpression*> src_index,trgt_index;
	src_index = source.evalExpression(env,vars,&aux_pvalues);
	trgt_index = target.evalExpression(env,vars,&aux_pvalues);

	//pattern::CompartmentExpr c_exp1 =
	//		new pattern::CompartmentExpr(env.getChannel(src_id),src_index);
	pattern::Channel *channel = nullptr;
			/*new pattern::Channel(name.getString(),filter->eval(env,vars),
					env.getCompartment(src_id),env.getCompartment(trgt_id),
					src_index,trgt_index
				);*/
	channel->setVarValues(aux_pvalues);
	return channel;
}







unsigned short Use::count = 0;

} /* namespace ast */
