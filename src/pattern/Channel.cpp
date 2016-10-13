/*
 * Channel.cpp
 *
 *  Created on: Sep 7, 2016
 *      Author: naxo
 */

#include "Channel.h"


namespace pattern {

Channel::Channel(const std::string &nme,const state::BaseExpression* where,
		const Compartment &src_comp,const Compartment &trgt_comp,
		const std::list<state::BaseExpression*> &src,const std::list<state::BaseExpression*> &dest)
	: name(nme),source(src_comp),target(trgt_comp),srcExpr(src),trgtExpr(dest)
{
	filter = dynamic_cast<const state::AlgExpression<bool>* >(where);

}

Channel::~Channel() {
	// TODO Auto-generated destructor stub
}

} /* namespace pattern */
