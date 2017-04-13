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


class SemanticError : public std::exception {
	char msg[150];
	yy::location loc;
public:
	SemanticError(const std::string &str,const yy::location& loc);

	//~SemanticError();

	virtual const char* what() const _GLIBCXX_USE_NOEXCEPT override;

	void setLocation(const yy::location &l);
};


class SyntaxError : public std::exception {
	char msg[150];
	yy::location loc;
public:
	SyntaxError(const std::string &str,const yy::location& loc);

	//~SyntaxError();

	virtual const char* what() const _GLIBCXX_USE_NOEXCEPT override;

	void setLocation(const yy::location &l);
};

#endif /* UTIL_EXCEPTIONS_H_ */
