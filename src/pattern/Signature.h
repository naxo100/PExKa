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
	class FloatRangeSite;
	class IntRangeSite;
	class Value;

	Signature();
	~Signature();

	short addSite(const string &name);
	short addSite(const string &name,const vector<string> &labels);
	short addSite(const string &name,int min,int max);
	short addSite(const string &name,float min,float max);

	const Site& getSite(const short id) const;
	const Site& getSite(const string &name) const;
private:
	int id;
	string name;
	vector<Site*> sites;
	unordered_map<string,short> site_ids;
};

class Value {
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
	virtual bool isPossibleValue(const Value &val) = 0;
	virtual ~Site();

};

class Signature::EmptySite : Site {
	virtual bool isPossibleValue(const Value &val) override;
};
class Signature::LabelSite : Site {
	vector<string> labels;
	unordered_map<string,short> label_ids;

	virtual bool isPossibleValue(const Value &val) override;
};
class Signature::FloatRangeSite : Site {
	float min,max;
	virtual bool isPossibleValue(const Value &val) override;
};
class Signature::IntRangeSite : Site {
	int min,max;
	virtual bool isPossibleValue(const Value &val) override;
};

} /* namespace ast */

#endif /* SIGNATURE_H_ */
