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

const Id& CompExpression::evalName(pattern::Environment &env,bool declare){
	if(!declare){
		try{
			env.getCompartmentId(name.getString());
		}
		catch(const std::out_of_range &ex){
			throw SemanticError("No compartment called "+name.getString(),loc);
		}
	}
	return name;
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
void Compartment::eval(pattern::Environment &env,const vector<Variable*> &vars){
	const Id& name = comp.evalName(env,true);
	pattern::Compartment& c = env.declareCompartment(name);
	vector<short> dims = comp.evalDimensions(env,vars);
	state::BaseExpression* vol = volume->eval(env,vars,Expression::CONST);
	c.setDimensions(dims);
	c.setVolume(vol);
	return;
}


/****** Class Channel ***************/

Channel::Channel(const location& l,const Id& nme, const CompExpression& src,
			const CompExpression& trgt, bool bckwrds, const Expression* where,
			const Expression* wait) :
		Node(l),name(nme),source(src),target(trgt),bidirectional(bckwrds),
		filter(where),delay(wait) {}


void Channel::eval(pattern::Environment &env,
		const vector<Variable*> &vars){
	short src_id,trgt_id;
	pattern::Channel& channel = env.declareChannel(name);

	src_id = env.getCompartmentId(source.evalName(env,false).getString());
	trgt_id = env.getCompartmentId(target.evalName(env,false).getString());

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
	channel.setCompExpressions(c_exp_src,c_exp_trgt);
	if(filter)
		channel.setFilter(filter->eval(env,vars));
	return;
}







unsigned short Use::count = 0;

} /* namespace ast */
