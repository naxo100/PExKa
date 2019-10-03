/*
 * Perturbation.cpp
 *
 *  Created on: May 2, 2019
 *      Author: naxo100
 */

#include "Perturbation.h"
#include "Parameters.h"
#include "Simulation.h"
#include "../state/State.h"
#include "../state/Node.h"
#include "../pattern/mixture/Agent.h"
#include "../pattern/mixture/Component.h"
#include "../pattern/Environment.h"
#include "../expressions/BinaryOperation.h"
#include "../expressions/NullaryOperation.h"
#include "../expressions/Vars.h"
#include "../util/Warning.h"

#include <limits>
#include <float.h>


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
#ifdef DEBUG
	cout << "Applying effects of perturbation " << id << " at time " << state.getCounter().getTime() << endl;
#endif
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
	//expressions::AuxMap aux_map;
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
	auto total = inj_cont.count();
	auto some_del = n->getValue(state);
	size_t del;
	if(some_del.t == Type::FLOAT){
		if(some_del.fVal == numeric_limits<FL_TYPE>::infinity())
			del = numeric_limits<size_t>::max();
		else{
			del = round(some_del.fVal);
			ADD_WARN_NOLOC("Making approximation of a float value in agent initialization to "+to_string(del));
		}
	}
	else
		del = some_del.valueAs<int>();

	if(some_del.valueAs<FL_TYPE>() < 0)
		throw invalid_argument("A perturbation is trying to delete a negative number ("+
			to_string(del)+") of instances of "+mix.getComponent(0).toString(state.getEnv()));
	if(total <= del){
		inj_cont.clear();
		if(total < del && del != numeric_limits<size_t>::max())
			ADD_WARN_NOLOC("Trying to delete "+to_string(del)+" instances of "+
				mix.getComponent(0).toString(state.getEnv())+" but there are "+
				to_string(total)+" available. Deleting all.");
		return;
	}

	auto distr = uniform_int_distribution<unsigned>();
	for(int i = 0; i < del; i++){
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


/***** Histogram **************************/

Histogram::Histogram(const Environment& env,int _bins,string file_name,Mixture& _mix,BaseExpression* f) :
		bins(_bins+2),points(_bins+1),min(-numeric_limits<float>::infinity()),max(-min),filetype("tsv"),func(f),
		mix(_mix),newLim(true),fixedLim(false){
	if(mix.compsCount() != 1)
		throw invalid_argument("Mixture must contain exactly one Connected Component.");
	auto& cc = mix.getComponent(0);
	auto& auxs = mix.getAux();
	if(auxs.size() < 1)
		throw invalid_argument("Mixture must have at least one Auxiliar variable.");
	if(!func){
		if(auxs.size() > 1)
			throw invalid_argument("Mixture must have exactly one Auxiliar variable if you don't declare a value function for histogram.");
		auto& aux_ccagst = *(auxs.begin());
		func = new expressions::Auxiliar<FL_TYPE>(aux_ccagst.first);
	}
	if(dynamic_cast<expressions::Auxiliar<FL_TYPE>*>(func)){
		auto& ccagst = auxs.at(func->toString());
		auto lims = env.getSignature(cc.getAgent(get<1>(ccagst)).getId()).getSite(get<2>(ccagst)).getLimits();
		min = lims.first.valueAs<FL_TYPE>();
		max = lims.second.valueAs<FL_TYPE>();
	}
	auto pos = file_name.find('.');
	if( pos == string::npos )
		filename = file_name;
	else{
		filename = file_name.substr(0,pos);
		filetype = file_name.substr(pos+1);
	}

	if(min != max && !isinf(max) && !isinf(min)){
		fixedLim = true;
		newLim = true;
		setPoints();
	}
}

Histogram::~Histogram(){
	//TODO
	//file.close();
}


void Histogram::apply(state::State& state) const {
	auto& cc = mix.getComponent(0);
	auto& aux_map = mix.getAux();

	char file_name[100];
	if(Parameters::get().runs > 1)
		sprintf(file_name,("%s-%0"+to_string(int(log10(Parameters::get().runs-1)+1))+"d.%s").c_str(),
				filename.c_str(),state.getSim().getId(),filetype.c_str());
	else
		sprintf(file_name,"%s.%s",filename.c_str(),filetype.c_str());
	ofstream file(file_name,ios_base::app);

	if(min == max || isinf(min) || isinf(max)){
		list<FL_TYPE> values;
		max = -numeric_limits<float>::infinity();
		min = -max;
		function<void (const matching::Injection*)> f = [&](const matching::Injection* inj) -> void {
			AuxNames aux_values;
			for(auto& aux_coords : aux_map)
				aux_values[aux_coords.first] = inj->getEmbedding()[get<1>(aux_coords.second)]->
					getInternalState(get<2>(aux_coords.second)).valueAs<FL_TYPE>();
			auto val = func->getValue(state, move(aux_values)).valueAs<FL_TYPE>();
			if(val < min)
				min = val;
			else if(val > max)
				max = val;
			values.push_back(val);
		};
		state.getInjContainer(cc.getId()).fold(f);
		bins.assign(bins.size(),0);
		setPoints();
		printHeader(file);
		newLim = false;
		if(min != max)
			for(auto v : values)
				tag(v);
	}
	else{
		if(newLim){
			printHeader(file);
			setPoints();
			newLim = false;
		}
		function<void (const matching::Injection*)> f = [&](const matching::Injection* inj) -> void {
			AuxNames aux_values;
			for(auto& aux_coords : aux_map)
				aux_values[aux_coords.first] = inj->getEmbedding()[get<1>(aux_coords.second)]->
					getInternalState(get<2>(aux_coords.second)).valueAs<FL_TYPE>();
			auto val = func->getValue(state, move(aux_values)).valueAs<FL_TYPE>();
			if(!fixedLim){
				if(val < min){
					min = val;newLim = true;
				}
				else if(val > max){
					max = val;newLim = true;
				}
			}
			tag(val);
		};
		bins.assign(bins.size(),0);
		state.getInjContainer(cc.getId()).fold(f);
	}

	file << state.getCounter().getTime();
	if(min == max)
		file << "\t" << state.getInjContainer(cc.getId()).count();
	else
		for(auto bin : bins)
			file << "\t" << bin ;
	file << "\n";

	file.close();
}

void Histogram::setPoints() const {
	auto dif = (max - min) / (bins.size()-2);
	for(unsigned i = 0; i < points.size(); i++){
		points[i] = min + i*dif;
	}
}

void Histogram::tag(FL_TYPE val) const {
	if(val < points[0]){
		if(fixedLim)
			ADD_WARN_NOLOC("The value for a site in an agent instance is bellow the minimum allowed");
		bins[0]++;
		return;
	}
	for(unsigned i = 1; i < points.size()-1; i++)
		if(val < points[i]){
			bins[i]++;return;
		}
	if(val <= points.back())
		bins[points.size()-1]++;
	else{
		if(fixedLim)
			ADD_WARN_NOLOC("The value for a site in an agent instance is over the maximum allowed");
		bins[points.size()]++;
	}
}

void Histogram::printHeader(ofstream& file) const {
	char s1[100];

	if(min == max){
		sprintf(s1,"time\t%.5g",min);
		file << "time\t " << min << "\n";
	}
	else{
		auto dif = (max - min) / (bins.size()-2);
		sprintf(s1,"time\tx < %.5g",min);
		file << s1;
		for(unsigned i = 0; i < bins.size()-2; i++){
			if(i)
				file << ")";
			sprintf(s1,"\t[%.5g,%.5g",min+dif*i,min+dif*(i+1));
			file << s1;
		}
		sprintf(s1,"]\t%.5g < x\n",max);
		file << s1 ;
	}
	newLim = false;
}



} /* namespace simulation */
