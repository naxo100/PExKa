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


/****** Class SiteState ***********/

SiteState::SiteState() :Node(),type(LABEL) {}
SiteState::~SiteState(){};
SiteState::SiteState(const location& loc, const list<Id> &labs)
		: Node(loc),type(LABEL),labels(labs){}

SiteState::SiteState(const location& loc, const Expression* min,
		const Expression* max,const Expression* def)
	: Node(loc),type(RANGE),range{min,def,max}{}

const vector<string>& SiteState::evalLabels(){
	vector<string> *labs = new vector<string>(labels.size());
	for(list<Id>::iterator it = labels.begin();it != labels.end(); it++)
		labs->push_back(it->getString());
	return *labs;
}

bool SiteState::evalRange(pattern::Environment &env,
		BaseExpression** expr_values){
	//using ast::Expression::VAR;
	//using ast::Expression::FLAGS;
	expr_values[0] = range[0]->eval(env,Expression::VAR(),
			Expression::FLAGS::FORCE | Expression::FLAGS::CONST);
	expr_values[2] = range[2]->eval(env,Expression::VAR(),
			Expression::FLAGS::FORCE | Expression::FLAGS::CONST);
	if(range[1] != nullptr)
		expr_values[1] = range[1]->eval(env,Expression::VAR(),
					Expression::FLAGS::FORCE | Expression::FLAGS::CONST);
	else
		expr_values[1] = expr_values[0];
	//Test
	bool isInt = true;
	for(int i = 0; i < 3; i++)
		switch(expr_values[i]->getType()){
		case BaseExpression::BOOL:
			throw std::exception();
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

void Site::eval(pattern::Environment &env,pattern::Signature &sign){
	if(link.getType())
		throw std::exception();//TODO
	//short id;
	switch(stateInfo.type){
	case SiteState::LABEL:
		sign.addSite(name.getString(),stateInfo.evalLabels());

		break;
	case SiteState::RANGE:
		BaseExpression* range[3];
		if(stateInfo.evalRange(env,range))
			sign.addSite(name.getString(),range[0]->getValue().fVal,
					range[2]->getValue().fVal);
		else
			sign.addSite(name.getString(),range[0]->getValue().iVal,
					range[2]->getValue().iVal);
		break;
	}
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

pattern::Signature* Agent::eval(pattern::Environment &env){
	//using namespace pattern;
	pattern::Signature* sign = new pattern::Signature(name.getString());
	short id = env.declareSignature(*sign);
	sign->setId(id);

	for(list<Site>::iterator it = sites.begin(); it != sites.end(); it++){
		it->eval(env,*sign);
	}
	return sign;
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
//TODO
Mixture::Mixture(){}

Mixture::Mixture(const location &l,const list<Agent> &m):
	Node(l), mix(m) {};

//TODO
Mixture::~Mixture(){};
/*TODO*/
pattern::Mixture Mixture::eval(pattern::Environment &env) const{

	for(list<Agent>::const_iterator it = mix.cbegin();it != mix.cend();it++){
		//it->;
	}
	return pattern::Mixture();
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

	for(list<Agent>::const_iterator it = mix.cbegin();it != mix.cend();it++){
		//it->;
	}
	return pattern::Mixture();
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
Token::Token(){}
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
