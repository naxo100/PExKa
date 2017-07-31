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
#include <set>


using namespace std;
namespace matching {

class Injection {
	map<small_id,big_id> ccAgToNode;//unordered
	mid_id address;
	small_id coordinate;//cc_id

public:
	Injection();
	~Injection();

	const map<small_id,big_id>& getEmbedding() const;

	bool isTrashed() const;
};

typedef set<Injection> InjSet;

} /* namespace simulation */

#endif /* SRC_SIMULATION_INJECTION_H_ */
