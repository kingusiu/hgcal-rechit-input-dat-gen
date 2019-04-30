/*
 * analyser.cpp
 *
 *  Created on: 24 Aug 2016
 *      Author: jkiesele
 */

#include <string>
#include <iostream>
#include <algorithm>
#include "interface/analyser.h"


template< typename T >
void analyser::copyInputVecToOutputVec( d_ana::tBranchHandler<std::vector<T>> & in_vec, std::vector<T> & out_vec ){

	std::vector<T> * in_vec_content = in_vec.content();

	for( unsigned int i = 0; i < in_vec_content->size(); i++ ){
		
		out_vec.push_back( in_vec_content->at(i) );

	}
}

template void analyser::copyInputVecToOutputVec<float>( d_ana::tBranchHandler<std::vector<float>> & in_vec, std::vector<float> & out_vec );
template void analyser::copyInputVecToOutputVec<int>( d_ana::tBranchHandler<std::vector<int>> & in_vec, std::vector<int> & out_vec );


void analyser::analyze(size_t childid /* this info can be used for printouts */){

	std::vector<std::string> feature_names_rechit = {"rechit_energy", "rechit_eta", "rechit_phi", "rechit_x", "rechit_y", "rechit_detid"};
	std::vector<std::string> feature_names_simcluster = {"simcluster_hits_indices", "simcluster_fractions" };

	
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
	std::vector<float> out_rechit_energy;
	myskim->Branch("rechit_energy", &out_rechit_energy);
	std::vector<float> out_rechit_eta;
	myskim->Branch("rechit_eta", &out_rechit_eta);
	std::vector<float> out_rechit_phi;
	myskim->Branch("rechit_phi", &out_rechit_phi);
	std::vector<float> out_rechit_x;
	myskim->Branch("rechit_x", &out_rechit_x);
	std::vector<float> out_rechit_y;
	myskim->Branch("rechit_y", &out_rechit_y);
	std::vector<int> out_rechit_detid;
	myskim->Branch("rechit_detid", &out_rechit_detid);
	// simcluster attribute output
	std::vector<std::vector<int>> out_simcluster_indices; // save simcluster id as index appearing in simcluster_* arrays of root file
	myskim->Branch("simcluster_indices", &out_simcluster_indices);
	std::vector<std::vector<float>> out_simcluster_frac;
	myskim->Branch("simcluster_fractions", &out_simcluster_frac);


	size_t nevents=tree()->entries();
	if(isTestMode())
		nevents/=100;
	for(size_t eventno=0;eventno<nevents;eventno++){

		out_rechit_energy.clear();
		out_rechit_eta.clear();
		out_rechit_phi.clear();
		out_rechit_x.clear();
		out_rechit_y.clear();
		out_rechit_detid.clear();		
		out_simcluster_indices.clear();
		out_simcluster_frac.clear();

		// read in simcluster data
		std::vector<std::vector<int>> * simcluster_hits_idx = in_simcluster_hits_idx.content();
		std::vector<std::vector<float>> * simcluster_frac = in_simcluster_frac.content();

		//std::cout << simcluster_hits_idx->size() << " simclusters found" << std::endl;

		for( int hit_idx = 0; hit_idx < rechit_energy.content()->size(); hit_idx++){ // get number of rechits from energy feature

			out_simcluster_indices.push_back(std::vector<int>());
			out_simcluster_frac.push_back(std::vector<float>());

			// go through each simcluster and look for hit 'hit_idx'
			for( int cluster_idx = 0; cluster_idx < simcluster_hits_idx->size(); cluster_idx++ ) {
	
				auto hits_in_cluster = simcluster_hits_idx->at(cluster_idx);
				//std::cout << "number of hits in cluster " << cluster_idx << ": " << hits_in_cluster.size() << std::endl;
				auto found = std::find( hits_in_cluster.begin(), hits_in_cluster.end(), hit_idx );

				if( found != hits_in_cluster.end() ){
					//std::cout << hit_idx << " found!" << std::endl;
					out_simcluster_indices[hit_idx].push_back(cluster_idx); // store cluster id to hit
					int frac_idx = found - hits_in_cluster.begin();
					float frac = simcluster_frac->at(cluster_idx).at(frac_idx);
					out_simcluster_frac[hit_idx].push_back( frac );// store fraction to hit
				}
			}
		}

		

		/*
		 * The following two lines report the status and set the event link
		 * Do not remove!
		 */
		reportStatus(eventno,nevents);
		tree()->setEntry(eventno);

		copyInputVecToOutputVec(rechit_energy, out_rechit_energy);
		copyInputVecToOutputVec(rechit_eta, out_rechit_eta);
		copyInputVecToOutputVec(rechit_phi, out_rechit_phi);
		copyInputVecToOutputVec(rechit_x, out_rechit_x);
		copyInputVecToOutputVec(rechit_y, out_rechit_y);
		copyInputVecToOutputVec(rechit_detid, out_rechit_detid);

		myskim->Fill();



		/*==SKIM==
		 * Access the branches of the skim
		 */
		//std::vector<Electron> * skimelecs=electrons.content();
		//for(size_t i=0;i<skimelecs->size();i++){
		//	histo->Fill(skimelecs->at(i).PT);
		//}
	}


	/*
	 * Must be called in the end, takes care of thread-safe writeout and
	 * call-back to the parent process
	 */
	processEndFunction();
}



void analyser::postProcess(){
	/*
	 * This function can be used to analyse the output histograms, e.g. extract a signal contribution etc.
	 * The function can also be called directly on an output file with the histograms, if
	 * RunOnOutputOnly = true
	 * is set in the analyser's config file
	 *
	 * This function also represents an example of how the output of the analyser can be
	 * read-back in an external program.
	 * Just include the sampleCollection.h header and follow the procedure below
	 *
	 */

	/*
	 * Here, the input file to the extraction of parameters from the histograms is the output file
	 * of the parallelised analysis.
	 * The sampleCollection class can also be used externally for accessing the output consistently
	 */
	d_ana::sampleCollection samplecoll;
	samplecoll.readFromFile(getOutPath());

	std::vector<TString> alllegends = samplecoll.listAllLegends();

	/*
	 * Example how to process the output.
	 * Usually, one would define the legendname of the histogram to be used here
	 * by hand, e.g. "signal" or "background".
	 * To make this example run in any case, I am using alllegends.at(0), which
	 * could e.g. be the signal legend.
	 *
	 * So in practise, the following would more look like
	 * samplecoll.getHistos("signal");
	 */
	if(false /*alllegends.size()>0*/){
				
		d_ana::histoCollection histos=samplecoll.getHistos(alllegends.at(0));

		/*
		 * here, the histogram created in the analyze() function is selected and evaluated
		 * The histoCollection maintains ownership (you don't need to delete the histogram)
		 */
		const TH1* myplot=histos.getHisto("histoname1");

		std::cout << "(example output): the integral is " << myplot->Integral() <<std::endl;

		/*
		 * If the histogram is subject to changes, please clone it and take ownership
		 */

		TH1* myplot2=histos.cloneHisto("histoname1");

		/*
		 * do something with the histogram
		 */

		delete myplot2;
	}

	/*
	 * do the extraction here.
	 */



}



