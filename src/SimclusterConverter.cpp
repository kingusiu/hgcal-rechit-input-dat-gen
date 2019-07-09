#include "interface/SimclusterConverter.h"

#include <numeric>
#include <algorithm>
#include <set>

using std::size_t;


SimclusterConverter::SimclusterConverter( std::vector<float> * eta, std::vector<float> * phi, std::vector<std::vector<int>> * hits_idx, std::vector<std::vector<float>> * frac, bool sorted ):
                                            _eta( *eta ), _phi( *phi ), _hits_indices( *hits_idx ), _frac( *frac ) { 

                                                if(sorted){
                                                    sortVecAByVecB( _hits_indices, _eta );
                                                    sortVecAByVecB( _frac, _eta );
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