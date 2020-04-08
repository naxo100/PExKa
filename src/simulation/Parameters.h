/*
 * Parameters.h
 *
 *  Created on: Aug 25, 2017
 *      Author: naxo
 */

#ifndef SRC_SIMULATION_PARAMETERS_H_
#define SRC_SIMULATION_PARAMETERS_H_

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <vector>

#include "Counter.h"

int main(int argc, char* argv[]);

namespace simulation {

using namespace boost::program_options;


/** \brief Store and evaluate given program arguments.
 *
 * Singleton class to store and evaluate the arguments given by
 * command line. This class is globally accessible for reading
 * using the singleton instance (static method get()). Only
 * friend method main() can access to non-const methods.
 *
 */
class Parameters {
	//friend class Counter;
	friend int ::main(int argc, char* argv[]); ///< Easier manipulation of Parameters from main.

	static Parameters singleton; //!< the singleton instance of the class

	/** Constructor to initialize default parameters.
	 */
	Parameters();
	/** \brief Constructs the boost::option_description object to evaluate program arguments.
	 */
	void makeOptions(const string &msg);

	/** \brief Set simulation parameters from arguments vector.
	 */
	void evalOptions(int argc, char* argv[]);
public:
	options_description *options;	//!< boost object to manipulate program options. Created on makeOptions() call.

	vector<string> inputFiles;		//!< Kappa files of the the model.
	string outputFile;				//!< Output file name. Default is "output". PExKa will add a simulation number if needed.
	string outputFileType;			//!< Type of the output file. Default is ".tsv".
	string outputDirectory;			//!< Output directory. Default is ".".

	UINT_TYPE maxEvent;	//!< Finish the simulation after this number of successful events.
	FL_TYPE maxTime;	//!< Finish the simulation after this amount of simulated time.
	unsigned points;	//!< Number of points (lines) in the output trajectory file.
	long seed;			//!< Seed for the RNG of the simulation(s). Default is random (from time).

	bool useMultiNode;	//!< If true, equal agents will be stored in one node. Default is false.


	int runs;			//!< Number of simulations to run with the same model.

	~Parameters();
	static const Parameters& get();	//!< Return the singleton instance of the class

};

} /* namespace simulation */

#endif /* SRC_SIMULATION_PARAMETERS_H_ */
