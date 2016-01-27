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

	positional_options_description pos;
	pos.add("input-file",-1);
	variables_map vm;
	store(command_line_parser(argc,argv).options(desc).positional(pos).run(), vm);
	notify(vm);

	if (vm.count("help")) {
	    cout << desc << "\n";
	    return 1;
	}

	grammar::KappaDriver *ast_model;
	if (vm.count("input-file"))
		ast_model = new grammar::KappaDriver(vm["input-file"].as<vector<string> >());
	else
		ast_model = new grammar::KappaDriver();

	ast_model->parse();

	delete ast_model;
	return 0;
}

