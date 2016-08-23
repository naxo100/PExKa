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


namespace pattern {

class Mixture {
public:
	struct Site;
	struct Agent;
	Mixture();
	~Mixture();
private:
	std::vector<Agent> agents;
	short sitesCount;
	//std::unordered_map< std::pair<short,short> , std::pair<short,short> > graph;

};

struct Mixture::Site {
	enum {WILD,BIND,FREE,BIND_TO,PATH} link_type;
	union {
		int int_value;
		float float_value;
		int id_value;
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
