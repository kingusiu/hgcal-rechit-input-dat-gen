/*
 * caloParticleMerger.h
 *
 *  Created on: 22 Jul 2019
 *      Author: jkiesele
 */

#ifndef HGCAL_RECHIT_INPUT_DAT_GEN_INTERFACE_CALOPARTICLEMERGER_H_
#define HGCAL_RECHIT_INPUT_DAT_GEN_INTERFACE_CALOPARTICLEMERGER_H_
#include <vector>


/**
 * Actually does not merge calo particles, but merged the simclusters contained in a calo particle
 * uses calopart_simClusterIndex.
 *
 * Not using const anywhere to not interfere with rest of code
 */
class caloParticleMerger{

public:
    caloParticleMerger(std::vector<std::vector<int> > * calopart_simClusterIndex,
            std::vector<std::vector<float> > * sc_frac,
            std::vector<std::vector<int> > * hits_idx);


     std::vector<std::vector<float> >  mergedFractions(){return _merged_fractions;}
     std::vector<std::vector<int> >    mergedHitIdx(){return _merged_hitidx;}


private:

    std::vector<std::vector<float> > _merged_fractions;
    std::vector<std::vector<int> > _merged_hitidx;

};


#endif /* HGCAL_RECHIT_INPUT_DAT_GEN_INTERFACE_CALOPARTICLEMERGER_H_ */
