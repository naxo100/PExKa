/*
 * State.cpp
 *
 *  Created on: Mar 23, 2016
 *      Author: naxo
 */

#include "State.h"

namespace state {

State::State(size_t tok_count,const std::vector<Variable*>& _vars,const state::BaseExpression& vol) :
	volume(vol),vars(_vars),tokens (new float[tok_count]){}

State::~State() {
	delete[] tokens;
}


void State::addTokens(float n,short tok_id){
	tokens[tok_id] += n;
}

void State::addNodes(int n,pattern::Mixture& mix){

}

} /* namespace ast */
