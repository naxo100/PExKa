/*
 * Signature.cpp
 *
 *  Created on: Aug 10, 2016
 *      Author: naxo
 */

#include "Signature.h"
#include "../util/Exceptions.h"
#include "../grammar/ast/Basics.h"

namespace pattern {

using namespace expressions;
using namespace grammar::ast;

/************** class Signature *******************/

Signature::Signature(const string &nme) : id((short_id)-1),name(nme) {}

Signature::~Signature() {
	for(auto site : sites){
		delete site;
	}
}

const std::string& Signature::getName() const{
	return name;
}
short_id Signature::getId() const {
	return id;
}
void Signature::setId(short_id i){
	id=i;
}
small_id Signature::getSiteCount() const {
	return sites.size();
}

short Signature::getSiteId(const string &name) const{
	return siteMap.at(name);
}

template <typename T>
Signature::Site& Signature::addSite(const Id &name_loc){
	const string& nme(name_loc.getString());
	if(siteMap.count(nme))
		throw SemanticError("Site "+nme+" declared twice!",name_loc.loc);
	short id = sites.size();
	siteMap[nme] = id;
	sites.push_back(new T(nme));
	return *sites[id];
}
template Signature::Site& Signature::addSite<Signature::EmptySite>(const Id &name_loc);
template Signature::Site& Signature::addSite<Signature::LabelSite>(const Id &name_loc);
template Signature::Site& Signature::addSite<Signature::RangeSite<int> >(const Id &name_loc);
template Signature::Site& Signature::addSite<Signature::RangeSite<FL_TYPE> >(const Id &name_loc);

small_id Signature::addSite(const Id &name_loc,int min,int max){
	//const string& nme(name_loc.getString());
	return 0;
}
small_id Signature::addSite(const Id &name_loc,FL_TYPE min,FL_TYPE max){
	//const string& nme(name_loc.getString());
	return 0;
}

const Signature::Site& Signature::getSite(small_id id) const{
	return *(sites[id]);
}
const vector<Signature::Site*>& Signature::getSites() const {
	return sites;
}


/***************** classes Site *********************/

Signature::Site::Site(const string& nme) : name(nme){}
Signature::Site::~Site(){}
const std::string& Signature::Site::getName() const {
	return this->name;
}

two<expressions::SomeValue> Signature::Site::getLimits() const {
	throw invalid_argument("This site doesn't have limits.");
}


///EmptySite
Signature::EmptySite::EmptySite(const string& nme) : Site(nme){}
bool Signature::EmptySite::isPossibleValue(const state::SomeValue &val) const {
	return false;
}
state::SomeValue Signature::EmptySite::getDefaultValue() const {
	//no default value
	//throw std::invalid_argument("Signature::EmptySite::getDefaultValue(): no default value for empty site.");
	return state::SomeValue(small_id(-1));
}


///LabelSite
Signature::LabelSite::LabelSite(const string& nme) : Site(nme),labels(){ }

void Signature::LabelSite::addLabel(const Id& name_loc){
	const string& lbl = name_loc.getString();
	if(label_ids.count(lbl))
		throw SemanticError("Label "+lbl+" of site "+name+" declared twice!",name_loc.loc);
	label_ids[lbl] = labels.size();
	labels.push_back(lbl);
}
/*short Signature::LabelSite::isPossibleValue(const state::SomeValue &val) const {
	if(val.t != state::BaseExpression::STR)
		throw std::invalid_argument("Not a string value.");
	return label_ids.at(*val.sVal);
}*/
const string& Signature::LabelSite::getLabel( small_id id ) const {
	return labels[id];
}
small_id Signature::LabelSite::getLabelId(const string& s) const {
	return label_ids.at(s);
}
state::SomeValue Signature::LabelSite::getDefaultValue() const {
	return state::SomeValue(small_id(0));
}

bool Signature::LabelSite::isPossibleValue(const state::SomeValue &val) const {
	return val.t == SMALL_ID && val.smallVal < labels.size();
}


///RangeSite
template <typename T>
Signature::RangeSite<T>::RangeSite(const string &nme) : Site(nme){};

template <typename T>
void Signature::RangeSite<T>::setBoundaries(T mn, T mx, T def){
	if(mn >= mx)
		throw std::out_of_range("Bad definition of min-max range.");
	if(def < mn || def > mx)
		throw std::out_of_range("Default value for range-site is out of boundaries.");
	min=mn;max=mx;byDefault=def;
	//std::cout << "min: " << min << "\tmax: " << max << "\tdef: " << byDefault << std::endl;
}

template <typename T>
state::SomeValue Signature::RangeSite<T>::getDefaultValue() const {
	return state::SomeValue(byDefault);
}

template <typename T>
two<SomeValue> Signature::RangeSite<T>::getLimits() const {
	return make_pair(SomeValue(min),SomeValue(max));
}

template <>
bool Signature::RangeSite<FL_TYPE>::isPossibleValue(const state::SomeValue &val) const {
	return val.t == FLOAT && val.fVal >= min && val.fVal <= max;
}
template <>
bool Signature::RangeSite<int>::isPossibleValue(const state::SomeValue &val) const {
	return val.t == INT && val.iVal >= min && val.iVal <= max;
}

template void Signature::RangeSite<FL_TYPE>::setBoundaries(FL_TYPE,FL_TYPE,FL_TYPE);
template void Signature::RangeSite<int>::setBoundaries(int,int,int);


} /* namespace ast */
