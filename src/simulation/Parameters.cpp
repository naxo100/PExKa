/*
 * Parameters.cpp
 *
 *  Created on: Aug 25, 2017
 *      Author: naxo
 */

#include "Parameters.h"

#include <limits>

namespace simulation {

Parameters Parameters::singleton;

Parameters::Parameters() : maxEvent(std::numeric_limits<UINT_TYPE>::max()),
		maxTime(/*std::numeric_limits<FL_TYPE>::infinity()*/0),points(1),
		outputFile("sim.out"),options(nullptr),seed(0){}

void Parameters::makeOptions(const string &msg){
	options = new options_description(msg);

	//allowed options
	options->add_options()
		("input-file,i",value<vector <string> >(),"Kappa files to read model (for backward compatibility with KaSim/PISKa)")
		("events,e",value<int>(),"Stop simulation at 'arg' events (negative for unbounded)")
		("time,t",value<float>(),"Stop simulation at time 'arg' (arbitrary time unit)")
		("points,p",value<int>(),"Number of points in plot files")
		("out,o",value<string>(),"Prefix for file name of data outputs (set to 'arg'+'comp_name[index].out')")
		("dir,d",value<string>(),"Specifies directory where output files should be stored")
		("load-sim,l",value<string>(),"Load kappa model from 'arg' instead of kappa files")
		("make-sim,m",value<string>(),"Save the kappa model 'arg' from kappa files")
		("implicit-signature","Parser will guess agent signatures automatically")
		("seed,s",value<int>(),"Seed for random number generation (default is chosen from time())")
		("sync-t",value<float>(),"Synchronize compartments every 'arg' simulation-time units")
		("version,v", "print version string")
		("help,h", "produce help message")
	;
}

void Parameters::evalOptions(int argc, char* argv[]){
	positional_options_description pos;
	pos.add("input-file",-1);
	variables_map vm;
	try {
		store(command_line_parser(argc,argv).options(*options).positional(pos).run(), vm);
		notify(vm);
	} catch(boost::program_options::error_with_option_name &e){
		cerr << e.what() << endl;
		cout << "Try PExKa --help for the list of valid program options." << endl;
		exit(0);
	}

	if (vm.count("help")) {
		cout << *options << "\n";
		exit(0);
	}

	if (vm.count("input-file"))
		inputFiles = vm["input-file"].as<vector<string> >();
	else {
		cout << "Try PExKa --help for the list of valid program options." << endl;
		exit(1);
	}

	if(vm.count("seed")){
		auto val = vm["seed"].as<int>();
		cout << "seed was predefined to value " << val << "." << endl;
		seed = val;
	}
	else{
		auto val = time(nullptr);
		cout << "seed was randomized to value " << int(val) << "." << endl;
		seed = val;
	}

	if(vm.count("time")){
		maxTime = vm["time"].as<float>();
		if(vm.count("events"))
			cout << "ignoring given max-event option." << endl;
	}
	else if(vm.count("events"))
		maxEvent = vm["events"].as<int>();
	else{
		cout << "No max-events or max-time given. Aborting..." << endl;
		exit(0);
	}
	if(vm.count("points"))
		points = vm["points"].as<int>();
	else
		cout << "No points to plot." << endl;
	if(vm.count("out"))
		outputFile = vm["out"].as<string>();
	if(vm.count("dir"))
		outputFile = vm["dir"].as<string>() + "/" + outputFile;

}

Parameters::~Parameters() {}


const Parameters& Parameters::get(){
	return singleton;
}

/*
UINT_TYPE Parameters::limitEvent() const{
	return maxEvent;
}
FL_TYPE Parameters::limitTime() const{
	return maxTime;
}
unsigned Parameters::plotPoints() const {
	return points;
}
const string& Parameters::outputFile() const {
	return file;
}*/

} /* namespace simulation */
