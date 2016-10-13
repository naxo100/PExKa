/*
 * Channel.h
 *
 *  Created on: Sep 7, 2016
 *      Author: naxo
 */

#ifndef PATTERN_CHANNEL_H_
#define PATTERN_CHANNEL_H_

#include <string>
#include <vector>
#include <list>
#include <map>
#include "Compartment.h"
#include "../state/AlgExpression.h"

namespace pattern {

class Channel {
	std::string name;
	const Compartment &source,&target;
	const std::list<state::BaseExpression*> srcExpr,trgtExpr;
	const state::AlgExpression<bool> *filter;
	std::map<std::string,int*> varValues;


public:
	Channel(const std::string &name,const state::BaseExpression* where,
			const Compartment &src_comp,const Compartment &trgt_comp,
			const std::list<state::BaseExpression*> &src,const std::list<state::BaseExpression*> &dest);
	~Channel();

	void setVarValues(std::map<std::string,int*> &var_values);
};

} /* namespace pattern */

#endif /* PATTERN_CHANNEL_H_ */
