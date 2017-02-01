/*
 * Mixture.h
 *
 *  Created on: Apr 26, 2016
 *      Author: naxo
 */

#ifndef PATTERN_MIXTURE_H_
#define PATTERN_MIXTURE_H_

#include <map>
#include <unordered_map>
#include <vector>
#include <list>
#include <utility> //pair
#include <string>


using namespace std;

namespace pattern {

class Environment;

typedef pair<short,short> id_pair;

struct CompareIdPair{
	bool operator()(id_pair p1,id_pair p2);
};
//bool f(id_pair p1,id_pair p2);

class Mixture {
public:
	struct Site;
	class Agent;
	class Component;
	enum LinkType {WILD,BIND,FREE,BIND_TO,PATH};
	enum ValueType {VOID,LABEL,INT_VAL,FLOAT_VAL};
	Mixture(short agent_count);
	~Mixture();

	short addAgent(const Agent* a);
	void addLink(const id_pair &first,const id_pair &second);
	void setComponents(Environment &env);


	string toString(const Environment &env) const;


private:
	union {
		const Agent** agents;
		const Component** comps;
	};
	short agentCount;
	short siteCount;
	short compCount;
	std::map< id_pair , id_pair ,CompareIdPair > graph;

};

//bool (*f)(pair<short,short>,pair<short,short>) = [](pair<short,short>& p1,pair<short,short>& p2) {return p1.first < p2.first ? p1 : (p1.second < p2.second ? p1 : p2 );};


struct Mixture::Site {
	ValueType val_type;
	union {
		int int_value;
		float float_value;
		short id_value;
		string* lbl;
	} state;
	//short path_id;
	LinkType link_type;
	std::pair<short,short> lnk;//agent,site

	bool operator==(const Site &s) const;

	//Site();
};

class Mixture::Agent {
	short signId;
	std::unordered_map<int,Site> interface;

public:
	Agent(short name_id);

	short getId() const;
	Site& addSite(short name_id);
	void setSiteValue(short mix_site,short label);
	void setSiteValue(short mix_site,int val);
	void setSiteValue(short mix_site,float val);

	void setSiteLink(short mix_site,LinkType l);

	const Site& getSite(short id) const;
	bool operator==(const Agent& a) const;
};

class Mixture::Component {
	vector<const Agent*> agents;
	map<id_pair,id_pair> graph;

public:
	Component();
	Component(const Component& comp);
	bool contains(const Mixture::Agent* a);
	short addAgent(const Mixture::Agent* a);
	void addLink(const pair<id_pair,id_pair> &lnk,const map<short,short> &mask);

	size_t size() const;

	bool operator==(const Mixture::Component &m) const;
};

} /* namespace pattern */

#endif /* PATTERN_MIXTURE_H_ */
