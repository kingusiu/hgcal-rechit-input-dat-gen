#include "interface/SimclusterConverter.h"
#include "../interface/helpers.h"
#include <numeric>
#include <algorithm>
#include <set>
#include "Math/Vector3D.h"

#include <iostream>

using std::size_t;


void SimclusterConverter::calculateEtaPhiR(std::vector<float> * rechit_x, std::vector<float> * rechit_y, std::vector<float> * rechit_z, std::vector<float> * rechit_energy){
    if(_frac.size()<1) return; //no simcluster

    std::vector<double> avg_x(_frac.size(),0);
    std::vector<double> avg_y(_frac.size(),0);
    std::vector<double> avg_z(_frac.size(),0);
    std::vector<double> energy(_frac.size(),0);

    _eta.clear();
    _phi.clear();
    _Rho.clear();
    _energy.clear();

    for(size_t rh_idx=0;rh_idx<rechit_x->size();rh_idx++){

        const float& rh_x = rechit_x->at(rh_idx);
        const float& rh_y = rechit_y->at(rh_idx);
        const float& rh_z = rechit_z->at(rh_idx);
        const float& rh_energy = rechit_energy->at(rh_idx);


        for(size_t sc_idx=0;sc_idx<_frac.size();sc_idx++){
            auto found = std::find( _hits_indices.at(sc_idx).begin(), _hits_indices.at(sc_idx).end(), (int)rh_idx );
            if(found  ==  _hits_indices.at(sc_idx).end())continue;

            int frac_idx = found - _hits_indices.at(sc_idx).begin();

            double weighted_energy = _frac.at(sc_idx).at(frac_idx) * rh_energy;

            avg_x.at(sc_idx)   += weighted_energy * rh_x;
            avg_y.at(sc_idx)   += weighted_energy * rh_y;
            avg_z.at(sc_idx)   += weighted_energy * rh_z;
            energy.at(sc_idx)  += weighted_energy;
        }
    }
    for(size_t sc_idx=0;sc_idx<_frac.size();sc_idx++){

        avg_x.at(sc_idx) /= energy.at(sc_idx);
        avg_y.at(sc_idx) /= energy.at(sc_idx);
        avg_z.at(sc_idx) /= energy.at(sc_idx);

        ROOT::Math::DisplacementVector3D<ROOT::Math::CylindricalEta3D<double> > RhoEtaPhiVector;
        RhoEtaPhiVector.SetXYZ(avg_x.at(sc_idx), avg_y.at(sc_idx),avg_z.at(sc_idx));

        _eta.push_back(RhoEtaPhiVector.Eta());
        _phi.push_back(RhoEtaPhiVector.Phi());
        _Rho.push_back(RhoEtaPhiVector.Rho());
        _energy.push_back(energy.at(sc_idx));
    }


}


SimclusterConverter::SimclusterConverter( std::vector<std::vector<int>> * hits_idx, std::vector<std::vector<float>> * frac, std::vector<float> * rechit_eta, std::vector<float> * rechit_phi, std::vector<float> * rechit_z, std::vector<float> * rechit_energy, bool sorted ):
                                            _eta( {} ), _phi( {} ), _hits_indices( *hits_idx ), _frac( *frac ) {
    calculateEtaPhiR(rechit_eta,rechit_phi,rechit_z,rechit_energy);
                                                if(sorted){
                                                    sortVecAByVecB( _hits_indices, _eta );
                                                    sortVecAByVecB( _frac, _eta );
                                                    sortVecAByVecB( _phi, _eta );
                                                    sortVecAByVecB( _Rho, _eta );
                                                    sortVecAByVecB( _eta, _eta );
                                                }
                                            }

/**
 * sorts vector A according to the order of values in another vector B in place
 **/
template< typename T, typename U >
void SimclusterConverter::sortVecAByVecB( std::vector<T> & a, std::vector<U> & b ){

	std::vector<std::pair<T,U>> zipped(a.size());
	for( size_t i = 0; i < a.size(); i++ ) zipped[i] = std::make_pair( a[i], b[i] ); // zip the two vectors (A,B)

	std::sort(zipped.begin(), zipped.end(), []( auto & lop, auto & rop ) { return lop.second < rop.second; }); // sort according to B

	std::transform(zipped.begin(), zipped.end(), a.begin(), []( auto & pair ){ return pair.first; }); // extract sorted A

}

template void SimclusterConverter::sortVecAByVecB( std::vector<std::vector<int>> & a, std::vector<float> & b ); // int vec instance
template void SimclusterConverter::sortVecAByVecB( std::vector<std::vector<float>> & a, std::vector<float> & b ); // float vec instance


std::pair<std::vector<int>, std::vector<float>> SimclusterConverter::getClusterIdxAndFracForHit( int hit_idx ){

    std::vector<int> cluster_indices_for_hit;
    std::vector<float> cluster_fractions_for_hit( numSimclusters(), 0.0 );

    // go through each simcluster and look for hit 'hit_idx'
    for( int cluster_idx = 0; cluster_idx < numSimclusters(); cluster_idx++ ) {

        auto hits_in_cluster = _hits_indices.at(cluster_idx);
        //std::cout << "number of hits in cluster " << cluster_idx << ": " << hits_in_cluster.size() << std::endl;
        auto found = std::find( hits_in_cluster.begin(), hits_in_cluster.end(), hit_idx );

        if( found != hits_in_cluster.end() ){
            //std::cout << hit_idx << " found!" << std::endl;
            cluster_indices_for_hit.push_back(cluster_idx); // store cluster id to hit
            int frac_idx = found - hits_in_cluster.begin();
            float frac = _frac.at(cluster_idx).at(frac_idx);
            cluster_fractions_for_hit.at( cluster_idx ) = frac;// store fraction of cluster to this hit's fraction vector
        }
    }

    return std::pair<std::vector<int>, std::vector<float>>{ cluster_indices_for_hit, cluster_fractions_for_hit };
}

std::vector<int> SimclusterConverter::getHitIndicesBelongingToClusters( ){

    std::size_t numRechitsHit = std::accumulate(_hits_indices.begin(), _hits_indices.end(), std::size_t{0}, []( auto & lop, auto & rop ){ return lop + rop.size(); });

    std::vector<int> hits_indices_flattened;
    hits_indices_flattened.reserve( numRechitsHit );

    for(auto & v : _hits_indices ) hits_indices_flattened.insert( hits_indices_flattened.end(), v.begin(), v.end() );

    std::set<int> unique( hits_indices_flattened.begin(), hits_indices_flattened.end() );
    unique.erase(-1); // indices in CloseByParticleGunProducer are initialized with -1, those not found in map of true "firing rechits" remain thus -1

    return std::vector<int>{ unique.begin(), unique.end() }; // return unique hit indices
}

std::vector<float> SimclusterConverter::getStatsForSimclusters( ){
    
    float eta_min = *std::min_element( _eta.begin(), _eta.end() );
    float eta_max = *std::max_element( _eta.begin(), _eta.end() );
    float eta_avg = std::accumulate( _eta.begin(), _eta.end(), 0.0 ) / _eta.size();
    float phi_min = *std::min_element( _phi.begin(), _phi.end() );
    float phi_max = *std::max_element( _phi.begin(), _phi.end() );
    float phi_avg = std::accumulate( _phi.begin(), _phi.end(), 0.0 ) / _phi.size();
    
    return std::vector<float>{ eta_min, eta_max, eta_avg, phi_min, phi_max, phi_avg };
}

std::vector<std::vector<float> > SimclusterConverter::getFeaturesForSimclusters( ){

    std::vector<std::vector<float> > out;

    for(size_t i=0;i<_energy.size();i++){
        std::vector<float> features = {_energy.at(i), _eta.at(i), _phi.at(i), _Rho.at(i)};
        out.push_back(features);
    }
    return out;
}


