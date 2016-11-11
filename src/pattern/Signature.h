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

	void setId(short id);

	short addSite(const string &name);
	short addSite(const string &name,const vector<string> &labels);
	short addSite(const string &name,int min,int max);
	short addSite(const string &name,float min,float max);

	const Site& getSite(const short id) const;
	const Site& getSite(const string &name) const;
private:
	short id;
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
	string name;

public:
	Site(const string &nme);
	virtual bool isPossibleValue(const Value &val) = 0;
	virtual ~Site();

};

class Signature::EmptySite : public Site {
	virtual bool isPossibleValue(const Value &val) override;
public:
	EmptySite(const string &nme);
};
class Signature::LabelSite : public Site {
	vector<string> labels;
	unordered_map<string,short> label_ids;

	virtual bool isPossibleValue(const Value &val) override;
public:
	LabelSite(const string &name,const vector<string> &labs);
};
template <typename T>
class Signature::RangeSite : Site {
	T min,max,byDefault;
	virtual bool isPossibleValue(const Value &val) override;
public:
	RangeSite(const string &nme,const T mn,const T mx);
	RangeSite(const string &nme,const T mn,const T mx,const T df);
};
/*class Signature::IntRangeSite : Site {
	int min,max;
	virtual bool isPossibleValue(const Value &val) override;
};*/

} /* namespace ast */

#endif /* SIGNATURE_H_ */
