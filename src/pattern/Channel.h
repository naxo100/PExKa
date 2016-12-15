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
#include <utility>
#include "Compartment.h"
#include "../state/AlgExpression.h"

namespace pattern {

class Channel {
	std::string name;
	const CompartmentExpr &source,&target;
	const state::AlgExpression<bool> *filter;
	std::map<std::string,int*> varValues;


public:
	Channel(const std::string &name,
			const CompartmentExpr& src, const CompartmentExpr& trgt);
	~Channel();

	const std::string& getName() const;
	void setFilter(const state::BaseExpression *where);
	void setVarValues(std::map<std::string,int*> &var_values);


	std::list<std::list<int> > getConnections() const;

	//DEBUG methods
	std::string toString(bool show_links = false) const;
	void printConnections(const std::list<std::list<int> >& conn) const;
};

} /* namespace pattern */

#endif /* PATTERN_CHANNEL_H_ */
