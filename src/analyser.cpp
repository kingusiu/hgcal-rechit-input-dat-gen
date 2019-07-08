/*
 * analyser.cpp
 *
 *  Created on: 07 May 2019
 *      Author: kiwoznia
 */

#include <string>
#include <iostream>
#include <utility>
#include "interface/analyser.h"
#include "interface/RechitConverter.h"
#include "interface/SimclusterConverter.h"
#include "interface/WindowEtaPhi.h"


void analyser::analyze(size_t childid /* this info can be used for printouts */){

	std::cout << "+++ reading input file " << getSampleFile() << std::endl;
	
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
	 d_ana::tBranchHandler<std::vector<float> > rechit_z(tree(),"rechit_z");
	 d_ana::tBranchHandler<std::vector<int> > rechit_detid(tree(),"rechit_detid");
	 // simcluster data
	 d_ana::tBranchHandler<std::vector <std::vector<int>> > in_simcluster_hits_idx(tree(), "simcluster_hits_indices");
	 d_ana::tBranchHandler<std::vector <std::vector<float>> > in_simcluster_frac(tree(), "simcluster_fractions");
	 d_ana::tBranchHandler<std::vector<float> > in_simcluster_eta(tree(),"simcluster_eta");
	 d_ana::tBranchHandler<std::vector<float> > in_simcluster_phi(tree(),"simcluster_phi");


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
	//if(isTestMode()) nevents/=100;

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
		RechitConverter rechitConv = RechitConverter( rechit_energy.content(), rechit_x.content(), rechit_y.content(), rechit_z.content(), rechit_detid.content(), rechit_phi.content(), rechit_eta.content() );

		// read in simcluster features and sort by eta
		SimclusterConverter simclusConv = SimclusterConverter( in_simcluster_eta.content(), in_simcluster_phi.content(), in_simcluster_hits_idx.content(), in_simcluster_frac.content() );

		// get hits in window
		std::vector<int> hit_idx_in_simclusters = simclusConv.getHitIndicesBelongingToClusters();
		double window_margin = 0.05;
		WindowEtaPhi winEtaPhi( &hit_idx_in_simclusters, window_margin, rechitConv.eta(), rechitConv.phi() );
		std::vector<int> hit_indices_in_eta_phi_window = winEtaPhi.getHitIndicesInEtaPhiWindow( &hit_idx_in_simclusters, rechitConv.eta(), rechitConv.phi() ); 

		for( int hit_idx : hit_indices_in_eta_phi_window ){ // for each rechit (get number of rechits from energy feature)

			// put together all features for rechit "hit_idx"
			std::vector<float> rechit_features = rechitConv.getFeaturesForHit( hit_idx );
			out_rechit.push_back(rechit_features);

			// get simcluster indices and their respective fractions for rechit "hit_idx"
			std::pair<std::vector<int>, std::vector<float>> simcluster_indices_and_fractions = simclusConv.getClusterIdxAndFracForHit( hit_idx ); 

			out_simcluster_indices.push_back( simcluster_indices_and_fractions.first );
			out_simcluster_frac.push_back( simcluster_indices_and_fractions.second );

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



