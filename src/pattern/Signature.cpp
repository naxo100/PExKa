/*
 * Signature.cpp
 *
 *  Created on: Aug 10, 2016
 *      Author: naxo
 */

#include "Signature.h"

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


short Signature::addSite(const string &nme){
	if(siteMap.count(nme))
		throw std::exception();
	short id = sites.size();
	siteMap[nme] = id;
	sites.push_back(new EmptySite(nme));
	return id;
}

short Signature::addSite(const string &nme,const vector<string> &labels){
	if(siteMap.count(nme))
		throw std::exception();
	short id = sites.size();
	siteMap[nme] = id;
	sites.push_back(new LabelSite(nme,labels));
	return id;
}
short Signature::addSite(const string &name,int min,int max){}
short Signature::addSite(const string &name,float min,float max){}




/***************** classes Site *********************/

Signature::Site::Site(const string& nme) : name(nme){}
Signature::EmptySite::EmptySite(const string& nme) : Site(nme){}
bool Signature::EmptySite::isPossibleValue(const Value &val){
	return false;
}
Signature::Site::~Site(){}

Signature::LabelSite::LabelSite(const string& nme,const vector<string> &labs) : Site(nme),labels(labs){
	for(unsigned short i = 0;i < labels.size(); i++){
		if(label_ids.count(labels[i]))
			throw exception();//TODO
		label_ids[labels[i]] = i;
	}
}
bool Signature::LabelSite::isPossibleValue(const Value &val){
	return label_ids.count(*val.s);
}

template <typename T>
Signature::RangeSite<T>::RangeSite(const string& nme, const T mn, const T mx)
		: Site(nme),min(mn),max(mx),byDefault(mn){}
template <typename T>
Signature::RangeSite<T>::RangeSite(const string& nme, const T mn, const T mx, const T df)
		: Site(nme),min(mn),max(mx),byDefault(df){}

template <>
bool Signature::RangeSite<float>::isPossibleValue(const Value &val){
	return val.f >= min && val.f <= max;
}

} /* namespace ast */
