/*
 * SomeValue.h
 *
 *  Created on: Oct 29, 2018
 *      Author: naxo100
 */

#ifndef SRC_EXPRESSIONS_SOMEVALUE_H_
#define SRC_EXPRESSIONS_SOMEVALUE_H_

#include "../util/params.h"

namespace expressions {

enum Type {
	FLOAT, INT, BOOL, SMALL_ID, SHORT_ID, NONE
};

class SomeValue {
private:
public:
	union {
		FL_TYPE fVal;
		int iVal;
		small_id smallVal;
		short_id shortVal;
		bool bVal;
		//std::string* sVal;
	};
	Type t;

	SomeValue();
	SomeValue(FL_TYPE f);
	SomeValue(int i);
	SomeValue(short_id id);
	SomeValue(small_id id);
	SomeValue(bool b);
	//SomeValue(const std::string &s);
	//SomeValue(const std::string *s);

	template<typename T>
	void set(T val);

	template<typename T>
	void safeSet(T val);

	template<typename T>
	T valueAs() const;

	bool operator!=(const SomeValue&) const;
	bool operator==(const SomeValue&) const;

	std::string toString() const;
	friend std::ostream& operator<<(std::ostream& out, const SomeValue& val);
};


std::ostream& operator<<(std::ostream& out, const SomeValue& val);

}

#endif /* SRC_EXPRESSIONS_SOMEVALUE_H_ */
