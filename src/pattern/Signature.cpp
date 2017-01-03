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

/************** class Signature *******************/

Signature::Signature(const string &nme) : name(nme) {}

Signature::~Signature() {
	// TODO Auto-generated destructor stub
}

const std::string& Signature::getName() const{
	return name;
}
void Signature::setId(short i){
	id = i;
}

template <typename T>
Signature::Site& Signature::addSite(const ast::Id &name_loc){
	const string& nme(name_loc.getString());
	if(siteMap.count(nme))
		throw SemanticError("Site "+nme+" declared twice!");
	short id = sites.size();
	siteMap[nme] = id;
	sites.push_back(new T(nme));
	return *sites[id];
}
template Signature::Site& Signature::addSite<Signature::EmptySite>(const ast::Id &name_loc);
template Signature::Site& Signature::addSite<Signature::LabelSite>(const ast::Id &name_loc);
template Signature::Site& Signature::addSite<Signature::RangeSite<int> >(const ast::Id &name_loc);
template Signature::Site& Signature::addSite<Signature::RangeSite<float> >(const ast::Id &name_loc);

short Signature::addSite(const ast::Id &name_loc,const vector<string> &labels){
	const string& nme(name_loc.getString());
	if(siteMap.count(nme))
		throw SemanticError("Site "+nme+" declared twice!");
	short id = sites.size();
	siteMap[nme] = id;
	sites.push_back(new LabelSite(nme));
	return id;
}
short Signature::addSite(const ast::Id &name_loc,int min,int max){
	//const string& nme(name_loc.getString());
	return 0;
}
short Signature::addSite(const ast::Id &name_loc,float min,float max){
	//const string& nme(name_loc.getString());
	return 0;
}




/***************** classes Site *********************/

Signature::Site::Site(const string& nme) : name(nme){}
Signature::EmptySite::EmptySite(const string& nme) : Site(nme){}
bool Signature::EmptySite::isPossibleValue(const Value &val){
	return false;
}
Signature::Site::~Site(){}

Signature::LabelSite::LabelSite(const string& nme) : Site(nme),labels(){
	for(unsigned short i = 0;i < labels.size(); i++){
		cout << "site: " << nme << "\t lab: " << labels[i] << endl;
		if(label_ids.count(labels[i]))
			throw SemanticError("Label "+labels[i]+" of site "+nme+" declared twice!");
		label_ids[labels[i]] = i;
	}
}
void Signature::LabelSite::addLabel(const ast::Id& name_loc){
	const string& lbl = name_loc.getString();
	if(label_ids.count(lbl))
		throw SemanticError("Label "+lbl+" of site "+name+" declared twice!");
	label_ids[lbl] = labels.size();
	labels.push_back(lbl);
}
bool Signature::LabelSite::isPossibleValue(const Value &val){
	return label_ids.count(*val.s);
}

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


template <>
bool Signature::RangeSite<float>::isPossibleValue(const Value &val){
	return val.f >= min && val.f <= max;
}
template <>
bool Signature::RangeSite<int>::isPossibleValue(const Value &val){
	return val.i >= min && val.i <= max;
}

template void Signature::RangeSite<float>::setBoundaries(float,float,float);
template void Signature::RangeSite<int>::setBoundaries(int,int,int);


} /* namespace ast */
