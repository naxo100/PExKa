/*
 * Counter.cpp
 *
 *  Created on: Apr 19, 2016
 *      Author: naxo
 */

#include "Counter.h"

namespace simulation {

Counter::Counter() : time(0.),events(0),null_events(0),cons_null_events(0),
		perturbation_events(0),null_actions(0),stat_null({}),zero_reactivity(false),
		initialized(false),dT(.0),dE(.0),next_sync_at(-1.),max_time(-1.),max_events(0),
		sync_count(0),sync_time(0),ticks(0),init_event(0),diffusion_ev(0),reaction_ev(0),
		init_time(0),progress_step(0),stop(false),sync_event_dt(.0){}

Counter::~Counter() {}

FL_TYPE Counter::getTime() const{
	return time;
}

UINT_TYPE Counter::getEvent() const{
	return events;
}
UINT_TYPE Counter::getNullEvent() const{
	return null_events;
}
UINT_TYPE Counter::getProdEvent() const{
	return events - null_events;
}

void Counter::advanceTime(FL_TYPE t){
	time += t;
}
void Counter::incEvents(){
	events++;
	cons_null_events = 0;
}
void Counter::incNullEvents(unsigned type){
	stat_null[type]++;
	null_events++;
	cons_null_events++;
}
void Counter::incNullActions(UINT_TYPE n){
	null_actions += n;
}

string Counter::toString() const {
	string ret;
	ret += "Simulated time: " + to_string(time) + "\tEvents: " + to_string(events);
	ret += "\nNull-Events: " + to_string(null_events) +
			" ["+to_string(stat_null[0])+","+to_string(stat_null[1])+","+
			to_string(stat_null[2])+","+to_string(stat_null[3])+","+to_string(stat_null[4])+"]\n";
	ret += "Null-actions = " + to_string(null_actions);
	return ret;
}





LocalCounter::LocalCounter() : Counter() {}


GlobalCounter::GlobalCounter() : Counter() {}



} /* namespace pattern */
