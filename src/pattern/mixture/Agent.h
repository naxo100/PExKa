/*
 * Agent.h
 *
 *  Created on: Jan 21, 2019
 *      Author: naxo100
 */

#ifndef SRC_PATTERN_MIXTURE_AGENT_H_
#define SRC_PATTERN_MIXTURE_AGENT_H_

#include "Mixture.h"
#include "../../grammar/location.hh"

namespace pattern {

/** \brief Class of an agent declared in a mixture.
 * Stores all site information of a kappa declared
 * agent in a mixture.
 */
class Pattern::Agent {
	short_id signId; //signature ID
	yy::location loc;
	std::map<small_id,Site> interface;//TODO using map because of possible dependency between sites.
	//map<small_id,list<Agent*> > parents;
	//map<small_id,list<Agent*> > childs;
	mutable list<pair<const Mixture::Component*,small_id> > includedIn;

public:
	Agent(short_id name_id);
	~Agent();

	void setLoc(const yy::location& loc);
	const yy::location& getLoc() const;

	short_id getId() const;
	/** \brief Includes the site with id name_id in this agent.
	 * The new site is added to a map with the default values,
	 * like it has no declared value or link status. i.e.
	 * (small_id)-1 and FREE.
	 */
	Site& addSite(small_id name_id);
	void setSiteValue(small_id mix_site,small_id label);
	void setSiteValue(small_id mix_site,int val);
	void setSiteValue(small_id mix_site,FL_TYPE val);
	void setSiteExprValue(small_id mix_site,const expressions::BaseExpression* expr);
	void setSiteAuxPattern(small_id mix_site,expressions::BaseExpression** vals);
	//void setSiteMinExpr(small_id mix_site,const expressions::BaseExpression* expr);
	//void setSiteMaxExpr(small_id mix_site,const expressions::BaseExpression* expr);

	/*not optimized*/
	map<small_id,string> getAuxNames() const;


	//void setSiteLink(short mix_site,LinkType l);
	void setLinkPtrn(small_id trgt_site,small_id ag_ptrn,small_id site_ptrn);

	const Site& getSite(small_id id) const;
	const Site& getSiteSafe(small_id id) const;
	bool operator==(const Agent& a) const;

	/** \brief Test if this agent is "contained" in 'a'.
	 * to_test stores site-ids of every bind that will need to be tested
	 */
	bool testEmbed(const Agent& a,list<small_id>& to_test) const;

	/*void addParent(small_id id,Agent *a);
	void addParents(small_id id,const list<Agent*>& la);
	void addChild(small_id id,Agent *a);
	void addChilds(small_id id,const list<Agent*>& la);
	const list<Agent*>& getParentPatterns(small_id id) const;
	const list<Agent*>& getChildPatterns(small_id id) const;
	const map<small_id,list<Agent*> >& getParentPatterns() const;
	const map<small_id,list<Agent*> >& getChildPatterns() const;*/

	void addCc(const Mixture::Component* cc,small_id id) const;

	//return a list of CCs where this agent-pattern is included (and its own position in CC)
	const list<pair<const Mixture::Component*,small_id> >& getIncludes() const;

	const map<small_id,Site>::const_iterator begin() const;
	const map<small_id,Site>::const_iterator end() const;

	string toString( const Environment& env, short mixAgId=-1,
			map<ag_st_id,short> bindLabels = map<ag_st_id,short>() ) const;
};



/** \brief Structure of an agent site of a mixture.
 * It stores the site value and link state of kappa
 * declared site of an agent mixture.
 */
struct Pattern::Site {
	const static small_id EMPTY = small_id(-1);
	const static small_id AUX = small_id(-2);
	const static small_id EXPR = small_id(-3);
	small_id label;//small_id(-2) -> aux

	LinkType link_type;

	//valid on BIND and BIND_TO
	ag_st_id lnk_ptrn;//agent_id,site (-1,-1)

	//string aux_id;//if "" then no aux
	const expressions::BaseExpression* values[3];//[smaller=,value,greater=]

	/** \brief Constructs an empty site.
	 * Default state value is (small_id)-1 and
	 * default link_type is FREE.
	 */
	Site(small_id _label = EMPTY,LinkType ltype = FREE);
	Site(expressions::SomeValue _val,LinkType ltype = FREE);
	Site(const Site& site);
	~Site();

	//Site& operator=(const Site& s);
	bool operator==(const Site &s) const;
	bool testEmbed(const Site &s,list<small_id>& to_test) const;
	int compareLinkPtrn(ag_st_id ptrn) const;
	bool isEmptySite() const;
	bool isExpression() const;
	bool isBindToAny() const;
	bool testValue(const expressions::SomeValue& val,
			const VarVector& vars) const;
	bool testValueOpt(const expressions::SomeValue& val,
			const state::State& state,
			const expressions::AuxMap& aux_map) const;
	bool testLink(const pair<const void*,small_id>& lnk) const;
};

template <typename T>
class RangedSite : public Pattern::Site {
	T max,min;

public:
	bool operator==(const Site &s) const;
	int compare(const Site &s) const;
	bool isEmptySite() const;//TODO inline???
	bool isBindToAny() const;
};


} /* namespace pattern */

#endif /* SRC_PATTERN_MIXTURE_AGENT_H_ */
