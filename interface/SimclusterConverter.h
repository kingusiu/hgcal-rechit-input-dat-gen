/*
 * SimclusterConverter.h
 *
 *  Created on: 02 July 2019
 *      Author: Kinga Anna Wozniak
 */


#ifndef simclusterconverter_H_
#define simclusterconverter_H_

#include <vector>

struct SimclusterStats {

    float _eta_min;
    float _eta_max;
    float _eta_avg;
    float _phi_min;
    float _phi_max;
    float _phi_avg;
};


class SimclusterConverter {

    void calculateEtaPhiR(std::vector<float> * rechit_eta, std::vector<float> * rechit_phi, std::vector<float> * rechit_z, std::vector<float> * rechit_energy);

    std::vector<float> _eta;
    std::vector<float> _phi;
    std::vector<float> _energy;
    std::vector<float> _Rho;
    // for the future std::vector<float> _true_energy;
    // for the future std::vector<float> _true_id;
    std::vector<std::vector<int>> _hits_indices;
    std::vector<std::vector<float>> _frac;    

    SimclusterStats _simclusStats;

    public:

    SimclusterConverter(  std::vector<std::vector<int>> * hits_idx, std::vector<std::vector<float>> * frac, std::vector<float> * rechit_eta, std::vector<float> * rechit_phi, std::vector<float> * rechit_z, std::vector<float> * rechit_energy, bool sorted = true );

    int numSimclusters( ){ return _hits_indices.size(); }

    template< typename T, typename U >
    void sortVecAByVecB( std::vector<T> & a, std::vector<U> & b ); // TODO: move to util class if more types are added   

    std::pair<std::vector<int>, std::vector<float>> getClusterIdxAndFracForHit( int hit_idx );

    std::vector<int> getHitIndicesBelongingToClusters( );

    std::vector<float> getStatsForSimclusters( );
    std::vector<std::vector<float> > getFeaturesForSimclusters( );

};

#endif /* simclusterconverter_H_ */
