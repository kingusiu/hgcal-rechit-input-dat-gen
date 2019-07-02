#include "interface/SimclusterConverter.h"

#include <numeric>
#include <algorithm>

using std::size_t;


SimclusterConverter::SimclusterConverter( std::vector<float> * eta, std::vector<std::vector<int>> * hits_idx, std::vector<std::vector<float>> * frac, bool sorted ):
                                            _eta( *eta ), _hits_indices( *hits_idx ), _frac( *frac ) { 

                                                if(sorted){
                                                    sortVecAByVecB( _hits_indices, _eta );
                                                    sortVecAByVecB( _frac, _eta );
                                                }
                                            }

/**
 * sorts vector A according to the order of values in another vector B
 * return : sorted A
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


