/*
 * analyser.cpp
 *
 *  Created on: 07 May 2019
 *      Author: kiwoznia
 */

#include <string>
#include <iostream>
#include <numeric>
#include <utility>
#include "interface/analyser.h"
#include "interface/RechitConverter.h"
#include "interface/SimclusterConverter.h"

/**
 * sorts vector A according to the order of values in another vector B
 * return : sorted A
 **/
template< typename T, typename U >
std::vector<T> sortVecAByVecB( std::vector<T> & a, std::vector<U> & b ){

	std::vector<std::pair<T,U>> zipped(a.size());
	for( size_t i = 0; i < a.size(); i++ ) zipped[i] = std::make_pair( a[i], b[i] ); // zip the two vectors (A,B)

	std::sort(zipped.begin(), zipped.end(), []( auto & lop, auto & rop ) { return lop.second < rop.second; }); // sort according to B

	std::vector<T> sorted;
	std::transform(zipped.begin(), zipped.end(), std::back_inserter(sorted), []( auto & pair ){ return pair.first; }); // extract sorted A

	return sorted;
}

template< typename T >
void analyser::copyInputVecToOutputVec( d_ana::tBranchHandler<std::vector<T>> & in_vec, std::vector<T> & out_vec ){

	std::vector<T> * in_vec_content = in_vec.content();
	out_vec = *in_vec_content;
}

template void analyser::copyInputVecToOutputVec<float>( d_ana::tBranchHandler<std::vector<float>> & in_vec, std::vector<float> & out_vec );
template void analyser::copyInputVecToOutputVec<int>( d_ana::tBranchHandler<std::vector<int>> & in_vec, std::vector<int> & out_vec );


void analyser::analyze(size_t childid /* this info can be used for printouts */){

	std::cout << "+++ reading input file " << getSampleFile() << std::endl;

	std::vector<std::string> feature_names_rechit = {"rechit_energy", "rechit_eta", "rechit_phi", "rechit_x", "rechit_y", "rechit_detid"};
	std::vector<std::string> feature_names_simcluster = {"simcluster_hits_indices", "simcluster_fractions", "simcluster_eta" };

	
	/* ==SKIM==
	 *
	 * If a skim of the Delphes outout was created in a way indicated
	 * further below, use the tBranchHandler (please notice the "t")
	 * to access vectors of objects...
	 *
	 */
	 // rechit data
	 d_ana::tBranchHandler<std::vector<float> > rechit_energy(tree(),"rechit_energy");
	 d_ana::tBranchHandler<std::vector<float> > rechit_eta(tree(),"rechit_eta");
	 d_ana::tBranchHandler<std::vector<float> > rechit_phi(tree(),"rechit_phi");
	 d_ana::tBranchHandler<std::vector<float> > rechit_x(tree(),"rechit_x");
	 d_ana::tBranchHandler<std::vector<float> > rechit_y(tree(),"rechit_y");
	 d_ana::tBranchHandler<std::vector<int> > rechit_detid(tree(),"rechit_detid");
	 // simcluster data
	 d_ana::tBranchHandler<std::vector <std::vector<int>> > in_simcluster_hits_idx(tree(), "simcluster_hits_indices");
	 d_ana::tBranchHandler<std::vector <std::vector<float>> > in_simcluster_frac(tree(), "simcluster_fractions");
	 d_ana::tBranchHandler<std::vector<float> > in_simcluster_eta(tree(),"simcluster_eta");

	/*
	 * If (optionally) a skim or a flat ntuple is to be created, please use the following function to initialize
	 * the tree.
	 * The output files will be written automatically, and a config file will be created.
	 */
	TTree* myskim=addTree();
	/*
	 * Or store a vector of objects (also possible to store only one object)
	 */
	// rechit attribute outputs
	std::vector< std::vector<float> > out_rechit;
	myskim->Branch("rechit_features", &out_rechit);
	// simcluster attribute output
	std::vector<std::vector<int>> out_simcluster_indices; // save simcluster id as index appearing in simcluster_* arrays of root file
	myskim->Branch("simcluster_indices", &out_simcluster_indices);
	std::vector<std::vector<float>> out_simcluster_frac;
	myskim->Branch("simcluster_fractions", &out_simcluster_frac);


	size_t nevents=tree()->entries();
	if(isTestMode())
		nevents/=100;
	for(size_t eventno=0;eventno<nevents;eventno++){

		out_rechit.clear();
		out_simcluster_indices.clear();
		out_simcluster_frac.clear();

		/*
		 * The following two lines report the status and set the event link
		 * Do not remove!
		 */
		reportStatus(eventno,nevents);
		tree()->setEntry(eventno); // all data (energy, eta, ...) of all registered in vectors for the event read in here

		// read in rechit features
		RechitConverter rechitConv = RechitConverter( rechit_energy.content(), rechit_x.content(), rechit_y.content(), rechit_detid.content(), rechit_phi.content(), rechit_eta.content() );
		// read in simcluster features
		SimclusterConverter simclusConv = SimclusterConverter( in_simcluster_eta.content(), in_simcluster_hits_idx.content(), in_simcluster_frac.content() );

		// read in simcluster data
		std::vector<float> simcluster_eta = *in_simcluster_eta.content();
		std::vector<std::vector<int>> simcluster_hits_idx = *in_simcluster_hits_idx.content(); // for each simcluster get the hits that it produced
		std::vector<std::vector<float>> simcluster_frac = *in_simcluster_frac.content();

		// sort simcluster data by eta
		std::vector<std::vector<int>> simcluster_hits_idx_sorted = sortVecAByVecB( simcluster_hits_idx, simcluster_eta );
		std::vector<std::vector<float>> simcluster_frac_sorted = sortVecAByVecB( simcluster_frac, simcluster_eta );

		int simcluster_num = simcluster_hits_idx.size(); 
		//std::cout << simcluster_hits_idx->size() << " simclusters found" << std::endl;

		for( int hit_idx = 0; hit_idx < rechitConv.numRechits(); hit_idx++){ // get number of rechits from energy feature

			// put together all features for rechit "hit_idx"
			std::vector<float> rechit_features = rechitConv.getRechitFeatures( hit_idx );
			out_rechit.push_back(rechit_features);

			//std::pair<std::vector<int>, std::vector<float>> simcluster_indices_and_fractions = simclusConv.getClusterIdxAndFracForHit( int hit_idx ); 

			out_simcluster_indices.push_back(std::vector<int>());
			out_simcluster_frac.push_back(std::vector<float>(simcluster_num, 0.0));

			// go through each simcluster and look for hit 'hit_idx'
			for( int cluster_idx = 0; cluster_idx < simcluster_num; cluster_idx++ ) {
	
				auto hits_in_cluster = simcluster_hits_idx_sorted.at(cluster_idx);
				//std::cout << "number of hits in cluster " << cluster_idx << ": " << hits_in_cluster.size() << std::endl;
				auto found = std::find( hits_in_cluster.begin(), hits_in_cluster.end(), hit_idx );

				if( found != hits_in_cluster.end() ){
					//std::cout << hit_idx << " found!" << std::endl;
					out_simcluster_indices[hit_idx].push_back(cluster_idx); // store cluster id to hit
					int frac_idx = found - hits_in_cluster.begin();
					float frac = simcluster_frac_sorted.at(cluster_idx).at(frac_idx);
					out_simcluster_frac[hit_idx].at( cluster_idx ) = frac;// store fraction of cluster to this hit's fraction vector
				}
			}
		}

		myskim->Fill();

	}

	/*
	 * Must be called in the end, takes care of thread-safe writeout and
	 * call-back to the parent process
	 */
	processEndFunction();
}



void analyser::postProcess(){ }



