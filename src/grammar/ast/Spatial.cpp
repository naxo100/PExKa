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




/****** Class Compartment ***********/





/****** Class Channel ***************/
pattern::Channel* Channel::eval(pattern::Environment &env,
		const unordered_map<string,state::Variable*> &vars){

	short id = env.declareChannel(name.getString());
	short src_id,trgt_id;
	src_id = source.evalName(env);
	trgt_id = target.evalName(env);
	map<string,int*> aux_pvalues;
	list<state::BaseExpression*> src_index,trgt_index;
	src_index = source.evalExpression(env,vars,&aux_pvalues);
	trgt_index = target.evalExpression(env,vars,&aux_pvalues);

	pattern::CompartmentExpr c_exp1 =
			new pattern::CompartmentExpr(env.getChannel(src_id),src_index);
	pattern::Channel *channel =
			new pattern::Channel(name.getString(),filter->eval(env,vars),
					env.getChannel(src_id),env.getChannel(trgt_id),
					src_index,trgt_index
				);
	channel->setVarValues(aux_pvalues);
	return channel;
}








unsigned short Use::count = 0;

} /* namespace ast */
