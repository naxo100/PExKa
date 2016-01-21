/*
 * main.cpp
 *
 *  Created on: Dec 16, 2015
 *      Author: naxo
 */

//David modifico esto :D

#include "grammar/KappaLexer.h"
#include "grammar/KappaParser.hpp"
#include "grammar/ast/KappaAst.h"
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <vector>


using namespace boost::program_options;

using namespace std;

int main(int argc, char* argv[]){
	const string version("0.1");
	const string v_msg("PExKa "+version);
	const string usage_msg("Simple usage is \n\t$ [mpirun* -np procs] PExKa ([-i] kappa_file)+ [-e events] -t time [-p points] -sync-t tau");

	//Allowed options
	options_description desc(v_msg + "\n" + usage_msg + "\nAllowed options");
	desc.add_options()
		("I,i",value<vector <string> >(),"Kappa files to read model (for backward compatibility with KaSim/PISKa)")
		(",e",value<int>(),"Stop simulation at 'arg' events (negative for unbounded)")
		(",t",value<float>(),"Stop simulation at time 'arg' (arbitrary time unit)")
		(",p",value<int>(),"Number of points in plot files")
		(",o",value<string>(),"Prefix for file name of data outputs (set to 'arg'+'comp_name[index].out')")
		(",d",value<string>(),"Specifies directory where output files should be stored")
		(",load-sim",value<string>(),"Load kappa model from 'arg' instead of kappa files")
		(",make-sim",value<string>(),"Save the kappa model 'arg' from kappa files")
		("implicit-signature","Parser will guess agent signatures automatically")
		(",seed",value<int>(),"Seed for random number generation (default is chosen from time())")
		(",sync-t",value<float>(),"Synchronize compartments every 'arg' simulation-time units")
	    ("version,v", "print version string")
	    ("help,h", "produce help message")
	;

	positional_options_description pos;
	pos.add("I",-1);
	variables_map vm;
	store(command_line_parser(argc,argv).options(desc).positional(pos).run(), vm);
	notify(vm);

	if (vm.count("help")) {
	    cout << desc << "\n";
	    return 1;
	}

	ast::KappaAst ast_model(vm["I"].as<vector<string> >());
	ast_model.parse();
	return 0;
}

