/*
 * Compartment.cpp
 *
 *  Created on: Sep 7, 2016
 *      Author: naxo
 */


#include "../util/Exceptions.h"
#include "Compartment.h"

namespace pattern {

/*****************************************
 *********** Class Compartment ***********
 ****************************************/
int Compartment::TOTAL_CELLS = 0;
Compartment::Compartment(const std::string &nme)
		:name(nme),dimensions(),volume(nullptr) {
	firstCell = TOTAL_CELLS;
	cellsCount = 1;
	TOTAL_CELLS += 1;
}

Compartment::~Compartment() {
	delete volume;
}

void Compartment::setDimensions(const std::vector<short> &dims){
	//TODO first cell?
	TOTAL_CELLS -= cellsCount;
	dimensions = dims;
	cellsCount = 1;
	for(unsigned int i = 0; i < dimensions.size(); i++)
		cellsCount *= dimensions[i];
	TOTAL_CELLS += cellsCount;
}
void Compartment::setVolume(const state::BaseExpression *vol){
	if(volume)
		delete(volume);
	volume = vol;
}

const std::string& Compartment::getName() const {
	return name;
}

void Compartment::getCellIndex(int cell_id,std::vector<short>& index) const{
	if(index.size() != dimensions.size())
		index = std::vector<short>(dimensions.size());
	int factor = cellsCount;
	unsigned i = dimensions.size();
	while(i--){
		factor /= dimensions[i];
		index[i] = cell_id/factor;
		cell_id %= factor;
	}
}

int Compartment::getCellId(const std::vector<short> &cell_index) const{
	int factor = 1, id = 0;
	for(unsigned int i = 0; i < dimensions.size(); i++){
		id += factor*cell_index[i];
		factor *= dimensions[i];
	}
	return id;
}

const std::vector<short>& Compartment::getDimensions() const{
	return dimensions;
}

bool Compartment::nextCell(std::vector<short>& cell) const {
	for(unsigned int i = 0; i < dimensions.size(); i++)
		if(cell[i] < dimensions[i]-1){
			cell[i]++;
			return true;
		}
		else{
			cell[i] = 0;
		}
	return false;
}


using namespace boost::numeric::ublas;

//needed for CompExpression::solve()
 /* Matrix inversion routine.
 Uses lu_factorize and lu_substitute in uBLAS to invert a matrix */
template<class T>
bool InvertMatrix(const matrix<T>& input, matrix<T>& inverse);


/************** CompartmentExpr ***********/

CompartmentExpr::CompartmentExpr(const Compartment& c,const std::list<state::BaseExpression*> &expr)
	: comp(c),cellExpr(expr),b(expr.size())
{
	if(c.getDimensions().size() < cellExpr.size())
		throw SemanticError("Compartment "+comp.getName()+" has not enough dimensions.");
}


void CompartmentExpr::cellIds(std::list<int>& cell_list,
		std::list<short> *cell_values,std::vector<short> &cell_index,unsigned int dim) const{
	if(dim == comp.getDimensions().size())
		cell_list.push_back(comp.getCellId(cell_index));
	else {
		for(std::list<short>::iterator it = cell_values[dim].begin(); it != cell_values[dim].end();it++){
			cell_index[dim] = *it;
			cellIds(cell_list,cell_values,cell_index,dim+1);
		}
	}
}

const Compartment& CompartmentExpr::getCompartment() const{
	return comp;
}

std::list<int> CompartmentExpr::getCells(const std::unordered_map<std::string,int> &var_values) const{
	std::list<int> ret;
	unsigned int dim = 0;
	std::list<short> *cell_values = new std::list<short>[comp.getDimensions().size()];
	for(std::list<state::BaseExpression*>::const_iterator it = cellExpr.cbegin();it != cellExpr.cend();it++){
		try{
			int index = (*it)->getValue(&var_values).iVal;
			if(index < 0 || index >= comp.getDimensions()[dim])
				return ret;
			cell_values[dim].push_back(index);
		}
		catch(const std::out_of_range &e){
			std::unordered_map<std::string,float> factors;
			float b = (*it)->auxFactors(factors);
			for(std::unordered_map<std::string,int>::const_iterator m_it = var_values.cbegin();
					m_it != var_values.cend(); m_it++){
				//maybe if factors[s] = 0 for none then remove '?' TODO
				b -= m_it->second * (factors.count(m_it->first) ? factors[m_it->first] : 0);
				factors.erase(m_it->first);
			}
			if(factors.size() > 1)
				throw std::exception();//Cannot solve this equation
			for(int i = 0; i < comp.getDimensions()[dim];i++){
				float value = (b + i) / (float)factors.begin()->second;
				if(value == (int)value)
					cell_values[dim].push_back(i);
			}
		}
		catch(std::exception &e){
			std::cout << e.what() << std::endl;
		}
		dim++;
	}
	for(unsigned int i = dim; i < comp.getDimensions().size(); i++)
		for(short index = 0; index < comp.getDimensions()[i]; index++)
			cell_values[i].push_back(index);

	//short *cell_index = new short[comp.getDimensions().size()];
	std::vector<short> cell_index(comp.getDimensions().size());

	cellIds(ret,cell_values,cell_index,0);
	delete[] cell_values;
	return ret;
}


void CompartmentExpr::solve(const std::vector<short> &cell_index,std::unordered_map<std::string,int>& var_values) const{
	try{
		vector<short,std::vector<short> > cell(cellExpr.size(),cell_index);
		vector<float> b_;
		if(cellExpr.size() != inverseA.size1())
			b_ = prod(transA,b+cell);
		else
			b_ = (b + cell);
		vector<float> result = prod(inverseA,b_);
		for(unsigned int i = 0; i < result.size(); i++)
			if(result[i] == (int)result[i])
				var_values[varOrder[i]] = result[i];
			else
				throw std::exception();
	}
	catch(std::exception &e){
		//std::cout << "an exception in solve()!!\n" << e.what() << std::endl;
		throw SemanticError("Cannot solve the implicit equation system for this expression.");
	}

}

void CompartmentExpr::setEquation(){
	//get factors, initialize b and var_order
	std::set<std::string> var_names;
	int j = 0;
	auto *var_factors = new std::unordered_map<std::string,float>[cellExpr.size()];
	for(std::list<state::BaseExpression*>::const_iterator it = cellExpr.cbegin();it != cellExpr.cend();it++){
		b[j] = -(*it)->auxFactors(var_factors[j]);
		for(auto var_value : var_factors[j])
			var_names.insert(var_value.first);
		j++;
	}
	varOrder = std::vector<std::string>(var_names.begin(),var_names.end());
	//initialize A
	j = 0;
	matrix<float> A(cellExpr.size(),varOrder.size());
	for(std::list<state::BaseExpression*>::const_iterator it = cellExpr.cbegin();it != cellExpr.cend();it++){
		for(unsigned int i = 0; i < varOrder.size(); i++)
			A(j,i) = var_factors[j][varOrder[i]];
		j++;
	}
	if(A.size1() > A.size2()){//if there are more equations than vars
		transA = matrix<float>(trans(A));
		A = prod(transA,A);
		//b = prod(At,b);
	}
	else if (A.size1() < A.size2())//if there are more vars than eqs
		throw SemanticError("Cannot solve the implicit equation system for this expression.");

	inverseA = matrix<float>(A.size1(),A.size2());
	bool invertible = InvertMatrix(A,inverseA);
	if(!invertible)
		throw SemanticError("Cannot solve the implicit equation system for this expression.");
}


template<class T>
bool InvertMatrix(const matrix<T>& input, matrix<T>& inverse)
{
	typedef permutation_matrix<std::size_t> pmatrix;

	// create a working copy of the input
	matrix<T> A(input);

	// create a permutation matrix for the LU-factorization
	pmatrix pm(A.size1());

	// perform LU-factorization
	int res = lu_factorize(A, pm);
	if (res != 0)
		return false;

	// create identity matrix of "inverse"
	inverse.assign(identity_matrix<T> (A.size1()));

	// backsubstitute to get the inverse
	lu_substitute(A, pm, inverse);

	return true;
}





//DEBUG methods
std::string Compartment::toString() const {
	std::string ret(name);
	for(unsigned int i = 0; i < dimensions.size(); i++)
		ret += std::string("[")+std::to_string(dimensions[i])+"]";
	return ret;
}
std::string Compartment::cellToString(const std::vector<short>& cell){
	std::string ret;
	for(unsigned i = 0; i < cell.size(); i++)
		ret = ret + "[" + std::to_string(cell[i]) + "]";
	return ret;
}
std::string Compartment::cellIdToString(int cell_id) const{
	std::vector<short> cell;
	getCellIndex(cell_id,cell);
	return cellToString(cell);
}

std::string CompartmentExpr::toString() const {
	std::string ret(comp.getName());
	for(std::list<state::BaseExpression*>::const_iterator it = cellExpr.cbegin(); it != cellExpr.cend(); it++)
		break;//ret += it->toString();
	return ret;
}




} /* namespace pattern */
