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
	Node(l),name(id), indexList(dim) {};

pattern::CompartmentExpr* CompExpression::eval
		(pattern::Environment& env,
		const unordered_map<string,state::Variable*> &vars,bool isDeclaration){
	//short id =
	evalName(env,isDeclaration);

	return nullptr;
}


vector<short> CompExpression::evalDimensions(pattern::Environment &env,
		const vector<Variable*> &vars){
	vector<short> ret;
	for(list<const Expression*>::iterator it = indexList.begin();
			it != indexList.end(); it++){
		try{
			ret.push_back(((*it)->eval(env,vars))->getValue().iVal);
		}catch(exception &e){

			cout << "fail" << endl;
		}
	}
	return ret;
}

string CompExpression::evalName(pattern::Environment &env,bool declare){
	if(!declare)
		env.getCompartmentId(name.getString());
	return name.getString();
}
list<state::BaseExpression*> CompExpression::evalExpression(pattern::Environment &env,
			const vector<Variable*> &vars){
	list<state::BaseExpression*> ret;
	for(auto index : indexList){
		ret.push_back(index->eval(env,vars,char(Expression::AUX)));
	}
	return ret;
}




/****** Class Compartment ***********/

Compartment::Compartment(const location& l,const CompExpression& comp_exp,
		Expression* exp) : Node(l), comp(comp_exp), volume(exp) {}

//TODO
short Compartment::eval(pattern::Environment &env,const vector<Variable*> &vars){
	short id;
	string name = comp.evalName(env,true);
	vector<short> dims = comp.evalDimensions(env,vars);
	state::BaseExpression* vol = volume->eval(env,vars,Expression::CONST);
	pattern::Compartment c(name,dims,vol);
	id = env.declareCompartment(c);
	return id;
}


/****** Class Channel ***************/

Channel::Channel(const location& l,const Id& nme, const CompExpression& src,
			const CompExpression& trgt, bool bckwrds, const Expression* where,
			const Expression* wait) :
		Node(l),name(nme),source(src),target(trgt),bidirectional(bckwrds),
		filter(where),delay(wait) {}

//TODO
pattern::Channel* Channel::eval(pattern::Environment &env,
		const vector<Variable*> &vars){

	short src_id,trgt_id;
	src_id = env.getCompartmentId(source.evalName(env,false));
	trgt_id = env.getCompartmentId(target.evalName(env,false));

	list<state::BaseExpression*> src_index,trgt_index;
	src_index = source.evalExpression(env,vars);
	trgt_index = target.evalExpression(env,vars);

	pattern::CompartmentExpr* c_exp_src =
			new pattern::CompartmentExpr(env.getCompartment(src_id),src_index);
	pattern::CompartmentExpr* c_exp_trgt =
			new pattern::CompartmentExpr(env.getCompartment(trgt_id),trgt_index);

	c_exp_src->setEquation();
	if(bidirectional)
		c_exp_trgt->setEquation();
	pattern::Channel *channel =
			new pattern::Channel(name.getString(),c_exp_src,c_exp_trgt);
	if(filter)
		channel->setFilter(filter->eval(env,vars));
	env.declareChannel(*channel);
	return channel;
}







unsigned short Use::count = 0;

} /* namespace ast */
