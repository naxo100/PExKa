/*
 * Perturbation.cpp
 *
 *  Created on: May 2, 2019
 *      Author: naxo100
 */

#include "Perturbation.h"
#include "../state/State.h"
#include "../state/Node.h"
#include "../pattern/mixture/Agent.h"
#include "../pattern/Environment.h"
#include "../expressions/BinaryOperation.h"
#include "../expressions/NullaryOperation.h"
#include "../util/Warning.h"


namespace simulation {

Perturbation::Perturbation(BaseExpression* cond,BaseExpression* unt,const yy::location& loc) : id(-1),
		condition(cond),until(unt),introCount(0),nextStop(-1.0),incStep(0.0),applies(0),isCopy(false){
	if(cond->getVarDeps() & BaseExpression::TIME){
		const BaseExpression *expr1,*expr2,*expr3;
		char op1,op2;
		auto ff_expr = dynamic_cast<BinaryOperation<bool,FL_TYPE,FL_TYPE>*>(cond);//opt1 [T] = x ?
		if(ff_expr){
			expr1 = ff_expr->exp1;
			expr2 = ff_expr->exp2;
			op1 = ff_expr->op;
		}
		else{
			auto fi_expr = dynamic_cast<BinaryOperation<bool,FL_TYPE,int>*>(cond);
			if(fi_expr){
				expr1 = fi_expr->exp1;
				expr2 = fi_expr->exp2;
				op1 = fi_expr->op;
			}
		}
		if(expr1 && op1 == BaseExpression::EQUAL){
			float val1 = expr2->getValue(VarVector()).valueAs<FL_TYPE>();
			auto time_expr = dynamic_cast<const NullaryOperation<FL_TYPE>*>(expr1);
			if(time_expr && (time_expr->getVarDeps() & BaseExpression::TIME) && op1 != BaseExpression::EQUAL ){
				nextStop = val1;
				return;
			}
			else{
				auto mod_expr = dynamic_cast<const BinaryOperation<FL_TYPE,FL_TYPE,FL_TYPE>*>(expr1);
				if(mod_expr && dynamic_cast<const NullaryOperation<FL_TYPE>*>(mod_expr->exp1)){
					expr3 = mod_expr->exp2;
					op2 = mod_expr->op;
				}
				else {
					auto mod_expr_int = dynamic_cast<const BinaryOperation<FL_TYPE,FL_TYPE,int>*>(expr1);
					if(mod_expr_int && dynamic_cast<const NullaryOperation<FL_TYPE>*>(mod_expr_int->exp1)){
						expr3 = mod_expr_int->exp2;
						op2 = mod_expr_int->op;
					}
				}
				if(expr3 && op2 == BaseExpression::MODULO){//option ([t] % expr3) = expr2
					auto val2 = expr3->getValue(VarVector()).valueAs<FL_TYPE>();
					if(val1 < 0)
						throw SemanticError("Comparing time with negative values will generate unexpected behavior.",loc);
					nextStop = std::signbit(val1) ? val2 : val1;
					incStep = val2;
					return;
				}
			}
		}
		ADD_WARN("Optimized time-perturbations should be expressed like [T] = x or ([T] % x) = n",loc);
	}
}

Perturbation::Perturbation(const Perturbation& p) : id(p.id),condition(p.condition),
		until(p.until),effects(p.effects),influence(p.influence),introCount(0),
		nextStop(p.nextStop),incStep(p.incStep),applies(0),isCopy(true) {}

Perturbation::~Perturbation() {
	if(!isCopy){
		delete condition;
		delete until;
		for(auto eff : effects)
			delete eff;
	}
}

void Perturbation::setId(int _id){
	id = _id;
}

int Perturbation::getId() const {
	return id;
}

bool Perturbation::test(const state::State& state) const {
	return condition->getValue(state).valueAs<bool>();
}

FL_TYPE Perturbation::timeTest(const state::State& state) const {
	static FL_TYPE min_inc = numeric_limits<FL_TYPE>::epsilon();
	if(nextStop >= 0)
		return nextStop > state.getCounter().getTime() ? 0.0 : nextStop+min_inc;
	else
		return condition->getValue(state).valueAs<bool>() ? -1.0 : 0.0;
}

void Perturbation::apply(state::State& state) const {
	cout << "Applying effects of perturbation " << id << " at time " << state.getCounter().getTime() << endl;
	for(auto eff : effects)
		eff->apply(state);
	state.positiveUpdate(influence);
	applies++;
}

bool Perturbation::testAbort(const state::State& state,bool just_applied){
	bool abort = false;
	if(until){
		abort = until->getValue(state).valueAs<bool>();
	}
	else{
		abort = just_applied;
	}
	while(incStep && nextStop <= state.getCounter().getTime())
		nextStop += incStep;
	if(abort && !applies)
		ADD_WARN_NOLOC("Perturbation "+toString(state)+" was aborted at time "+to_string(state.getCounter().getTime())+" and never applied.");
	return abort;
}

void Perturbation::addEffect(Effect* eff,const Environment &env) {
	effects.push_back(eff);
	introCount += eff->addInfluences(introCount,influence,env);
}

float Perturbation::nextStopTime() const {
	return nextStop;
}

string Perturbation::toString(const state::State& state) const {
	string ret;
	if(until)
		ret += "While "+ until->toString()+" ";
	ret += "Apply effects: -";
	ret += " whenever "+condition->toString();
	return ret;
}

Perturbation::Effect::~Effect(){}

int Perturbation::Effect::addInfluences(int current,Rule::CandidateMap& map,const Environment& env) const {
	return 0;
}

/**************** Effects *******************/
/****** Intro****************************/
Intro::Intro(const BaseExpression* n, const Mixture* mix ) : n(n),mix(mix) {}

Intro::~Intro(){
	delete n;
	delete mix;
}

void Intro::apply(state::State& state) const {
	auto nn = n->getValue(state).valueAs<int>();
	if(nn < 0)
		throw invalid_argument("A perturbation is trying to add a negative number of Agents.");
	state.addNodes(nn, *mix);//no neg update
}


int Intro::addInfluences(int current,Rule::CandidateMap& map,const Environment &env) const {
	expressions::AuxMap aux_map;
	for(unsigned i = 0; i < mix->size(); i++){
		auto& new_ag = mix->getAgent(i);
		for(auto& ag : env.getAgentPatterns(new_ag.getId())){
			bool isEmb = true;
			for(auto& id_site : ag){
				Pattern::Site new_site;
				try{
					new_site = new_ag.getSite(id_site.first);
				}
				catch(out_of_range &ex){
					new_site = Pattern::Site(env.getSignature(new_ag.getId()).getSite(id_site.first).getDefaultValue());
				}
				if(!id_site.second.isEmptySite() && id_site.second.values[1] == nullptr)
					if(!new_site.values[1] && new_site.label != id_site.second.label)
						isEmb=false;

				switch(id_site.second.link_type){
				case Pattern::FREE:
					if(new_site.link_type != Pattern::FREE)
						isEmb=false;
					break;
				case Pattern::BIND:
					if(new_site.lnk_ptrn != id_site.second.lnk_ptrn)//TODO maybe a complete check
						isEmb = false;
					break;
				case Pattern::BIND_TO:
					if(new_site.link_type != Pattern::BIND)
						isEmb=false;
					break;
				default://WILD,PATH
					break;
				}
				if(!isEmb)
					break;
			}
			if(isEmb)
				Rule::addAgentIncludes(map,ag,ag_st_id(-1,current+i),999);
		}
		//i++;
	}
	return mix->size();
}

/****** Delete *************************/
Delete::Delete(const BaseExpression* n, const Mixture& mix ) : n(n),mix(mix) {
	if(mix.compsCount() != 1)
		throw invalid_argument("Perturbations can only delete unary agent patterns.");
}

Delete::~Delete(){
	delete n;
}

void Delete::apply(state::State& state) const {
	auto& inj_cont = state.getInjContainer(mix.getId());
	auto distr = uniform_int_distribution<unsigned>(inj_cont.count());
	for(int i = 0; i < n->getValue(state).valueAs<int>(); i++){
		int j = distr(state.getRandomGenerator());
		auto& inj = inj_cont.choose(j);
		for(auto node : inj.getEmbedding()){
			state.del(node);
		}
	}
}

/***** Update **************************/


Update::Update(const state::Variable& _var,expressions::BaseExpression* expr) : var(nullptr),byValue(false){
	var = state::Variable::makeAlgVar(_var.getId(), _var.toString(), expr);
}
Update::~Update(){
	delete var;
}

void Update::apply(state::State &state) const {
	//cout << "updating var " << var->toString() << " to value " << var->getValue(state) << endl;
	state.updateVar(*var,byValue);
}

void Update::setValueUpdate() {
	byValue= true;
}


} /* namespace simulation */
