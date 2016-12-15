/*
 * Channel.cpp
 *
 *  Created on: Sep 7, 2016
 *      Author: naxo
 */

#include "Channel.h"


namespace pattern {

Channel::Channel(const std::string &nme,const CompartmentExpr* src,
		const CompartmentExpr* trgt)
	: name(nme),source(src),target(trgt),filter(nullptr) {}

Channel::~Channel() {
	// TODO Auto-generated destructor stub
}


const std::string& Channel::getName() const {
	return name;
}


std::list< std::list< int > > Channel::getConnections() const{
	std::vector<short> cell_index(source->getCompartment().getDimensions().size());
	std::unordered_map<std::string,int> var_map;
	std::list< std::list<int> > ret;
	do{
		//std::cout << Compartment::cellToString(cell_index) << source.getCompartment().getCellId(cell_index) << std::endl;
		std::list<int> l;
		l.push_back(source->getCompartment().getCellId(cell_index));
		try{
			source->solve(cell_index,var_map);
		}catch(std::exception &e){
			continue;
		}
		//for(auto &elem : var_map)
		//	std::cout << elem.first << " -> " << elem.second << std::endl;
		auto l2 = target->getCells(var_map);
		if(l2.size() == 0)
			continue;
		l.insert(l.end(),l2.begin(),l2.end());
		ret.push_back(l);
	}
	while(source->getCompartment().nextCell(cell_index));
	return ret;
}


void Channel::setFilter(const state::BaseExpression *where){
	if(filter)
		delete filter;
	filter = dynamic_cast<const state::AlgExpression<bool>* >(where);;
}
//TODO
void Channel::setVarValues(std::map<std::string,int*> &var_values){}



//DEBUG methods
std::string Channel::toString(bool show_links) const{
	std::string ret(name);
	ret += ": from " + source->toString() + " to " + target->toString();
	return ret;
}

void Channel::printConnections(const std::list<std::list<int> >& l) const{
	for(std::list<std::list<int> >::const_iterator l_it = l.cbegin();l_it != l.cend(); l_it++){
		std::cout << source->getCompartment().cellIdToString(l_it->front()) << "  ->\t";
		for(std::list<int>::const_iterator int_it = ++(l_it->cbegin());int_it != l_it->cend(); int_it++)
			std::cout << target->getCompartment().cellIdToString(*int_it) << "\t";
		std::cout << std::endl;
	}
}



} /* namespace pattern */
