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

class NullEvent : public std::exception {
	/* 0: not a null event
	 * 1:unary rule with binary instance
	 * 2:binary rule with unary instance
	 * 3:clashing instance
	 * 4:overapproximation clash
	 * 5:invalid injection clash
	 * 6: perturbation interrupting time */
public:
	const int error;
	NullEvent(int e);
	virtual const char* what() const _GLIBCXX_USE_NOEXCEPT override;
};

#endif /* UTIL_EXCEPTIONS_H_ */
