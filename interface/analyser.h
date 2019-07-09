/*
 * analyser.h
 *
 *  Created on: 24 Aug 2016
 *      Author: jkiesele
 */

#ifndef analyser_H_
#define analyser_H_

#include "interface/basicAnalyzer.h"
#include "interface/sampleCollection.h"
#include "classes/DelphesClasses.h"


class analyser: public d_ana::basicAnalyzer{
public:
	analyser():d_ana::basicAnalyzer(){
		setTreeName("ana/hgc");
	}
	~analyser(){}


private:

	// rechit outputs
	std::vector< std::vector<float> > _out_rechit;
	std::vector<std::vector<int>> _out_rechit_simcluster_indices; // save simcluster id as index appearing in simcluster_* arrays of root file
	std::vector<std::vector<float>> _out_rechit_simcluster_frac;
	// simcluster stats output
	std::vector<float> _out_simcluster_stats;

	void registerOutputVectors( TTree * tree );

	void clearOutputVectors( );

	void analyze(size_t id);

	void postProcess();
	
};



#endif /* analyser_H_ */
