/*
 * Plot.cpp
 *
 *  Created on: Dec 12, 2017
 *      Author: naxo
 */

#include "Plot.h"
#include "../pattern/Environment.h"

namespace simulation {

Plot::Plot(const pattern::Environment& env,int run_id) : nextPoint(0.),dT(0.) {
	auto& params = Parameters::get();
	try {
		if(params.runs > 1){
			char file_name[100],buff[20];
			sprintf(buff,"%%s-%%%dd.%%s",int(log10(params.runs-1))+1);
			sprintf(file_name,buff,params.outputFile.c_str(),run_id,params.outputFileType.c_str());
			cout << buff << "\n" << file_name << endl;
			file.open(file_name,ios::out);
		}
		else{
			if(params.outputFile.find(".") && params.outputFileType == "csv")//not nice
				file.open(params.outputFile.c_str(),ios::out);
			else
				file.open(params.outputFile+"."+params.outputFileType,ios::out);
		}
	}
	catch(...){
		cout << "error opening output file." << endl;
	}
	dT = params.maxTime / params.points;
	if(dT == 0.0)
		dT = 0.0001;
	file << "#Time";
	for(auto obs : env.getObservables())
		file << "\t" << obs->toString();
	file << endl;
}

Plot::~Plot() {
	file.close();
}


void Plot::fill(const state::State& state,const pattern::Environment& env) {
	auto t = min(state.getCounter().getTime(),state.getCounter().next_sync_at);
	while(t >= nextPoint){
		file << nextPoint;
		for(auto var : env.getObservables()){
			file << "\t" << var->getValue(state);
		}
		file << endl;
		nextPoint += dT;
	}
}


} /* namespace simulation */
