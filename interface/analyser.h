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
	void analyze(size_t id);

	void postProcess();
	
};



#endif /* analyser_H_ */
