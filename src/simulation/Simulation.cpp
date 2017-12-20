/*
 * Simulation.cpp
 *
 *  Created on: Apr 19, 2016
 *      Author: naxo
 */

#include "Simulation.h"
#include <set>
#include <boost/random.hpp>

namespace simulation {

Simulation::Simulation(pattern::Environment& _env,const vector<state::Variable*>& _vars) : env(_env),vars(_vars),
		params(Parameters::getInstance()), plot(env){
	//ccInjections = new set<matching::Injection*>[env.size<pattern::Mixture::Component>()];
	//mixInjections = new set<matching::Injection*>[env.size<pattern::Mixture>()];
}

Simulation::~Simulation() {
	// TODO Auto-generated destructor stub
	//delete[] ccInjections;
	//delete[] mixInjections;
}

void Simulation::setCells(list<unsigned int>& _cells){
	for(auto cell_id : _cells){
		cells.emplace(piecewise_construct,forward_as_tuple(cell_id),
				forward_as_tuple(env.size<pattern::Token>(),vars,env.getCompartmentByCellId(cell_id).getVolume(),plot,env));
	}
}

void Simulation::initialize(){
	for(auto& id_state : cells){
		id_state.second.initInjections();
		id_state.second.initActTree();
	}
}


void Simulation::run(const Parameters& params){
	//updates
	//TODO while(counter.getTime() < params.limitTime()){
		//calculate Tau-Leaping
			//calculate map [species -> diffusion-to-cells array]
			//map-diffusion-in = scatter map-diffusion-to?
		auto tau = params.limitTime();// = calculate-tau( map-diffusion-in )
		//parallel
		for(auto& id_state : cells){
			id_state.second.advanceUntil(counter.getTime()+tau);
		}
		counter.advanceTime(tau);
	//}
}

//TODO
template <typename T>
list<T> Simulation::allocParticles(unsigned cells,T count,const list<T>* vol_ratios){
	//TODO ....
	return list<T>();
}
template list<float> Simulation::allocParticles<float>(unsigned cells,float count,const list<float>* vol_ratios);


vector<unsigned> Simulation::allocAgents1(unsigned cells,unsigned ag_count,const list<float>* vol_ratios){
	vector<unsigned> allocs(cells,0);
	unsigned div = ag_count/cells;
	unsigned r = ag_count%cells;
	for(unsigned i = 0; i < cells; i++)
		allocs[i] = div;
	for(unsigned i = 0; i < r; i++)
		allocs[i] += 1;
	return allocs;
}

// Allocate agents sampling a multinomial distribution.
vector<unsigned> Simulation::allocAgents2(unsigned cells, unsigned ag_counts, const list<float>* vol_ratios) {

	vector<unsigned> allocs;
	list<float> p_values;

	// Create p_values from *vol_ratios.
	if ( vol_ratios == nullptr )
		p_values.assign(cells, 1.0/cells);
	else {
		float sum_vol_ratios = 0.0;
		for (auto v : *vol_ratios)
			sum_vol_ratios += v;
		for (auto v : *vol_ratios)
			p_values.push_back(v/sum_vol_ratios);
	}

	// Sampling multinomial distribution.
	boost::mt19937 rng;
	unsigned sum_ag = 0;
	float sum_p  = 1.0;
	for (auto p : p_values) {
		boost::binomial_distribution<> distr(ag_counts - sum_ag , p/sum_p);
		boost::variate_generator<boost::mt19937&,
		                         boost::binomial_distribution<> > sampler(rng, distr);
		unsigned s = sampler();
		sum_ag += s;
		sum_p  -= p;

		allocs.push_back(s);
	}

	return allocs;
}

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
void Simulation::addAgents(const Range<int,Args...> &cell_ids,unsigned count,const pattern::Mixture &mix){
	auto per_cell = allocAgents2(cell_ids.size(),count);
	auto ids_it = cell_ids.begin();
	for(auto n : per_cell){
		if(n == 0)
			continue;
		try{
			cells.at(*ids_it).addNodes(n,mix);
		}
		catch(std::out_of_range &e){
			//other mpi_process will add this tokens.
		}
		ids_it++;
	}
}
template void Simulation::addAgents(const std::set<int> &cell_ids,unsigned count,const pattern::Mixture &mix);


/** \brief Return a way to allocate cells among cpu's.
 *
 * @param n_cpus Number of machines or comm_world.size()
 * @param w_vertex Weights for every cell (total reactivity).
 * @param w_edges Weights for every channel (sum of transport reactivity).
 * @param tol Tolerance in the number of processors to be assigned
 * @return a vector with the compartments indexed by ID processor
 */
vector<list<unsigned int>> Simulation::allocCells( int n_cpus,
		const vector<double> &w_vertex,
		const map<pair<int,int>,double> &w_edges,
		int tol) {

	vector<list<unsigned int>> P (n_cpus); //array of indexed compartments by cpu; P[ core ] = { compartments }
	// initialize P
	for( int i = 0 ; i < n_cpus ; i++ )
		P[i] = list<unsigned int>();

	vector<pair<pair<int,int>,double>> ordered_edges = sortEdgesByWeidht(w_edges);
	vector<double> assigned (n_cpus, 0); // assigned and saved edges
	double totalWeight = 0;

	// initialize totalWeight
	for( auto weight : w_vertex )
		totalWeight += weight;

	// assign cores to compartments
	for( auto edge : ordered_edges ) {
		//cout << "viewing " << edge.first.first << " -> " << edge.first.second << " -> " << edge.second << endl;

		int core1 = searchCompartment(P, edge.first.first);
		int core2 = searchCompartment(P, edge.first.second);
		//cout << "edge " << edge.first.first << "," << edge.first.first << endl ;

		if( core1 == -1 && core2 == -1 ) { // if the compartments edge.first.first & edge.first.second don't has assigned core
			unsigned core = minP(P);
			//cout << "core = " << core << endl;

			if( edge.first.first == edge.first.second ){
				P[core].push_back( edge.first.first );
				//assigned[core] += w_vertex[ edge.first.first ] + w_vertex[ edge.first.second ];
				assigned[core] += w_vertex[ edge.first.first ];
			} else {
				P[core].push_back( edge.first.first );
				P[core].push_back( edge.first.second );
				assigned[core] += w_vertex[ edge.first.first ] + w_vertex[ edge.first.second ];
			}

		} else if( core1 == -1 ) { // if edge.first.first has been assigned
			//cout << "k = " << kSecond << endl;

			if( assigned[core2] < (double)totalWeight / n_cpus + tol ) {
				P[core2].push_back( edge.first.first );
				assigned[core2] += w_vertex[ edge.first.first ];
			}

		} else if( core2 == -1 ) {
			//cout << "k = " << kFirst << endl;

			if( assigned[core1] < (double)totalWeight / n_cpus + tol ) {
				P[core1].push_back( edge.first.second );
				assigned[core1] += w_vertex[ edge.first.second ];
			}

		}
	}
	// post processing
	//find no assigned compartments
	for(size_t i = 0;  i < w_vertex.size() ; i++ ) {
		int core = searchCompartment(P, i);
		if( core == -1 ) {
			core = minP(P);
			P[core].push_back( i );
			assigned[core] += w_vertex[ i ];
		}
	}

	// showing P
	cout << "showing assignment of vertex" << endl;
	for( unsigned i = 0 ; i < P.size() ; i++ ) {
		cout << "P[" << i << "] = { " ;
		for ( auto p : P[i] ) {
			cout << p << ",";
		}
		cout << " }" << endl;
	}
	cout << "end showing" << endl;

	return P;
}



/** \brief Sort edges by weight from lowest to highest
 *  @param w_edges edges with weight
 */
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


unsigned Simulation::minP( vector<list<unsigned int>> P ) {
	unsigned k = 0;
	for ( unsigned i = 0; i < P.size() ; i++ ) {
		for ( unsigned j = 0 ; j < P.size() ; j++ ) {
			if( P[i].size() < P[j].size() )
				k = i; // save the minus size
		}
	}
	return k;
}

/** \brief Search if a compartment was assigned to a core
 * @param assigned assigned compartment list
 * @param c compartment to search
 */
int Simulation::searchCompartment(vector<list<unsigned int>> assigned, unsigned c ) {
	for( int i = 0 ; i < (int)assigned.size() ; i++ ) {
		for ( auto p : assigned[i] ) {
			if( p == c )
				return i;
		}
	}

	return -1;
}

void Simulation::print() const {
	cout << cells.size() << " cells in this simulation object." << endl;
	for(auto& id_state : cells){
		cout << env.cellIdToString(id_state.first) << endl;
		id_state.second.print();
	}
}

} /* namespace state */
