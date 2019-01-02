/*
 * Dynamics.cpp
 *
 *  Created on: May 12, 2016
 *      Author: naxo
 */

#include "Dynamics.h"
#include "../../util/Warning.h"
#include "../../expressions/BinaryOperation.h"

namespace ast {


/****** Class Link ***********/
Link::Link() : type(FREE),value(0){}
Link::Link(const location &l,LinkType t):
	Node(l), type(t), value()  {};
Link::Link(const location &l,LinkType t,int val):
	Node(l), type(t), value(val) {};
Link::Link(const location &l,LinkType t,const Id &ag,const Id &s):
	Node(l), type(t), ag_site{ag,s}, value() {	};
Link::Link(const Link &l):Node(l){
	type = l.type;
	switch(type){
	case VALUE:
		value = l.value;
		break;
	case AG_SITE:
		ag_site = l.ag_site;
		break;
	default:
		break;
	}
};
Link& Link::operator=(const Link &l){
	loc = l.loc;
	type = l.type;
	switch(type){
	case VALUE:
		value = l.value;
		break;
	case AG_SITE:
		ag_site = l.ag_site;
		break;
	default:
		break;
	}
	return *this;
};
Link::~Link(){};
const Link::LinkType& Link::getType() const{
	return type;
}

void Link::eval(const pattern::Environment &env,
		pattern::Mixture::Site& mix_site,
		unordered_map<unsigned,list<pair<short,short> > > &links,
		const pair<short,short> &mix_ag_site,bool allow_pattern) const {
	int n;
	switch(type){
	case FREE:
		mix_site.link_type = pattern::Mixture::FREE;
		break;
	case VALUE:
		mix_site.link_type = pattern::Mixture::BIND;
		n = links[value].size();
		if(n > 1)
			throw SemanticError("Edge identifier "+to_string(value)+
					" used to many times.",loc);
		else
			links[value].push_back(mix_ag_site);
		break;
	case ANY:
		if(!allow_pattern)
			throw SemanticError("Patterns are not allowed here.",loc);
		mix_site.link_type = pattern::Mixture::WILD;
		break;
	case SOME:
		if(!allow_pattern)
			throw SemanticError("Patterns are not allowed here.",loc);
		mix_site.link_type = pattern::Mixture::BIND;
		break;
	case AG_SITE:
		if(!allow_pattern)
			throw SemanticError("Patterns are not allowed here.",loc);
		mix_site.link_type = pattern::Mixture::BIND_TO;
		try{
			short bind_to_ag(env.getSignatureId(ag_site.first.getString()));
			short bind_to_site(env.getSignature(bind_to_ag).getSiteId(ag_site.second.getString()));
			mix_site.lnk_ptrn.first = bind_to_ag;
			mix_site.lnk_ptrn.second = bind_to_site;
		}catch(std::out_of_range &e){
			throw SemanticError("Link pattern '"+ag_site.second.getString()+
					"."+ag_site.first.getString()+"' is not defined.",
					ag_site.second.loc+ag_site.first.loc);
		}
		break;
	}
}


/****** Class SiteState ***********/

SiteState::SiteState() :Node(),type(EMPTY),val(nullptr),flag(0) {}
SiteState::~SiteState(){};
SiteState::SiteState(const location& loc, const list<Id> &labs) :
		Node(loc),type(labs.size()>0?LABEL:EMPTY),labels(labs),
		  val(nullptr),flag(0){}

SiteState::SiteState(const location& loc, const Expression* min,
		const Expression* max,const Expression* def) : Node(loc),
				type(RANGE),val(nullptr),range{min,def,max},flag(0){
	if(!def)
		range[1] = min;
}
SiteState::SiteState(const location& loc, const Expression* value) :
		Node(loc),type(EXPR),val(value),flag(0){}
SiteState::SiteState(const location& loc, const Id &_aux,
		const Expression* equal) : Node(loc),
		type(AUX),aux(_aux),val(equal),flag(0){}
SiteState::SiteState(const location& loc, const Id &_aux,char _flag,
		const Expression* min,const Expression* max) : Node(loc),
		type(AUX),aux(_aux),val(nullptr),range{min,nullptr,max},flag(_flag){}

void SiteState::evalLabels(pattern::Signature::LabelSite& site) const{
	for(const auto& lab : labels)
		site.addLabel(lab);
	return;
}

bool SiteState::evalRange(pattern::Environment &env,const vector<state::Variable*> &consts,
		expressions::BaseExpression** expr_values) const{
	//using ast::Expression::VAR;
	//using ast::Expression::FLAGS;
	expr_values[0] = range[0]->eval(env,consts,nullptr,
			Expression::FORCE | Expression::CONST);
	expr_values[2] = range[2]->eval(env,consts,nullptr,
			Expression::FORCE | Expression::CONST);
	if(range[1] != nullptr)
		expr_values[1] = range[1]->eval(env,consts,nullptr,
					Expression::FORCE | Expression::CONST);
	else
		expr_values[1] = expr_values[0];
	//Test
	bool isInt = true;
	for(int i = 0; i < 3; i++)
		switch(expr_values[i]->getType()){
		case expressions::BOOL:
			throw SemanticError("Not a valid value to define a range.",range[i]->loc);
		case expressions::FLOAT:
			isInt = false;
			break;
		default:break;
		}
	return isInt;
}
void SiteState::show( string tabs ) const {
	cout << " ";
	for(list<Id>::const_iterator it = labels.cbegin(); it != labels.cend(); it++) {
		if( it == labels.cbegin() ) cout << "estados: ";

		if( it != labels.cend() && it != labels.cbegin() ) cout << ", ";

		it->show();
	}

	// que guardan los range's?
	//range[0]->show();
	//range[1]->show();
	//range[2]->show();
}


/****** Class Site ***********/
Site::Site(){}
Site::Site(const location &l,const Id &id,const SiteState &s,const Link &lnk):
	Node(l), name(id), stateInfo(s), link(lnk) {};

void Site::eval(pattern::Environment &env,const vector<state::Variable*> &consts,
		pattern::Signature &sign) const{
	if(link.getType())
		throw SemanticError("Link status in a definition of signature.",link.loc);
	//short id;
	pattern::Signature::Site* site;
	switch(stateInfo.type){
	case SiteState::EMPTY:
		sign.addSite<pattern::Signature::EmptySite>(name);
		break;
	case SiteState::LABEL:
		site = &sign.addSite<pattern::Signature::LabelSite>(name);
		stateInfo.evalLabels(*static_cast<pattern::Signature::LabelSite*>(site));
		break;
	case SiteState::RANGE:
		try{
			BaseExpression* range[3];
			if(!stateInfo.evalRange(env,consts,range)){
				site = &sign.addSite<pattern::Signature::RangeSite<FL_TYPE> >(name);
				static_cast<pattern::Signature::RangeSite<FL_TYPE>*>
					(site)->setBoundaries(range[0]->getValue().valueAs<FL_TYPE>(),
						range[2]->getValue().valueAs<FL_TYPE>(),range[1]->getValue().valueAs<FL_TYPE>());
			}
			else{
				site = &sign.addSite<pattern::Signature::RangeSite<int> >(name);
				static_cast<pattern::Signature::RangeSite<int>*>
					(site)->setBoundaries(range[0]->getValue().valueAs<int>(),
						range[2]->getValue().valueAs<int>(),range[1]->getValue().valueAs<int>());
			}
		}
		catch(std::out_of_range &e){
			throw SemanticError(e.what(),stateInfo.loc);
		}
		break;
	default:
		throw SemanticError("Not a valid value for a site in agent's signature.",loc);
	}
}

pair<small_id,Id> Site::eval(const pattern::Environment &env,const vector<Variable*> &consts,
		pair<short,pattern::Mixture::Agent&> id_agent,
		unordered_map<unsigned,list<two<short> > > &links,char ptrn_flag) const{
	const pattern::Signature* sign;
	small_id site_id;
	short ag_id = id_agent.first;
	pattern::Mixture::Agent& agent = id_agent.second;
	pattern::Mixture::Site* mix_site;
	sign = &env.getSignature(agent.getId());
	try{
		site_id = sign->getSiteId(name.getString());
		mix_site = &agent.addSite(site_id);
	}
	catch(std::out_of_range &ex){
		throw SemanticError("Site "+name.getString()
				+" is not declared in agent signature of "+
				sign->getName()+"().",loc);
	}
	//const state::SomeValue *v;
	state::BaseExpression* num;
	switch(stateInfo.type){
	case SiteState::EMPTY:
		//agent.setSiteValue(site_id,(small_id)-1);// default value
		break;
	case SiteState::LABEL:
		if(stateInfo.labels.size() > 1)
			throw SemanticError("List of labels are only allowed in agent declaration.",loc);
		/*pattern::Signature::Value v;
		v.type = pattern::Signature::Value::STRING;
		v.s = &stateInfo.labels.front().getString();*/
		if(stateInfo.labels.size() == 1){
			//v = new state::SomeValue(stateInfo.labels.front().getString());
			small_id lbl_id;
			try{
				lbl_id = dynamic_cast<const pattern::Signature::LabelSite&>(sign->getSite(site_id))
						.getLabelId(stateInfo.labels.front().getString());
				agent.setSiteValue(site_id,lbl_id);
			}
			catch(std::out_of_range &e){
				throw SemanticError("Label "+stateInfo.labels.front().getString()+
						" is not defined for site "+name.getString()+
						" of agent "+sign->getName()+"().",loc);
			}
			catch(std::bad_cast &e){
				try{
					dynamic_cast<const pattern::Signature::RangeSite<int>&>(sign->getSite(site_id));
					auto& lbl = stateInfo.labels.front().getString();
					int n = atoi(lbl.c_str());
					if(n == 0){
						if(lbl[0] != '0' || lbl.size() != 1)
							throw SemanticError("Can not convert "+lbl+" into a numeric value",loc);
					}
					else if(lbl.length() > log10(n)+1)
						throw SemanticError("Can not convert "+lbl+" into a numeric value",loc);
					agent.setSiteValue(site_id,n);
				}
				catch(std::bad_cast &e){
					throw SemanticError("Site "+name.getString()+" is not defined as a labeled site.",loc);
				}
			}
		}
		else{//empty list ? this should have been an SiteState::EMPTY
			throw invalid_argument("empty list? this should have been an SiteState::EMPTY");
		}
		break;
	case SiteState::RANGE:
		throw SemanticError("Ranges for sites are only allowed in agent declaration.",loc);
		break;
	case SiteState::AUX:
		try{
			dynamic_cast<const pattern::Signature::RangeSite<int>&>(sign->getSite(site_id));
			if(stateInfo.range[0]){
				if(stateInfo.flag & SiteState::MIN_EQUAL)
					agent.setSiteMinExpr(site_id,stateInfo.range[0]->
							eval(env, consts, nullptr, ptrn_flag));
				else
					agent.setSiteMinExpr(site_id,BaseExpression::makeBinaryExpression<false>(
							stateInfo.range[0]->eval(env, consts, nullptr, ptrn_flag),
							new Constant<int>(1),BaseExpression::SUM));
			}
			if(stateInfo.range[2]){
				if(stateInfo.flag & SiteState::MAX_EQUAL)
					agent.setSiteMaxExpr(site_id,stateInfo.range[2]->
							eval(env, consts, nullptr, ptrn_flag));
				else
					agent.setSiteMaxExpr(site_id,BaseExpression::makeBinaryExpression<false>(
							stateInfo.range[2]->eval(env, consts, nullptr, ptrn_flag),
							new Constant<int>(-1),BaseExpression::SUM));
			}
		}catch(std::bad_cast &e){try{
			dynamic_cast<const pattern::Signature::RangeSite<FL_TYPE>&>(sign->getSite(site_id));
			if(stateInfo.range[0]){
				if(stateInfo.flag & SiteState::MIN_EQUAL)
					agent.setSiteMinExpr(site_id,stateInfo.range[0]->
							eval(env, consts, nullptr, ptrn_flag));
				else
					agent.setSiteMinExpr(site_id,BaseExpression::makeBinaryExpression<false>(
							stateInfo.range[0]->eval(env, consts, nullptr, ptrn_flag),
							new Constant<FL_TYPE>(std::numeric_limits<FL_TYPE>::epsilon()),
								BaseExpression::SUM));
			}
			if(stateInfo.range[2]){
				if(stateInfo.flag & SiteState::MAX_EQUAL)
					agent.setSiteMaxExpr(site_id,stateInfo.range[2]->
							eval(env, consts, nullptr, ptrn_flag));
				else
					agent.setSiteMaxExpr(site_id,BaseExpression::makeBinaryExpression<false>(
							stateInfo.range[2]->eval(env, consts, nullptr, ptrn_flag),
							new Constant<FL_TYPE>(-std::numeric_limits<FL_TYPE>::epsilon()),
								BaseExpression::SUM));
			}
		}catch(std::bad_cast &e){
			throw SemanticError("Only valued sites can assign auxiliar.",stateInfo.loc);
		}}//double try

		if(ptrn_flag & Mixture::Info::LHS)
			agent.setSiteAux(site_id);
		else if(stateInfo.range[0] || stateInfo.range[2])
			throw SemanticError("Can't use an inequation pattern at RHS.",stateInfo.loc);
		else {
			if(stateInfo.range[1])
				agent.setSiteExpr(site_id,stateInfo.range[1]->eval
						(env,consts,nullptr,ptrn_flag|Expression::FORCE));
			else
				agent.setSiteExpr(site_id,Var(stateInfo.aux.loc,Var::AUX,stateInfo.aux).eval
						(env, consts, nullptr, ptrn_flag));
			break;
		}
		break;
	case SiteState::EXPR:
		try{
			dynamic_cast<const pattern::Signature::RangeSite<int>&>(sign->getSite(site_id));
		}catch(std::bad_cast &e){try{
			dynamic_cast<const pattern::Signature::RangeSite<FL_TYPE>&>(sign->getSite(site_id));
		}catch(std::bad_cast &e){
			throw SemanticError("Only valued sites can assign expressions.",stateInfo.loc);
		}}
		if(ptrn_flag & Mixture::Info::RHS)
			num = stateInfo.val->eval(env,consts,nullptr,ptrn_flag);
		else //LHS
			num = stateInfo.val->eval(env,consts,nullptr,Expression::CONST | ptrn_flag);
		try{
			if(! sign->getSite(site_id).isPossibleValue(num->getValue()))
				throw SemanticError("This value is not included in range declaration of site",loc);
		}
		catch(exception &e){}//TODO check which exceptions
		agent.setSiteExpr(site_id,num);
		break;
	}

	link.eval(env,*mix_site,links,make_pair(ag_id,site_id),true);
	return make_pair(site_id,stateInfo.aux);
}

void Site::show( string tabs ) const {
	cout << tabs << "Site " << name.getString();

	stateInfo.show( tabs );
}

//SiteState::~SiteState(){}

/****** Class Agent **********/
Agent::Agent(){}
Agent::Agent(const location &l,const Id &id,const list<Site> s):
	Node(l), name(id),sites(s) {};

void Agent::eval(pattern::Environment &env,const vector<state::Variable*> &consts) const{
	//using namespace pattern;
	pattern::Signature& sign = env.declareSignature(name);
	//sign.setId(id);

	for(const auto& site : sites){
		site.eval(env,consts,sign);
	}
	return;
}

void Agent::eval(const pattern::Environment &env,const vector<state::Variable*> &consts,pattern::Mixture &mix,
		unordered_map<unsigned,list<two<short> > > &lnks,char ptrn_flag) const {
	short sign_id;
	try{
		sign_id = env.getSignatureId(name.getString());
	}
	catch(std::out_of_range &e){
		throw SemanticError("Agent "+name.getString()+"() has not been declared.",loc);
	}

	pattern::Mixture::Agent* a_buff = new pattern::Mixture::Agent(sign_id);
	pair<short,pattern::Mixture::Agent&> id_ag(mix.size(),*a_buff);
	for(auto &site : sites){
		auto aux = site.eval(env,consts,id_ag,lnks,ptrn_flag);
		if(aux.second.getString() != "" && (ptrn_flag & Mixture::Info::PATTERN))
			mix.setAux(aux.second.getString(), mix.size(), aux.first);
	}

	//auto& a = env.declareAgentPattern(a_buff);
	mix.addAgent(a_buff);
}

void Agent::show( string tabs ) const {
	tabs += "   ";
	cout << tabs << "Agent " << name.getString() << " {";

	for(list<Site>::const_iterator it = sites.cbegin(); it != sites.cend(); it++){
		cout << endl << "   ";
		it->show( tabs );
	}

	cout << endl << tabs <<  "}" << endl;
}


/****** Class Mixture ************/
Mixture::Mixture(){}

Mixture::Mixture(const location &l,const list<Agent> &m):
	Node(l), agents(m) {};

Mixture::~Mixture(){};

pattern::Mixture* Mixture::eval(const pattern::Environment &env,
		const vector<Variable*> &vars,char ptrn_flag) const{
	pattern::Mixture* mix = new pattern::Mixture(agents.size());
	unordered_map<unsigned,list<pair<short,short> > > links;
	for(list<Agent>::const_iterator it = agents.cbegin();it != agents.cend();it++){
		it->eval(env,vars,*mix,links,ptrn_flag);
	}
	for(auto &n_link : links){
		if(n_link.second.size() == 1)
			throw SemanticError("Edge identifier "+to_string(n_link.first)+
					" is not paired in mixture.",loc);
		mix->addLink(n_link.second.front(),*(++n_link.second.begin()),env);
	}
	//mix->setComponents();
	return mix;
}

void Mixture::show(string tabs) const {
	for(list<Agent>::const_iterator it = agents.cbegin() ; it != agents.cend() ; it++) {
		it->show(tabs);
	}
}


/****** Class Effect *************/
Effect::Effect():
	action(),n(nullptr),mix(nullptr),set(VarValue()) {};
//INTRO,DELETE
Effect::Effect(const location& l,const Action& a,const Expression* n ,list<Agent>& mix):
	Node(l),action(a),n(n), mix(new Mixture(l,mix)) {};
//UPDATE,UPDATE_TOK
Effect::Effect(const location& l,const Action& a,const VarValue& dec):
	Node(l),action(a),set(dec),n(nullptr),mix(nullptr){};
//CFLOW,CFLOW_OFF
Effect::Effect(const location& l,const Action& a,const Id& id):
	Node(l),action(a),name(id),n(nullptr),mix(nullptr) {};
//STOP,SNAPSHOT,FLUX,FLUXOFF,PRINT
Effect::Effect(const location& l,const Action& a,const list<StringExpression>& str):
	Node(l),action(a),string_expr(str),n(nullptr),mix(nullptr) {};
//PRINTF
Effect::Effect(const location& l,const Action& a,const list<StringExpression>& str1,const list<StringExpression>& str2):
	Node(l),action(a),filename(str1),string_expr(str2),n(nullptr),mix(nullptr) {};

Effect::Effect(const Effect &eff):
	Node(eff.loc),action(eff.action),n(nullptr),mix(nullptr) {

	n = eff.n;
	mix = eff.mix;

	switch(action){
	case INTRO:case DELETE:
		//multi_exp = eff.multi_exp;
		break;
	case UPDATE:case UPDATE_TOK:
		set = eff.set;
		break;
	case CFLOW:case CFLOW_OFF:
		name = eff.name;
		break;
	case STOP:case SNAPSHOT:case FLUX:case FLUX_OFF:case PRINT:
		string_expr = eff.string_expr;
		break;
	case PRINTF:
		string_expr = eff.string_expr;
		filename = eff.filename;
		break;
	}
}

Effect& Effect::operator=(const Effect& eff){
	loc = eff.loc;
	action = eff.action;

	n = eff.n;
	mix = eff.mix;

	switch(action){
	//case INTRO:case DELETE:
	//	multi_exp = eff.multi_exp;
	//	break;
	case UPDATE:case UPDATE_TOK:
		set = eff.set;
		break;
	case CFLOW:case CFLOW_OFF:
		name = eff.name;
		break;
	case STOP:case SNAPSHOT:case FLUX:case FLUX_OFF:case PRINT:
		string_expr = eff.string_expr;
		break;
	case PRINTF:
		string_expr = eff.string_expr;
		filename = eff.filename;
		break;
	}
	return *this;
}

void Effect::show(string tabs) const {
	tabs += "   ";
	cout << tabs << "effect :" << endl ;
	cout << tabs << "   " << "action:" << action << endl;

	if(n) {
		cout << tabs << "   " << "expression:";
		n->show(tabs+"   ");
		cout << endl;
	}

	if(mix) {
		cout << tabs << "   " << "mixture:" << endl;
		mix->show(tabs+"   ");
	}

	cout << tabs << "   " << "set:" << endl;
	set.show(tabs+"   ");

	cout << tabs << "   " << "name:" << endl;
	name.show(tabs+"   ");

	// show  string_expr and string_expr2 StringExpression class
	cout << endl;
	cout << tabs << "   " << "string_expr:" << endl;
	short i = 0;
	for(list<StringExpression>::const_iterator it = string_expr.cbegin(); it != string_expr.cend() ; it++) {
		cout << tabs << "      " << ++i << ")" ;
		it->show(tabs);
		cout << endl;
	}

	cout << tabs << "   " << "filename:" << endl;
	i = 0;
	for(list<StringExpression>::const_iterator it = filename.cbegin(); it != filename.cend() ; it++) {
		cout << tabs << "      " << ++i << ")" ;
		it->show();
		cout << endl;
	}
}

Effect::~Effect(){
	switch(action){
	case INTRO: case DELETE:
		//delete n;
		//delete mix;
		break;
	default:
		break;
	}
}


/****** Class Perturbation *******/
Perturbation::Perturbation() : condition(nullptr),until(nullptr){};
Perturbation::Perturbation(const location &l,const Expression *cond,const list<Effect> &effs,const Expression* rep):
	Node(l), condition(cond), until(rep), effects(effs){};

void Perturbation::show(string tabs) {
	tabs += "   ";
	cout << "Perturbation {" << endl;

	cout << "condition :" ;
	condition->show(tabs);

	if(until) {
		cout << "until :";
		until->show(tabs);
	}

	for(list<Effect>::const_iterator it = effects.cbegin(); it != effects.cend(); it++){
		cout << endl;
		it->show(tabs);
	}

	cout << endl << "}" << endl;
}

Perturbation::~Perturbation() {
	if(until)
		delete until;
};


/****** Class Radius ****/
Radius::Radius(){}
Radius::Radius(const location &l,const Expression *k1):
	Node(l),k1(k1), opt(NULL) {};
Radius::Radius(const location &l,const Expression *k1,const Expression *opt):
	Node(l),k1(k1), opt(opt) {};
Radius::~Radius(){
	if(opt)
		delete opt;
}


/****** Class Rate ***************/
Rate::Rate():
	Node(), base(nullptr), reverse(nullptr), fixed(nullptr), unary(nullptr) {};
Rate::Rate(const Rate& r) :
		base(r.base ? r.base->clone() : nullptr),
		reverse(r.reverse ? r.reverse->clone() : nullptr),
		unary(r.unary ? new Radius(*r.unary) : nullptr),
		volFixed(r.volFixed),fixed(r.fixed){}
Rate& Rate::operator=(const Rate& r) {
	base = r.base ? r.base->clone() : nullptr;
	reverse = r.reverse ? r.reverse->clone() : nullptr;
	unary = r.unary ? new Radius(*r.unary) : nullptr;
	volFixed = r.volFixed;
	fixed = r.fixed;
	return *this;
}

Rate::Rate(const location &l,const Expression *def,const bool fix):
	Node(l), base(def), reverse(nullptr), fixed(fix), unary(nullptr) {};
Rate::Rate(const location &l,const Expression *def,const bool fix,const Radius *un):
	Node(l), base(def), reverse(nullptr), fixed(fix), unary(new Radius(*un)) {};
Rate::Rate(const location &l,const Expression *def,const bool fix,const Expression *ope):
	Node(l), base(def), reverse(ope), fixed(fix), unary(nullptr) {};
Rate::~Rate(){
	if(base)
		delete base;
	if(unary){
		delete unary->k1;
		delete unary->opt;
	}
	if(reverse)
		delete reverse;
}

const state::BaseExpression* Rate::eval(const pattern::Environment& env,simulation::Rule& r,
		const vector<state::Variable*> &vars,two<pattern::DepSet> &deps,bool is_bi) const {
	if(!base)
		throw std::invalid_argument("Base rate cannot be null.");
	auto base_rate = base->eval(env,vars,&deps.first,Expression::AUX_ALLOW+Expression::RHS);//TODO enum rate?
	r.setRate(base_rate);
	if(is_bi){
		if(unary)
			throw SemanticError("Cannot define a bidirectional rule with a rate for unary cases.",loc);
		if(!reverse)
			WarningStack::getStack().emplace_back(
				"Assuming same rate for both directions of bidirectional rule.",loc);
		else{
			return reverse->eval(env,vars,&deps.second,Expression::AUX_ALLOW);
		}
	}
	else{
		if(reverse)
			throw SemanticError("Reverse rate defined for unidirectional rule.",loc);
		if(unary){
			auto un_rate = unary->k1->eval(env,vars,&deps.first,0);
			int radius = 0;
			if(unary->opt){
				radius = unary->opt->eval(env,vars,&deps.first,Expression::CONST + Expression::AUX_ALLOW)->
						getValue().valueAs<int>();
			}
			r.setUnaryRate(make_pair(un_rate,radius));
		}
	}
	return nullptr;

}

/****** Class Token **************/
Token::Token() : exp(nullptr) {}
Token::Token(const location &l,const Expression *e,const Id &id):
	Node(l), exp(e), id(id) {};
pair<unsigned,const BaseExpression*> Token::eval(const pattern::Environment& env,
			const vector<state::Variable*> &vars,bool neg) const {
	unsigned i;
	try{
		i = env.getTokenId(id.getString());
	}
	catch(std::out_of_range& ex){
		throw SemanticError("Token "+id.getString()+" has not been declared",loc);
	}
	const Expression* expr = neg ? new AlgBinaryOperation(
			location(),exp,new Const(location(),-1),BaseExpression::MULT)
			: exp;
	return make_pair(i,expr->eval(env,vars,0));//TODO flags
}

/****** Class RuleSide ***********/
RuleSide::RuleSide() : agents(location(),list<Agent>()){}
RuleSide::RuleSide(const location &l,const Mixture &agents,const list<Token> &tokens):
	Node(l), agents(agents), tokens(tokens) {};

/****** Class Rule ***************/

size_t Rule::count = 0;

Rule::Rule() : bi(false),filter(nullptr){}
Rule::Rule(	const location &l,
		const Id          &label,
		const RuleSide    &lhs,
		const RuleSide    &rhs,
		const bool       arrow,
		const Expression* where,
		const Rate 		  &rate):
	Node(l), label(label), lhs(lhs), rhs(rhs),
	bi(arrow),filter(where),rate(rate) {count += bi ? 2 : 1;};
Rule::~Rule() {};

Rule::Rule(const Rule& r) : Node(r.loc),label(r.label),lhs(r.lhs),rhs(r.rhs),
		bi(r.bi),filter(r.filter ? r.filter->clone() : nullptr),rate(r.rate){}

Rule& Rule::operator=(const Rule& r){
	loc = r.loc;
	label = r.label;
	lhs = r.lhs;
	rhs = r.rhs;
	bi = r.bi;
	if(r.filter)
		filter = r.filter->clone();
	else
		filter = nullptr;
	rate = r.rate;
	return *this;
}

void Rule::eval(pattern::Environment& env,
		const vector<state::Variable*> &vars) const{
	//TODO eval name...
	auto lhs_mix_p = lhs.agents.eval(env,vars,
			Expression::PATTERN + Expression::LHS + Expression::AUX_ALLOW);//TODO do not allow aux?
	lhs_mix_p->declareAgents(env);
	auto lhs_mask = lhs_mix_p->setAndDeclareComponents(env);
	auto& lhs_mix = env.declareMixture(*lhs_mix_p);
	delete lhs_mix_p;
	auto& rule = env.declareRule(label,lhs_mix);

	lhs_mix.addInclude(rule.getId());
	two<pattern::DepSet> deps;//first-> | second<-
	auto reverse = rate.eval(env,rule,vars,deps,bi);
	for(auto dep : deps.first){
		if(dep.type == pattern::Dependencies::AUX){
			auto cc_id = get<0>(lhs_mix.getAux().at(dep.aux));
			lhs_mix.getComponent(cc_id).addRateDep(rule,cc_id);
		}
	}


	auto rhs_mix_p = rhs.agents.eval(env,vars,
			Expression::RHS + Expression::AUX_ALLOW);
	vector<pattern::ag_st_id> rhs_mask;
	for(auto& t : lhs.tokens)
		rule.addTokenChange(t.eval(env,vars,true));
	for(auto& t : rhs.tokens)
		rule.addTokenChange(t.eval(env,vars));
	if(bi){
		rhs_mix_p->declareAgents(env);
		rhs_mask = rhs_mix_p->setAndDeclareComponents(env);
		auto& rhs_mix = env.declareMixture(*rhs_mix_p);
		delete rhs_mix_p;
		rule.setRHS(&rhs_mix,bi);
		auto reverse_label = label.getString()+" @bckwrds";
		auto& inverse_rule = env.declareRule(Id(label.loc,reverse_label),rhs_mix);
		inverse_rule.setRHS(&lhs_mix,bi);
		inverse_rule.difference(env,rhs_mask,lhs_mask);
		inverse_rule.setRate(reverse);
		rhs_mix.addInclude(inverse_rule.getId());
		for(auto& t : rhs.tokens)
			inverse_rule.addTokenChange(t.eval(env,vars,true));
		for(auto& t : lhs.tokens)
			inverse_rule.addTokenChange(t.eval(env,vars));
		for(auto& dep : deps.second)
			env.addDependency(dep,pattern::Dependencies::RULE,inverse_rule.getId());
	}
	else{
		rhs_mix_p->declareAgents(env,false);
		rhs_mask = rhs_mix_p->setComponents();
		rule.setRHS(rhs_mix_p,bi);
	}
	//DEBUG cout << lhs_mix.toString(env,lhs_mask) << " -> " << rhs_mix_p->toString(env,rhs_mask) << endl;
	rule.difference(env,lhs_mask,rhs_mask);
	for(auto& dep : deps.first)
		env.addDependency(dep,pattern::Dependencies::RULE,rule.getId());
}

size_t Rule::getCount(){
	return count;
}


} /* namespace ast */






