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

/** \brief A directed connection between compartments.
 *
 * A kappa declared channel. It can be evaluated to obtain several
 * connections between compartment cells. The index expression of every
 * dimension in compartment expressions are solved as a set of integer
 * linear equations on auxiliars using A*x = b. All integer solutions
 * for auxiliars implies a connection between source and target.
 */
class Channel {
	std::string name;
	const CompartmentExpr *source,*target;
	const state::AlgExpression<bool> *filter;
	//std::map<std::string,int*> varValues;
	const state::BaseExpression* delay;

public:
	/** \brief Channel constructor.
	 * @param name the kappa name of this channel.
	 */
	Channel(const std::string &name);
	~Channel();

	/** \brief Setter for source and target compExpressions.
	 * @param src,trgt pointers to compartment expressions.
	 */
	void setCompExpressions(const CompartmentExpr* src, const CompartmentExpr* trgt);

	/** \brief return the name of this channel.
	 */
	const std::string& getName() const;
	/** \brief Set a boolean expression on auxiliars to be a filter for cells.
	 */
	void setFilter(const state::BaseExpression *where);

	void setDelay(const state::BaseExpression *delay);

	void setVarValues(std::map<std::string,int*> &var_values);

	/** \brief Return all the connections between cells from source to target.
	 * This method evaluate all the expressions and validate the internal matrixes
	 * to solve the implicit equation system of auxiliars and return connections.
	 *
	 * @return A list of connections as [ head: source cell | tail: target cell list]
	 *
	 */
	std::list<std::list<int> > getConnections() const;

	//DEBUG methods
	std::string toString(bool show_links = false) const;
	void printConnections(const std::list<std::list<int> >& conn) const;
};

} /* namespace pattern */

#endif /* PATTERN_CHANNEL_H_ */
