/*
 * Dynamics.cpp
 *
 *  Created on: May 12, 2016
 *      Author: naxo
 */

#include "Dynamics.h"

namespace ast {


/****** Class Link ***********/
Link::Link() : type(FREE){}
Link::Link(const location &l,LinkType t):
	Node(l), type(t)  {};
Link::Link(const location &l,LinkType t,int val):
	Node(l), type(t), value(val) {};
Link::Link(const location &l,LinkType t,const Id &ag,const Id &s):
	Node(l), type(t), ag_site{ag,s} {	};
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

void Link::eval(unordered_map<unsigned,list<pair<short,short> > > &links,
		const pair<short,short> &mix_ag_site,bool allow_pattern) const {
	int n;
	switch(type){
	case FREE:

		break;
	case VALUE:
		n = links[value].size();
		if(n > 1)
			throw SemanticError("Edge identifier "+to_string(value)+
					" used to many times.",loc);
		else
			links[value].push_back(mix_ag_site);
		break;
	default:
		if(!allow_pattern)
			throw SemanticError("Patterns are not allowed here.",loc);
		break;
	case ANY:
		break;
	case SOME:
		break;
	case AG_SITE:
		break;
	}
}


/****** Class SiteState ***********/

SiteState::SiteState() :Node(),type(LABEL) {}
SiteState::~SiteState(){};
SiteState::SiteState(const location& loc, const list<Id> &labs)
		: Node(loc),type(LABEL),labels(labs){}

SiteState::SiteState(const location& loc, const Expression* min,
		const Expression* max,const Expression* def)
	: Node(loc),type(RANGE),range{min,def,max}{
		if(!def)
			range[1] = min;
	}

void SiteState::evalLabels(pattern::Signature::LabelSite& site) const{
	for(const auto& lab : labels)
		site.addLabel(lab);
	return;
}

bool SiteState::evalRange(pattern::Environment &env,const vector<state::Variable*> &consts,
		BaseExpression** expr_values) const{
	//using ast::Expression::VAR;
	//using ast::Expression::FLAGS;
	expr_values[0] = range[0]->eval(env,consts,
			Expression::FLAGS::FORCE | Expression::FLAGS::CONST);
	expr_values[2] = range[2]->eval(env,consts,
			Expression::FLAGS::FORCE | Expression::FLAGS::CONST);
	if(range[1] != nullptr)
		expr_values[1] = range[1]->eval(env,consts,
					Expression::FLAGS::FORCE | Expression::FLAGS::CONST);
	else
		expr_values[1] = expr_values[0];
	//Test
	bool isInt = true;
	for(int i = 0; i < 3; i++)
		switch(expr_values[i]->getType()){
		case BaseExpression::BOOL:
			throw SemanticError("Not a valid value to define a range.",range[i]->loc);
		case BaseExpression::FLOAT:
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
	case SiteState::LABEL:
		site = &sign.addSite<pattern::Signature::LabelSite>(name);
		stateInfo.evalLabels(*static_cast<pattern::Signature::LabelSite*>(site));
		break;
	case SiteState::RANGE:
		try{
			BaseExpression* range[3];
			if(!stateInfo.evalRange(env,consts,range)){
				site = &sign.addSite<pattern::Signature::RangeSite<float> >(name);
				static_cast<pattern::Signature::RangeSite<float>*>
					(site)->setBoundaries(range[0]->getValue().valueAs<float>(),
						range[2]->getValue().valueAs<float>(),range[1]->getValue().valueAs<float>());
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

void Site::eval(pattern::Environment &env,const vector<Variable*> &consts,
		pair<short,pattern::Mixture::Agent&> id_agent,
		unordered_map<unsigned,list<pair<short,short> > > &links) const{
	const pattern::Signature* sign;
	short site_id;
	short ag_id = id_agent.first;
	pattern::Mixture::Agent& agent = id_agent.second;
	pattern::Mixture::Site* mix_site;
	try{
		sign = &env.getSignature(agent.getId());
		site_id = sign->getSiteId(name.getString());
		mix_site = &agent.addSite(site_id);
	}
	catch(std::out_of_range &ex){
		throw SemanticError("Site "+name.getString()
				+" is not declared in agent signature of "+
				sign->getName()+"().",loc);
	}
	const state::SomeValue *v;
	state::BaseExpression* num;
	switch(stateInfo.type){
	case SiteState::LABEL:
		if(stateInfo.labels.size() > 1)
			throw SemanticError("List of labels are only allowed in agent declaration.",loc);
		/*pattern::Signature::Value v;
		v.type = pattern::Signature::Value::STRING;
		v.s = &stateInfo.labels.front().getString();*/
		if(stateInfo.labels.size() == 1){
			v = new state::SomeValue(stateInfo.labels.front().getString());
			short lbl_id;
			try{
				lbl_id = sign->getSite(site_id).isPossibleValue(*v);
			}
			catch(std::out_of_range &e){
				throw SemanticError("Label "+*v->sVal+" is not defined for site "+
						name.getString()+" of agent "+sign->getName()+"().",loc);
			}
			agent.setSiteValue(site_id,lbl_id);
		}
		break;
	case SiteState::RANGE:
		throw SemanticError("Ranges for sites are only allowed in agent declaration.",loc);
		break;
	case SiteState::AUX:
		//agent.setS
		break;
	case SiteState::EXPR:
		num = stateInfo.val->eval(env,consts,Expression::CONST);
		sign->getSite(site_id).isPossibleValue(num->getValue());
		break;
	}


	link.eval(links,make_pair(ag_id,site_id),true);
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

void Agent::eval(pattern::Environment &env,const vector<state::Variable*> &consts,pattern::Mixture &mix,
		unordered_map<unsigned,list<pair<short,short> > > &lnks,bool is_pattern) const {
	short sign_id;
	try{
		sign_id = env.getSignatureId(name.getString());
	}
	catch(std::out_of_range &e){
		throw SemanticError("Agent "+name.getString()+"() has not been declared.",loc);
	}

	pattern::Mixture::Agent a_buff(sign_id);
	pair<short,pattern::Mixture::Agent&> id_ag(mix.size(),a_buff);
	for(auto &site : sites){
		site.eval(env,consts,id_ag,lnks);
	}

	auto a = env.declareAgentPattern(a_buff);
	mix.addAgent(&a);
}

void Agent::show( string tabs ) const {
	tabs += "   ";
	cout << "Agent " << name.getString() << " {";

	for(list<Site>::const_iterator it = sites.cbegin(); it != sites.cend(); it++){
		cout << endl;
		it->show( tabs );
	}

	cout << endl << "}" << endl;
}


/****** Class Mixture ************/
Mixture::Mixture(){}

Mixture::Mixture(const location &l,const list<Agent> &m):
	Node(l), agents(m) {};

Mixture::~Mixture(){};

pattern::Mixture Mixture::eval(pattern::Environment &env,
		const vector<Variable*> &vars,bool is_pattern) const{
	pattern::Mixture &mix(env.declareMixture(agents.size()));
	unordered_map<unsigned,list<pair<short,short> > > links;
	for(list<Agent>::const_iterator it = agents.cbegin();it != agents.cend();it++){
		it->eval(env,vars,mix,links,is_pattern);
	}
	for(auto &n_link : links){
		if(n_link.second.size() == 1)
			throw SemanticError("Edge identifier "+to_string(n_link.first)+
					" is not paired in mixture.",loc);
		mix.addLink(n_link.second.front(),*(++n_link.second.begin()));
	}
	mix.setComponents(env);
	return mix;
}


/****** Class MultipleMixture ****/
MultipleMixture::MultipleMixture(): n(nullptr){};
MultipleMixture::MultipleMixture(const location &l,const list<Agent> &m,const Expression *e):
	Mixture(l,m), n(e) {};

MultipleMixture::~MultipleMixture(){
	if(n)
		delete n;
}

/*TODO*/
pattern::Mixture MultipleMixture::eval(pattern::Environment &env) const{

	for(list<Agent>::const_iterator it = agents.cbegin();it != agents.cend();it++){
		//it->;
	}
	return pattern::Mixture(0);
}




/****** Class Effect *************/
Effect::Effect(){}
	//INTRO,DELETE
Effect::Effect(const location &l,const Action &a,const MultipleMixture &mix):
	Node(l),action(a),multi_exp(mix) {};
//UPDATE,UPDATE_TOK
Effect::Effect(const location &l,const Action &a,const VarValue &dec):
	Node(l),action(a),set(dec) {};
//CFLOW,CFLOW_OFF
Effect::Effect(const location &l,const Action &a,const Id &id):
	Node(l),action(a),name(id) {};
//STOP,SNAPSHOT,FLUX,FLUXOFF,PRINT
Effect::Effect(const location &l,const Action &a,const StringExpression &str):
	Node(l),action(a),string_expr(new StringExpression(str)) {};
//PRINTF
Effect::Effect(const location &l,const Action &a,const StringExpression &str1,const StringExpression &str2):
		Node(l),action(a),string_expr(new StringExpression[2]{str1,str2}) {};

Effect::Effect(const Effect &eff) : Node(eff.loc),action(eff.action){
	switch(action){
	case INTRO:case DELETE:
		multi_exp = eff.multi_exp;
		break;
	case UPDATE:case UPDATE_TOK:
		set = eff.set;
		break;
	case CFLOW:case CFLOW_OFF:
		name = eff.name;
		break;
	case STOP:case SNAPSHOT:case FLUX:case FLUX_OFF:case PRINT:
		string_expr = new StringExpression(*eff.string_expr);
		break;
	case PRINTF:
		string_expr = new StringExpression[2]{eff.string_expr[0],eff.string_expr[1]};
	}
}

Effect& Effect::operator=(const Effect& eff){
	loc = eff.loc;
	action = eff.action;
	switch(action){
	case INTRO:case DELETE:
		multi_exp = eff.multi_exp;
		break;
	case UPDATE:case UPDATE_TOK:
		set = eff.set;
		break;
	case CFLOW:case CFLOW_OFF:
		name = eff.name;
		break;
	case STOP:case SNAPSHOT:case FLUX:case FLUX_OFF:case PRINT:
		string_expr = new StringExpression(*eff.string_expr);
		break;
	case PRINTF:
		string_expr = new StringExpression[2]{eff.string_expr[0],eff.string_expr[1]};
	}
	return *this;
}

Effect::~Effect(){
	switch(action){
	case STOP:case SNAPSHOT:case FLUX:case FLUX_OFF:case PRINT:
		delete string_expr;
		break;
	case PRINTF:
		delete[] string_expr;
		break;
	default:
		break;
	}
}


/****** Class Perturbation *******/
Perturbation::Perturbation() : condition(nullptr),until(nullptr){};
Perturbation::Perturbation(const location &l,const Expression *cond,const list<Effect> &effs,const Expression* rep):
	Node(l), condition(cond), until(rep), effects(effs){};

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
Rate::Rate(){}
Rate::Rate(const location &l,const Expression *def,const bool &fix):
	Node(l), def(def), fixed(fix), un(NULL), op(NULL) {};
Rate::Rate(const location &l,const Expression *def,const bool &fix,const Radius *un):
	Node(l), def(def), fixed(fix), un(new Radius(*un)), op(NULL) {};
Rate::Rate(const location &l,const Expression *def,const bool &fix,const Expression *ope):
	Node(l), def(def), fixed(fix), un(NULL), op(ope) {};
Rate::~Rate(){
	if(un)
		delete un;
	if(op)
		delete op;
}

/****** Class Token **************/
Token::Token() : exp(nullptr) {}
Token::Token(const location &l,const Expression *e,const Id &id):
	Node(l), exp(e), id(id) {};

/****** Class RuleSide ***********/
RuleSide::RuleSide() : agents(location(),list<Agent>()){}
RuleSide::RuleSide(const location &l,const Mixture &agents,const list<Token> &tokens):
	Node(l), agents(agents), tokens(tokens) {};

/****** Class Rule ***************/

Rule::Rule(){}
Rule::Rule(	const location &l,
		const Id          &label,
		const RuleSide    &lhs,
		const RuleSide    &rhs,
		const bool       arrow,
		const Expression* where,
		const Rate 		  &rate):
	Node(l), label(label), lhs(lhs), rhs(rhs),
	bidirectional(arrow),filter(where),rate(rate) {};
Rule::~Rule() {};

} /* namespace ast */
