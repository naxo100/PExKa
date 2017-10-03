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
public:

	virtual ~Injection() = 0;
	virtual const map<small_id,big_id>& getEmbedding() const = 0;

	virtual bool isTrashed() const = 0;

	virtual void codomain(big_id *injs[],set<big_id> cod) const = 0;

	bool operator< (const Injection& inj) const;
};

class CcInjection : Injection {
	//map<small_id,big_id> ccAgToNode;//unordered
	big_id* ccAgToNode;
	mid_id address;
	small_id coordinate;//cc_id

public:
	CcInjection(small_id size);
	~CcInjection();

	const map<small_id,big_id>& getEmbedding() const override;

	bool isTrashed() const override;

	void codomain(big_id *injs[],set<big_id> cod) const override;
};

class MixInjection : Injection {
	big_id** agCcIdToNode;
	mid_id address;
	small_id coordinate;//mix_id

public:
	MixInjection(small_id size);
	~MixInjection();

	const map<small_id,big_id>& getEmbedding() const override;

	bool isTrashed() const override;

	void codomain(big_id *injs[],set<big_id> cod) const override;
};

//TODO choose the best implementation of InjSet
class InjSet : public set<Injection*> {

public:
	const Injection& chooseRandom() const;
};

} /* namespace simulation */

#endif /* SRC_SIMULATION_INJECTION_H_ */
