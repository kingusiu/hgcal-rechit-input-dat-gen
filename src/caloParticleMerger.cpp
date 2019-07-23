/*
 * caloParticleMerger.cpp
 *
 *  Created on: 22 Jul 2019
 *      Author: jkiesele
 */

#include "interface/caloParticleMerger.h"
#include <string> //dirty hack to get size_t


caloParticleMerger::caloParticleMerger(std::vector<std::vector<int> > * calopart_simClusterIndex,
            std::vector<std::vector<float> > * sc_frac,
            std::vector<std::vector<int> > * hits_idx){

    //calopart_simClusterIndex: calopart, SC

    for(size_t i=0;i<calopart_simClusterIndex->size();i++){
        std::vector<int> this_hits;
        std::vector<float> this_fracs;
        for(size_t j=0;j<calopart_simClusterIndex->at(i).size();j++){
            const auto & this_sc_hits = hits_idx->at(calopart_simClusterIndex->at(i).at(j));
            this_hits.insert(this_hits.end(), this_sc_hits.begin(),this_sc_hits.end());

            const auto& this_sc_fracs = sc_frac->at(calopart_simClusterIndex->at(i).at(j));
            this_fracs.insert(this_fracs.end(), this_sc_fracs.begin(),this_sc_fracs.end());
        }
        _merged_hitidx.push_back(this_hits);
        _merged_fractions.push_back(this_fracs);
    }

}
