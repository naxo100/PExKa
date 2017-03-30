/*
 * Simulation.cpp
 *
 *  Created on: Apr 19, 2016
 *      Author: naxo
 */

#include "Simulation.h"
#include <set>
namespace simulation {

Simulation::Simulation() {
	// TODO Auto-generated constructor stub

}

Simulation::~Simulation() {
	// TODO Auto-generated destructor stub
}


//TODO
template <typename T>
list<T> Simulation::allocParticles(unsigned cells,T count,const list<T>* vol_ratios){
	//TODO ....
	return list<T>();
}
template list<float> Simulation::allocParticles<float>(unsigned cells,float count,const list<float>* vol_ratios);



template <template<typename,typename...> class Range,typename... Args>
void Simulation::addTokens(const Range<int,Args...> &cell_ids,float count,short token_id){
	list<float> per_cell = allocParticles(cell_ids.size(),count);
	auto ids_it = cell_ids.begin();
	for(auto n : per_cell){
		try{
			cells.at(*ids_it).addTokens(token_id,n);
		}
		catch(std::out_of_range &e){
			//other mpi_process will add this tokens.
		}
		ids_it++;
	}
}
template void Simulation::addTokens(const std::set<int> &cell_ids,float count,short token_id);


template <template<typename,typename...> class Range,typename... Args>
void Simulation::addAgents(const Range<int,Args...> &cell_ids,unsigned count,pattern::Mixture &mix){
	list<int> per_cell = allocParticles(cell_ids.size(),count);
	auto ids_it = cell_ids.begin();
	for(auto n : per_cell){
		try{
			cells.at(*ids_it).addAgents(mix,n);
		}
		catch(std::out_of_range &e){
			//other mpi_process will add this tokens.
		}
		ids_it++;
	}
}


vector<list<int>> Simulation::allocCells(
		int n_cpus,
		const vector<double> &w_vertex,
		const map<pair<int,int>,double> &w_edges,
		int tol
) {

	vector<list<int>> P (n_cpus);      // array of indexed compartments by cpu's
	// initialize P
	for( int i = 0 ; i < n_cpus ; i++ ) P[i] = list<int>();
	vector<pair<pair<int,int>,double>> ordered_edges = sortEdgesByWeidht(w_edges);
	vector<double> assigned (n_cpus, 0); // assigned and saved edges
	double totalWeight = 0;
	// initialize totalWeight
	for( auto weight : w_vertex ) totalWeight += weight;


	// assign cores to compartments
	for( auto edge : ordered_edges ) {
		//cout << "viewing " << edge.first.first << "," << edge.first.second << " -> " << edge.second << endl;

		int kFirst = searchCompartment(P, edge.first.first);
		int kSecond = searchCompartment(P, edge.first.second);

		if( searchCompartment(P, edge.first.first) == -1 && searchCompartment(P, edge.first.second) == -1 ) {
			unsigned k = minP(P);
			//cout << "k = " << k << endl;

			P[k].push_back( edge.first.first );
			P[k].push_back( edge.first.second );
			assigned[k] += w_vertex[ edge.first.first ] + w_vertex[ edge.first.second ];

		} else if( kFirst == -1 ) {
			//cout << "k = " << kSecond << endl;

			if( assigned[kSecond] < (double)totalWeight / n_cpus + tol ) {
				P[kSecond].push_back( edge.first.first );
				assigned[kSecond] += w_vertex[ edge.first.first ];
			}

		} else if( kSecond == -1 ) {
			//cout << "k = " << kFirst << endl;

			if( assigned[kFirst] < (double)totalWeight / n_cpus + tol ) {
				P[kFirst].push_back( edge.first.second );
				assigned[kFirst] += w_vertex[ edge.first.second ];
			}

		}

		//TODO missing part of v2


		// show P
		/*for( unsigned i = 0 ; i < P.size() ; i++ ) {
			cout << "P[" << i << "] = { " ;
			for ( auto p : P[i] ) {
				cout << p << ",";
			}
			cout << " }" << endl;
		}*/
	}

	return P;
}


vector< pair< pair<int,int>, double > > Simulation::sortEdgesByWeidht( const map<pair<int,int>,double> &w_edges ) {
	// function to sort edges
	struct LocalSort {
		bool operator()( pair<pair<int,int>,float> a, pair<pair<int,int>,float> b ) {
			return a.second < b.second;
		}
	} f;

	vector< pair< pair<int,int>, double> > ordered_edges;

	// save edge
	if( ordered_edges.empty() ) {
		for( auto &edge : w_edges ) {
			ordered_edges.push_back( edge );
		}
	}

	sort( ordered_edges.begin(), ordered_edges.end(), f );
	return ordered_edges;
}


unsigned Simulation::minP( vector<list<int>> P ) {
	unsigned k = 0;
	for ( unsigned i = 0; i < P.size() ; i++ ) {
		for ( unsigned j = 0 ; j < P.size() ; j++ ) {
			if( P[i].size() < P[j].size() ) k = i; // save the minus size
		}
	}
	return k;
}


int Simulation::searchCompartment(vector<list<int>> assigned, unsigned c ) {
	for( int i = 0 ; i < (int)assigned.size() ; i++ ) {
		for ( auto p : assigned[i] ) {
			if( p == (int)c ) return i;
		}
	}

	return -1;
}

} /* namespace state */
