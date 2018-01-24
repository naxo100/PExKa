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
#include "../util/params.h"

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
	small_id addSite(const ast::Id &name,const vector<string> &labels);
	small_id addSite(const ast::Id &name,int min,int max);
	small_id addSite(const ast::Id &name,FL_TYPE min,FL_TYPE max);

	const Site& getSite(const small_id id) const;
	const Site& getSite(const string &name) const;
	const vector<Site*>& getSites() const;
	short getSiteId(const string &name) const;

	short_id getId() const;
	small_id getSiteCount() const;
	void setId(short_id id);
private:
	short_id id;
	string name;
	vector<Site*> sites;
	unordered_map<string,small_id> siteMap;
};

class Signature::Value {
public:
	union {
		const FL_TYPE f;
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
	virtual bool isPossibleValue(const state::SomeValue &val) const = 0;
	virtual state::SomeValue getDefaultValue() const = 0;
	virtual ~Site();

	const string& getName() const;

};

class Signature::EmptySite : public Site {
	virtual bool isPossibleValue(const state::SomeValue &val) const override;
public:
	EmptySite(const string &nme);
	state::SomeValue getDefaultValue() const override;
};

class Signature::LabelSite : public Site {
	vector<string> labels;
	unordered_map<string,small_id> label_ids;

	virtual bool isPossibleValue(const state::SomeValue &val) const override;
public:
	LabelSite(const string &name);
	void addLabel(const ast::Id& name_loc);
	const string& getLabel( small_id id ) const;
	small_id getLabelId(const string& s) const;
	state::SomeValue getDefaultValue() const override;
};

template <typename T>
class Signature::RangeSite : public Site {
	T min,max,byDefault;
	virtual bool isPossibleValue(const state::SomeValue &val) const override;
public:
	RangeSite(const string &nme);
	void setBoundaries(T mn,T mx, T def);
	state::SomeValue getDefaultValue() const override;
};

//class Token {};

/*class Signature::IntRangeSite : Site {
	int min,max;
	virtual bool isPossibleValue(const Value &val) override;
};*/

} /* namespace ast */

#endif /* SIGNATURE_H_ */
