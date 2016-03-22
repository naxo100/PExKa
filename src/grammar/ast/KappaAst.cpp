/*
 * KappaAst.cpp
 *
 *  Created on: Jan 27, 2016
 *      Author: naxo
 */

#include "KappaAst.h"

namespace ast {

KappaAst::KappaAst() {
	// TODO Auto-generated constructor stub

}

KappaAst::~KappaAst() {
	// TODO Auto-generated destructor stub
}


void KappaAst::add(Declaration d){
	decls.push_back(d);
}

/*void KappaAst::add(Perturbation p){
	perts.push_back(p);
}*/

} /* namespace ast */
