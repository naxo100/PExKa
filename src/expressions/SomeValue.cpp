/*
 * SomeValue.cpp
 *
 *  Created on: Oct 29, 2018
 *      Author: naxo100
 */

#include "SomeValue.h"
#include <stdexcept>
#include <iostream>


namespace expressions {


/****** SomeValue ************/
SomeValue::SomeValue() : t(NONE) {}
SomeValue::SomeValue(FL_TYPE f) :
		fVal(f), t(FLOAT) {
}
SomeValue::SomeValue(int i) :
		fVal(0.0), t(INT) {
	iVal = i;
}
SomeValue::SomeValue(bool b) :
		fVal(0.0), t(BOOL) {
	bVal = b;
}
SomeValue::SomeValue(small_id i) :
		fVal(0.0), t(SMALL_ID) {
	smallVal = i;
}
SomeValue::SomeValue(short_id i) :
		fVal(0.0), t(SHORT_ID) {
	shortVal = i;
}

//SomeValue::SomeValue(const std::string &s) : sVal(new std::string(s)),t(BaseExpression::STR){}

template<typename T>
T SomeValue::valueAs() const {
	switch (t) {
	case FLOAT:
		return static_cast<T>(fVal);
	case INT:
		return static_cast<T>(iVal);
	case BOOL:
		return static_cast<T>(bVal);
	default:
		throw std::invalid_argument(
				"SomeValue::valueAs(): not sense in convert id-like values.");
		/*case BaseExpression::STR:
		 return std::stof(*sVal);*/
	}
	return 0;
}
template FL_TYPE SomeValue::valueAs<FL_TYPE>() const;
template int SomeValue::valueAs<int>() const;
template bool SomeValue::valueAs<bool>() const;

template<> void SomeValue::set(FL_TYPE f) {
	fVal = f;
}
template<> void SomeValue::set(int i) {
	iVal = i;
}
template<> void SomeValue::set(bool b) {
	bVal = b;
}
template<> void SomeValue::set(small_id i) {
	smallVal = i;
}
template<> void SomeValue::set(short_id i) {
	shortVal = i;
}

bool SomeValue::operator !=(const SomeValue& val) const {
	/*switch(t){
	 case BaseExpression::FLOAT:
	 return static_cast<T>(fVal);
	 case BaseExpression::INT:
	 return static_cast<T>(iVal);
	 case BaseExpression::BOOL:
	 return static_cast<T>(bVal);
	 default:
	 throw std::invalid_argument("SomeValue::valueAs(): not sense in convert id-like values.");
	 }*/
	return val.fVal != fVal || val.t != t;
}
bool SomeValue::operator ==(const SomeValue& val) const {
	return val.fVal == fVal && val.t == t;
}

std::ostream& operator <<(std::ostream& out,const SomeValue& val) {
	switch(val.t){
	case FLOAT:
		return out << std::to_string(val.fVal);
	case INT:
		return out << val.iVal;
	case BOOL:
		return out << val.bVal;
	case SMALL_ID:
		return out << int(val.smallVal);
	case SHORT_ID:
		return out << int(val.shortVal);
	default:
		return out << "NaN";
	}
}


}
