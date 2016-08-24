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
	class LabeledSite;
	class FloatRangeSite;
	class IntRangeSite;
	union Value {
		float f;
		int i;
		string s;
	};
	Signature();
	~Signature();
private:
	int id;
	string name;
	vector<Site*> sites;
	unordered_map<string,short> site_ids;
};


class Signature::Site {
	string name;

public:
	virtual bool isPossibleValue(Value val) = 0;
	virtual ~Site();

};

class Signature::EmptySite : Site {
	virtual bool isPossibleValue(Value val) override;
};
class Signature::LabeledSite : Site {
	string labels;
	unordered_map<string,short> label_ids;

	virtual bool isPossibleValue(Value val) override;
};
class Signature::FloatRangeSite : Site {
	float min,max;
	virtual bool isPossibleValue(Value val) override;
};
class Signature::IntRangeSite : Site {
	int min,max;
	virtual bool isPossibleValue(Value val) override;
};

} /* namespace ast */

#endif /* SIGNATURE_H_ */
