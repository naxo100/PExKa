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


using namespace boost::program_options;

using namespace std;

int main(int argc, char* argv[]){
	const string version("0.1");
	const string v_msg("PExKa "+version);
	const string usage_msg("Simple usage is \n$ [mpirun* -np procs] PExKa ([-i] kappa_file)+ [-e events] -t time [-p points] -sync-t tau");

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
	store(command_line_parser(argc,argv).options(desc).positional(pos).run(), vm);
	notify(vm);

	if (vm.count("help")) {
	    cout << desc << "\n";
	    return 1;
	}

	grammar::KappaDriver *driver;
	if (vm.count("input-file"))
		driver = new grammar::KappaDriver(vm["input-file"].as<vector<string> >());
	else
		driver = new grammar::KappaDriver();

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
		//ast.evaluateInits(env,vars);
	}
	catch(const exception &e){
		cerr << "An exception found: " << e.what() << endl;
		exit(1);
	}

	env.show();

	simulation::Simulation sim;

	// 5 vertices, 6 aristas
	map<pair<int,int>,double> w_edges;
	w_edges[ make_pair(1,3) ] = 1;
	w_edges[ make_pair(3,1) ] = 0.3;
	w_edges[ make_pair(3,4) ] = 0.7;
	w_edges[ make_pair(4,2) ] = 0.1;
	w_edges[ make_pair(4,5) ] = 0.5;
	w_edges[ make_pair(5,3) ] = 0.4;

	// 5 vertices
	vector<double> w_vertex(5,0.5);
	w_vertex[0] = 1;

	int n_cpus = 2;

	sim.allocCells(n_cpus, w_vertex, w_edges, 0);


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

