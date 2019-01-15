/*
 * main.cpp
 *
 *  Created on: Dec 16, 2015
 *      Author: naxo
 */

//David modifico esto :D
//new modification

#include "grammar/KappaLexer.h"
#include "grammar/KappaParser.hpp"
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <vector>
#include "grammar/KappaDriver.h"
#include "grammar/ast/KappaAst.h"
#include "pattern/Environment.h"
#include "simulation/Simulation.h"
#include "simulation/Parameters.h"
#include "util/Warning.h"


using namespace boost::program_options;

using namespace std;

int main(int argc, char* argv[]){

	const string version("0.1");
	const string v_msg("PExKa "+version);
	const string usage_msg("Simple usage is \n$ [mpirun* -np procs] PExKa ([-i] kappa_file)+ [-e events] -t time [-p points] -sync-t tau");

	for(int i = 0; i < argc; i++) cout << argv[i] << " | "; cout << endl;

	//Allowed options
	options_description desc(v_msg + "\n" + usage_msg + "\n\nAllowed options");
	desc.add_options()
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

	// parser kappa file
	positional_options_description pos;
	pos.add("input-file",-1);
	variables_map vm;
	try {
		store(command_line_parser(argc,argv).options(desc).positional(pos).run(), vm);
		notify(vm);
	} catch(boost::program_options::error_with_option_name &e){
		cerr << e.what() << endl;
		cout << "Try PExKa --help for the list of valid program options." << endl;
		return 1;
	}

	if (vm.count("help")) {
	    cout << desc << "\n";
	    return 1;
	}

	grammar::KappaDriver *driver;
	if (vm.count("input-file"))
		driver = new grammar::KappaDriver(vm["input-file"].as<vector<string> >());
	else {
		cout << "Try PExKa --help for the list of valid program options." << endl;
		return 1;
	}

	//parser Kappa file
	try{
		driver->parse();
	} catch(const exception &e) {
		cerr << "A parser error found: " << e.what() << endl;
		exit(1);
	}
	// make AST
	ast::KappaAst &ast = driver->getAst();
	//ast.show();
	//cout << "\n\n" ;

	// initialize states
	pattern::Environment env;
	vector<Variable*> vars;
	try{
		ast.evaluateDeclarations(env,vars,true);//constants
		ast.evaluateCompartments(env,vars);
		ast.evaluateUseExpressions(env,vars);
		ast.evaluateSignatures(env,vars);
		ast.evaluateDeclarations(env,vars,false);//vars
		ast.evaluateChannels(env,vars);
		ast.evaluateRules(env,vars);
	}
	catch(const exception &e){
		cerr << "An exception found: " << e.what() << endl;
		exit(1);
	}
	vector<simulation::Rule> rules = env.getRules();
	for(unsigned int i=0; i < rules.size(); i++){
		cout << "rate: " << rules[i].getRate().toString() << endl;
	}
#ifdef DEBUG
	//env.show();
#endif
	WarningStack::getStack().show();

	auto& params = simulation::Parameters::getInstance();
	if(vm.count("time")){
		params.maxTime = vm["time"].as<float>();
		if(vm.count("events"))
			cout << "ignoring given max-event option" << endl;
	}
	else if(vm.count("events"))
		params.maxEvent = vm["events"].as<int>();
	else{
		cout << "No max-events or max-time given. Aborting..." << endl;
		exit(0);
	}
	if(vm.count("points"))
		params.points = vm["points"].as<int>();
	else
		cout << "No points to plot." << endl;
	if(vm.count("out"))
		params.file = vm["out"].as<string>();
	if(vm.count("dir"))
		params.file = vm["dir"].as<string>() + "/" + params.file;

	map<pair<int,int>,double> edges;
	for(size_t i = 0; i < env.size<pattern::Channel>(); i++ ){
		for(auto& channel : env.getChannels(i)){
			for(auto cells : channel.getConnections()){
				int src_id = cells.front();
				cells.pop_front();
				for(auto trgt_id : cells){
					edges[pair<int,int>(src_id,trgt_id)] = 1.0;
				}
			}
		}
	}
	//cout << "total cells: " << pattern::Compartment::getTotalCells() << endl;
	//for(auto edge : edges)
	//	cout << edge.first.first << "->" << edge.first.second << ": " << edge.second << endl;
	auto cells = simulation::Simulation::allocCells(1,vector<double>(pattern::Compartment::getTotalCells(),1.0),edges,-1);
	simulation::Simulation sim(env,vars);
	//for(auto i : cells[0])
	//	cout << i << ", ";
	//cout << endl;
	sim.setCells(cells[0]);
	try{
		ast.evaluateInits(env,vars,sim);
	}
	catch(const exception &e){
		cerr << "An exception found: " << e.what() << endl;
		exit(1);
	}
	if(env.getRules().size() < 1){
		cout << "No rules to execute a simulation. Aborting." << endl;
		return 0;
	}
	sim.initialize();
	//sim.print();

	try{
		sim.run(params);
	}catch(exception &e){
		cerr << "An exception found: " << e.what() << endl;
		exit(1);
	}

	//sim.print();

	cout << "finished!" << endl;

	/* TODO
	 * Environment env = ast.evaluateGlobals();
	 * MPI::Bcast((void*)&env,Nbytes,MPI::Datatype.PACKED, 0);
	 * env = ast.evaluateEnvironment(env,my_rank);
	 * State state = ast.evaluateState(env,my_rank);
	 *
	 * state.run(env) ?
	 */

	delete driver;
	return 0;
}
