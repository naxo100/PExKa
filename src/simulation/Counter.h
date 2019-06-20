/*
 * Counter.h
 *
 *  Created on: Apr 19, 2016
 *      Author: naxo
 */

#ifndef SIMULATION_COUNTER_H_
#define SIMULATION_COUNTER_H_

#include <iostream>
#include <utility>
#include "../util/params.h"

namespace state{
	class State;
}

namespace simulation {

class Plot;

using namespace std;

class Counter {
	friend class state::State;
	friend class Plot;

	FL_TYPE time;
	UINT_TYPE events;
	UINT_TYPE null_events;
	UINT_TYPE cons_null_events;
	UINT_TYPE perturbation_events;
	UINT_TYPE null_actions;
	pair<UINT_TYPE,FL_TYPE> last_tick;
	bool initialized;
	unsigned int ticks;
	unsigned int stat_null[6];
	FL_TYPE init_time;
	UINT_TYPE init_event;
	FL_TYPE max_time;
	UINT_TYPE max_events;
	int dE;
	FL_TYPE dT;
	bool stop;

	uint progress_step;
	/* [events|react_evts|diff_evts|nl_evts|cns_nl_evts|pert_evts|nl_action] */
	UINT_TYPE total_events[7];
	UINT_TYPE reaction_ev;
	UINT_TYPE diffusion_ev;

	uint sync_count;
	FL_TYPE next_sync_at;
	FL_TYPE sync_time;
	FL_TYPE sync_event_dt;
	bool zero_reactivity;
public:
	Counter();
	virtual ~Counter();

	virtual FL_TYPE getTime() const;
	virtual UINT_TYPE getEvent() const;
	virtual UINT_TYPE getNullEvent() const;
	virtual UINT_TYPE getProdEvent() const;
	virtual void advanceTime(FL_TYPE t);
	virtual void incEvents();
	virtual void incNullEvents(unsigned type);
	virtual void incNullActions(UINT_TYPE n = 1);

	virtual string toString() const;

};


class LocalCounter : public Counter {
	//friend class State;
public:
	LocalCounter();
	//~LocalCounter();
};

class GlobalCounter : public Counter {

public:
	GlobalCounter();
};


} /* namespace simulation */

#endif /* SIMULATION_COUNTER_H_ */
