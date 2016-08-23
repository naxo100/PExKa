/*
 * Spatial.cpp
 *
 *  Created on: May 12, 2016
 *      Author: naxo
 */

#include "Spatial.h"

namespace ast {

/****** Class CompExpression *********/
CompExpression::CompExpression(){}
CompExpression::CompExpression(const Id &id,const list<const Expression*> &dim,const location &l):
	Node(l),name(id), index(dim),where(NULL) {};
CompExpression::CompExpression(const Id &id,const list<const Expression*> &dim,const Expression* w,const location &l):
	Node(l),name(id), index(dim),where(w) {};



} /* namespace ast */
