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

class Parameters {
	friend class Counter;
	friend int ::main(int argc, char* argv[]);
	static Parameters singleton;

	Parameters();
	void makeOptions(const string &msg);
	void evalOptions(int argc, char* argv[]);
public:
	UINT_TYPE maxEvent;
	FL_TYPE maxTime;
	unsigned points;
	long seed;

	vector<string> inputFiles;
	string outputFile;
	string outputFileType;
	string outputDirectory;
	options_description *options;

	int runs;

	~Parameters();
	static const Parameters& get();

	//UINT_TYPE limitEvent() const;
	//FL_TYPE limitTime() const;
	//unsigned plotPoints() const;
	//const string& outputFile() const;
};

} /* namespace simulation */

#endif /* SRC_SIMULATION_PARAMETERS_H_ */
