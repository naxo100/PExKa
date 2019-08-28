/*
 * Agent.cpp
 *
 *  Created on: Jan 21, 2019
 *      Author: naxo100
 */

#include "Agent.h"
#include "../Environment.h"
#include "../../expressions/Vars.h"

namespace pattern {

using namespace expressions;
/*************** Class Agent ********************/

Mixture::Agent::Agent(short_id sign_id) : signId(sign_id){}
Mixture::Agent::~Agent(){};

void Mixture::Agent::setLoc(const yy::location& _loc) {
	loc = _loc;
}

const yy::location& Mixture::Agent::getLoc() const {
	return loc;
}



Mixture::Site& Mixture::Agent::addSite(small_id env_site){
	return interface[env_site] = Site();
}

const Mixture::Site& Mixture::Agent::getSite(small_id id) const {
	return interface.at(id);
}
const Mixture::Site& Mixture::Agent::getSiteSafe(small_id id) const {
	static Site empty_site(Site::EMPTY,WILD);
	if(interface.count(id))
		return interface.at(id);
	else
		return empty_site;
}

short_id Mixture::Agent::getId() const {
	return signId;
}

map<small_id,string> Mixture::Agent::getAuxNames() const {
	map<small_id,string> ret;
	for(auto id_site : *this){
		string name;
		auto aux = dynamic_cast<const expressions::Auxiliar<FL_TYPE>*>(id_site.second.values[1]);
		if(aux)
			ret.emplace(id_site.first,aux->toString());
	}
	return ret;
}


const map<small_id,Mixture::Site>::const_iterator Mixture::Agent::begin() const{
	return interface.begin();
}
const map<small_id,Mixture::Site>::const_iterator Mixture::Agent::end() const{
	return interface.end();
}

bool Mixture::Agent::operator ==(const Agent &a) const {
	if(this == &a)
		return true;
	if(signId == a.signId && interface.size() == a.interface.size()){
		for(auto &id_site : interface){
			try{
				if(! (id_site.second == a.getSite(id_site.first)) )
					return false;
			}
			catch(std::out_of_range &e){
				return false;
			}
		}
	}
	else
		return false;
	return true;
}

bool Mixture::Agent::testEmbed(const Agent &a,list<small_id>& to_test) const {
	if(this == &a)
		return true;
	if(signId != a.signId)
		return false;

	set<small_id> already_done;
	for(auto &id_site : interface){
		to_test.push_front(id_site.first);
		if(!id_site.second.testEmbed(a.getSiteSafe(id_site.first),to_test))
			return false;
		to_test.pop_front();
		already_done.insert(id_site.first);
	}
	for(auto &id_site : a.interface){
		to_test.push_front(id_site.first);
		if(already_done.count(id_site.first))
			continue;
		if(!getSiteSafe(id_site.first).testEmbed(id_site.second,to_test))
			return false;
		to_test.pop_front();
	}
	return true;
}

void Mixture::Agent::setSiteValue(small_id site_id,small_id lbl_id){
	//interface[site_id].val_type = ValueType::LABEL;
	interface[site_id].label = lbl_id;
}
void Mixture::Agent::setSiteValue(small_id site_id,FL_TYPE val){
	//interface[site_id].val_type = ValueType::LABEL;
	//interface[site_id].label = Site::AUX;
}
void Mixture::Agent::setSiteValue(small_id site_id,int val){
	//interface[site_id].val_type = ValueType::LABEL;/
	//interface[site_id].label = Site::AUX;
}

void Mixture::Agent::setSiteExprValue(small_id mix_site,const BaseExpression* expr){
	interface[mix_site].values[1] = expr;
	interface[mix_site].label = Site::EXPR;//Expression site TODO
}
void Mixture::Agent::setSiteAuxPattern(small_id mix_site, BaseExpression** vals){
	//interface[mix_site].aux_id = id;
	interface[mix_site].label = Site::AUX;//expression site
	for(int i = 0; i < 3; i++)
		interface[mix_site].values[i] = vals[i];
}
/*
void Mixture::Agent::setSiteMinExpr(small_id mix_site,const BaseExpression* expr){
	interface[mix_site].values[0] = expr;
}
void Mixture::Agent::setSiteMaxExpr(small_id mix_site,const BaseExpression* expr){
	interface[mix_site].values[2] = expr;
}
*/



void Mixture::Agent::setLinkPtrn(small_id trgt_site,small_id ag_ptrn,small_id site_ptrn){
	interface.at(trgt_site).lnk_ptrn = make_pair(ag_ptrn,site_ptrn);
}

/*
void Mixture::Agent::addParent(small_id id,Agent *a){
	parents[id].emplace_back(a);
}
void Mixture::Agent::addParents(small_id id,const list<Agent*>& la){
	parents[id].insert(parents[id].end(),la.begin(),la.end());
}
void Mixture::Agent::addChild(small_id id,Agent *a){
	childs[id].emplace_back(a);
}
void Mixture::Agent::addChilds(small_id id,const list<Agent*>& la){
	childs[id].insert(childs[id].end(),la.begin(),la.end());
}

const list<Mixture::Agent*>& Mixture::Agent::getParentPatterns(small_id id) const {
	static list<Mixture::Agent*> empty;
	try{
		return parents.at(id);
	} catch(std::out_of_range &ex){}
	return empty;
}
const list<Mixture::Agent*>& Mixture::Agent::getChildPatterns(small_id id) const {
	static list<Mixture::Agent*> empty;
	try{
		return childs.at(id);
	} catch(std::out_of_range &ex){}
	return empty;
}
const map<small_id,list<Mixture::Agent*> >& Mixture::Agent::getParentPatterns() const {
	return parents;
}
const map<small_id,list<Mixture::Agent*> >& Mixture::Agent::getChildPatterns() const {
	return childs;
}
*/

void Mixture::Agent::addCc(const Mixture::Component* cc,small_id id) const{
	includedIn.emplace_back(cc,id);
}
const list<pair<const Mixture::Component*,small_id> >& Mixture::Agent::getIncludes() const{
	return includedIn;
}


string Mixture::Agent::toString(const Environment& env, short mixAgId,
		map<ag_st_id,short> bindLabels  ) const {
	string out = "", glue = ",";

	const Signature& sign = env.getSignature(signId);
	out += sign.getName() + "(";

	// inspect interface
	for( auto it = interface.begin(); it != interface.end(); ++it ) {
		/* it.first = site ID , it.second = Mixture::Site type
		 */
		const Signature::Site& site = sign.getSite( it->first );
		const Signature::LabelSite* labelSite;

		// variables needed to LinkType::BIND_TO
		const Signature* signature2bind;
		const Signature::Site* site2bind;

		//out += "[" + to_string(mixAgId) + "," + to_string(it->first) + "]" + site.getName(); //site name
		out += site.getName(); //site name

		switch(it->second.label){
		case Site::EXPR:
			out += "~{ ";
			if(it->second.values[1])
				out += it->second.values[1]->toString();
			out += " }";
			break;
		case Site::AUX:
			out += "~{ ";
			if(it->second.values[0])
				out += it->second.values[0]->toString()+ " <= ";
			if(it->second.values[1])
				out += it->second.values[1]->toString();
			else
				out += "AUX";
			if(it->second.values[2])
				out += " <= " + it->second.values[2]->toString();
			out += " }";
			break;
		default:
			labelSite = dynamic_cast<const Signature::LabelSite*>(& site);
			if(labelSite && !it->second.isEmptySite())//is not an empty site
				out += "~" + labelSite->getLabel(it->second.label); //value of site
				//throw std::invalid_argument("Mixture::Agent::toString(): not a valid state type.");
		}

		switch(it->second.link_type) {
			case LinkType::BIND :
				if ( bindLabels.size() > 0 && bindLabels[ag_st_id(mixAgId, it->first)] )
					out += "!" + to_string( bindLabels[ag_st_id(mixAgId, it->first)] );
				else
					if(it->second.isBindToAny())
						out += "!_";
					else{
						signature2bind = & env.getSignature( it->second.lnk_ptrn.first );
						site2bind = & signature2bind->getSite( it->second.lnk_ptrn.second );
						out += "!{" + site2bind->getName() + "." + signature2bind->getName()+"}";
					}
				break;
			case LinkType::BIND_TO :
				signature2bind = & env.getSignature( it->second.lnk_ptrn.first );
				site2bind = & signature2bind->getSite( it->second.lnk_ptrn.second );
				out += "!"+ site2bind->getName() + "." + signature2bind->getName();
				break;
			case LinkType::FREE :
				out += ""; // no noted
				break;
			case LinkType::PATH :
				out += "PATH";
				break;
			case LinkType::WILD :
				out += "?";
				break;
		}

		out += glue;
	}

	// remove last glue
	if( out.substr(out.size()-glue.size(), out.size()) == glue )
		out = out.substr(0, out.size()-glue.size());

	out += ")";

	return out;
}



/************** class Site *********************/


Mixture::Site::Site(small_id lbl,LinkType lt) : label(lbl),link_type(lt),lnk_ptrn(-1,-1),
		values{nullptr,nullptr,nullptr}{}

Mixture::Site::Site(expressions::SomeValue val,LinkType lt) : label(0),link_type(lt),lnk_ptrn(-1,-1),
		values{nullptr,nullptr,nullptr}{
	switch(val.t){
	case expressions::SMALL_ID:
		label = val.smallVal;break;
	case expressions::FLOAT:
		values[1] = new expressions::Constant<FL_TYPE>(val.fVal);break;
	case expressions::INT:
		values[1] = new expressions::Constant<int>(val.iVal);break;
	default:
		throw invalid_argument("Not a valid value type for agent site.");
	}
}

Mixture::Site::Site(const Site& site) : label(site.label),link_type(site.link_type),lnk_ptrn(site.lnk_ptrn){
	for(int i = 0; i < 3; i++)
		if(site.values[i]){
			if(dynamic_cast<const state::Variable*>(site.values[i]))
				values[i] = site.values[i];
			else
				values[i] = site.values[i]->clone();
		}
		else
			values[i] = nullptr;
}

Mixture::Site::~Site(){
	for(int i = 0; i < 3; i++)
		if(values[i] && !dynamic_cast<const state::Variable*>(values[i]))
			delete values[i];
}
/*
Mixture::Site& Mixture::Site::operator=(const Site& site) : label(site.label),link_type(site.link_type),lnk_ptrn(site.lnk_ptrn){
	for(int i = 0; i < 3; i++)
		if(values[i]){
			if(dynamic_cast<const state::Variable*>(values[i]))
				values[i] = site.values[i];
			else
				values[i] = site.values[i]->clone();
		}
		else
			values[i] = nullptr;
}*/

bool Mixture::Site::isEmptySite() const{
	return label == EMPTY ;//&& values[1] == nullptr;
}
bool Mixture::Site::isExpression() const{
	throw invalid_argument("this method is wrong");//return label == AUX || values[1];
}

//test if mix_site match with value or inequation
bool Mixture::Site::testValue(const state::SomeValue& val,const VarVector& vars) const {
	//TODO ignore vars when using for check influence
	if(label != EMPTY && val.t != NONE){
		if(val.t == expressions::SMALL_ID)
			return val.smallVal == label;
		else{
			if(values[1]){
				if(!values[1]->isAux())//TODO only if aux expression are stored
					try{
						return values[1]->getValue(vars) == val;
					}
					catch(std::out_of_range &e){
						return true;//cannot evaluate so maybe true
					}
			}
		}
		auto fl_val = val.valueAs<FL_TYPE>();
		if(values[0])
			if(values[0]->getValue(vars).valueAs<FL_TYPE>() > fl_val)
				return false;
		if(values[2])
			if(values[2]->getValue(vars).valueAs<FL_TYPE>() < fl_val)
				return false;
		return true;
	}
	return true;
}

//test if mix_site match with value or inequation
bool Mixture::Site::testValueOpt(const state::SomeValue& val,const state::State& state,
		const expressions::AuxMap& aux_map) const {
	//TODO ignore vars when using for check influence
	if(val.t == expressions::SMALL_ID)
		return val.smallVal == label;
	else{
		if(values[1]){
			if(!values[1]->isAux())//TODO only if aux expression are stored
				try{
					return values[1]->getValue(state,std::move(aux_map)) == val;
				}
				catch(std::out_of_range &e){
					if(aux_map.size())//aux_values is not empty so we miss some aux
						throw out_of_range("testValueOpt(): Auxiliar or Var was not found.");
					else
						throw out_of_range("testValueOpt(): no auxiliars given.");//no aux?? something wrong here
				}
		}
	}
	auto fl_val = val.valueAs<FL_TYPE>();
	if(values[0])
		if(values[0]->getValue(state,move(aux_map)).valueAs<FL_TYPE>() > fl_val)
			return false;
	if(values[2])
		if(values[2]->getValue(state,move(aux_map)).valueAs<FL_TYPE>() < fl_val)
			return false;
	return true;
}

bool Mixture::Site::operator ==(const Site &s) const{
	switch(label){
	case AUX:
		if(s.label == EMPTY)
			break;
		if(s.label != AUX)
			return false;
		for(int i = 0; i < 3; i=i+2)
			if(values[i] != s.values[i]){
				if(values[i] && s.values[i]){
					if(*values[i] != *s.values[i])
						return false;
				}
				else
					return false;
			}
		break;
	case EXPR:
		if(s.label != EXPR)
			return false;
		if(*values[1] != *s.values[1])
			return false;
		break;
	case EMPTY:
		if(s.label == AUX)
			break;
		if(label != s.label)
			return false;
		break;
	default:
		if(label != s.label)
			return false;
	}
	if(s.link_type == link_type){
		if(link_type >= LinkType::BIND && s.lnk_ptrn != lnk_ptrn)//bind or bind_to //TODO PATH
			return false;
	}
	else
		return false;

	return true;
}

/*test if this rhs-site is contained in s*/
bool Mixture::Site::testEmbed(const Site &s,list<small_id>& to_test) const{
	switch(label){
	case EMPTY:break;
	case EXPR:
		if(values[1] && values[1]->getVarDeps() <= BaseExpression::CONSTS){
			auto rhs_val = values[1]->getValue(VarVector());
			if(s.values[1] && s.values[1]->getVarDeps() < BaseExpression::CONSTS &&
					s.values[1]->getValue(VarVector()).valueAs<FL_TYPE>() != rhs_val.valueAs<FL_TYPE>() )
				return false;
		}
		break;
	case AUX:
		if(s.label == EMPTY)//else is AUX never VAL
			break;
		if(values[1] && values[1]->getVarDeps() <= BaseExpression::CONSTS){
			auto rhs_val = values[1]->getValue(VarVector());
			if(s.values[0] && s.values[0]->getVarDeps() < BaseExpression::CONSTS &&
					s.values[0]->getValue(VarVector()).valueAs<FL_TYPE>() > rhs_val.valueAs<FL_TYPE>() )
				return false;
			if(s.values[2] && s.values[2]->getVarDeps() < BaseExpression::CONSTS &&
					s.values[2]->getValue(VarVector()).valueAs<FL_TYPE>() < rhs_val.valueAs<FL_TYPE>() )
				return false;
			if(s.values[1] && s.values[1]->getVarDeps() < BaseExpression::CONSTS &&
					s.values[1]->getValue(VarVector()).valueAs<FL_TYPE>() != rhs_val.valueAs<FL_TYPE>() )
				return false;
		}
		break;
	default:
		if(s.label != EMPTY && label != s.label)
			return false;
	}

	if(link_type == WILD || s.link_type == WILD)
		return true;
	switch(link_type){//rhs-link-type
	case FREE:
		if(s.link_type != FREE)
			return false;
		break;
	case BIND:
		if(s.link_type == BIND)
			to_test.push_back(to_test.front());
		//no break
	case BIND_TO:
		if(s.lnk_ptrn != lnk_ptrn)
			return false;
		break;
	default:break;//only PATH
	}
	return true;
}

int Mixture::Site::compareLinkPtrn(ag_st_id ptrn) const{
	if(isBindToAny()){
		if(ptrn.first == small_id(-1))
			return 0;
		else
			return 1;
	}
	else{
		if(ptrn.first == small_id(-1))
			return -1;
		else
			if(lnk_ptrn == ptrn)
				return link_type == LinkType::BIND_TO ? 1 : 0;//need to compare with other type
			else
				throw False();
	}
}

bool Mixture::Site::isBindToAny() const{
	return link_type == LinkType::BIND && lnk_ptrn.first == small_id(-1);
}





} /* namespace pattern */
