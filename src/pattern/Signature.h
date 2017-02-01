/*
 * Signature.h
 *
 *  Created on: Aug 10, 2016
 *      Author: naxo
 */

#ifndef SIGNATURE_H_
#define SIGNATURE_H_

#include <string>
#include <vector>
#include <unordered_map>
#include "../state/AlgExpression.h"

namespace ast {
class Id;
}

namespace pattern {

using namespace std;

class Signature {
public:
	class Site;
	class EmptySite;
	class LabelSite;
	template <typename T>
	class RangeSite;
	//class IntRangeSite;
	class Value;

	Signature(const string &name);
	~Signature();

	const string& getName() const;
	//void setId(short id);
	template <typename T>
	Site& addSite(const ast::Id &name);
	short addSite(const ast::Id &name,const vector<string> &labels);
	short addSite(const ast::Id &name,int min,int max);
	short addSite(const ast::Id &name,float min,float max);

	const Site& getSite(const short id) const;
	const Site& getSite(const string &name) const;
	short getSiteId(const string &name) const;
private:
	//short id;
	string name;
	vector<Site*> sites;
	unordered_map<string,short> siteMap;
};

class Signature::Value {
public:
	union {
		const float f;
		const int i;
		const string *s;
		const short id;
	};
	enum {FLOAT,INT,STRING,ID} type;
};

class Signature::Site {
protected:
	string name;

public:
	Site(const string &nme);
	/** \brief Test whether @val is a valid value for this site and
	 * returns id for labeled sites.
	 */
	virtual short isPossibleValue(const state::SomeValue &val) const = 0;
	virtual ~Site();

};

class Signature::EmptySite : public Site {
	virtual short isPossibleValue(const state::SomeValue &val) const override;
public:
	EmptySite(const string &nme);
};
class Signature::LabelSite : public Site {
	vector<string> labels;
	unordered_map<string,short> label_ids;

	virtual short isPossibleValue(const state::SomeValue &val) const override;
public:
	LabelSite(const string &name);
	void addLabel(const ast::Id& name_loc);
};
template <typename T>
class Signature::RangeSite : public Site {
	T min,max,byDefault;
	virtual short isPossibleValue(const state::SomeValue &val) const override;
public:
	RangeSite(const string &nme);
	void setBoundaries(T mn,T mx, T def);
};
/*class Signature::IntRangeSite : Site {
	int min,max;
	virtual bool isPossibleValue(const Value &val) override;
};*/

} /* namespace ast */

#endif /* SIGNATURE_H_ */
