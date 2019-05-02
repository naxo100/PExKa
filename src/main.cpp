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



#include <ctime>
#include <random>

using namespace boost::program_options;

using namespace std;

int main(int argc, char* argv[]){

	const string version("0.1");
	const string v_msg("PExKa "+version);
	const string usage_msg("Simple usage is \n$ [mpirun* -np procs] PExKa ([-i] kappa_file)+ [-e events] -t time [-p points] -sync-t tau");

	for(int i = 0; i < argc; i++) cout << argv[i] << " | "; cout << endl;


	auto& params = simulation::Parameters::singleton;

	params.makeOptions(v_msg + "\n" + usage_msg + "\n\nAllowed options");
	params.evalOptions(argc, argv);

	// parser kappa file
	grammar::KappaDriver *driver;
	driver = new grammar::KappaDriver(params.inputFiles);


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

	WarningStack::getStack().show();


#ifdef DEBUG
	env.show();
#endif

	sim.print();
	try{
		sim.run(params);
	}catch(exception &e){
		cerr << "An exception found: " << e.what() << endl;
		exit(1);
	}

	sim.print();

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
