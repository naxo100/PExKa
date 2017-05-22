/*
 * Injection.h
 *
 *  Created on: Apr 11, 2017
 *      Author: naxo
 */

#ifndef SRC_MATCHING_INJECTION_H_
#define SRC_MATCHING_INJECTION_H_

#include "../util/params.h"
#include <map>


using namespace std;
namespace matching {

class Injection {
	map<small_id,big_id> ccAgToNode;//unordered
	mid_id address;
	pair<small_id,small_id> coordinate;//mix_id,cc_id

public:
	Injection();
	~Injection();
};

} /* namespace simulation */

#endif /* SRC_SIMULATION_INJECTION_H_ */
