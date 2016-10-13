/*
 * Mixture.h
 *
 *  Created on: Apr 26, 2016
 *      Author: naxo
 */

#ifndef PATTERN_MIXTURE_H_
#define PATTERN_MIXTURE_H_

#include <unordered_map>
#include <vector>
#include <utility> //pair
#include <string>


using namespace std;

namespace pattern {

class Mixture {
public:
	struct Site;
	struct Agent;
	enum LinkType {WILD,BIND,FREE,BIND_TO,PATH};
	Mixture();
	~Mixture();

	short addAgent(short name_id);
	short addSite(short mix_ag, short name_id);
	void setSiteValue(short mix_site,short label);
	void setSiteValue(short mix_site,int val);
	void setSiteValue(short mix_site,float val);

	void setSiteLink(short mix_site,LinkType l);


private:
	vector<Agent> agents;
	short sitesCount;
	//std::unordered_map< std::pair<short,short> , std::pair<short,short> > graph;

};

struct Mixture::Site {
	LinkType link_type;
	union {
		int int_value;
		float float_value;
		short id_value;
	} state;
	short path_id;
	std::pair<short,short> lnk;
};

struct Mixture::Agent {
	int name;
	std::unordered_map<int,Site> interface;
};

} /* namespace pattern */

#endif /* PATTERN_MIXTURE_H_ */
