/*
 * Exceptions.h
 *
 *  Created on: Aug 16, 2016
 *      Author: naxo
 */

#ifndef UTIL_EXCEPTIONS_H_
#define UTIL_EXCEPTIONS_H_

#include <exception>
#include <string>
#include "../grammar/location.hh"

using namespace std;

class SemanticError : public exception {
	string msg;
	yy::location loc;
public:
	SemanticError(const string &str,const yy::location &loc = yy::location());
	//~SemanticError();

	virtual const char* what() const _GLIBCXX_USE_NOEXCEPT override;
	void setLocation(const yy::location &l);
};


#endif /* UTIL_EXCEPTIONS_H_ */
