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
CompExpression::CompExpression(const location &l,const Id &id,const list<const Expression*> &dim):
	Node(l),name(id), index(dim),where(NULL) {};
CompExpression::CompExpression(const location &l,const Id &id,const list<const Expression*> &dim,const Expression* w):
	Node(l),name(id), index(dim),where(w) {};



} /* namespace ast */
