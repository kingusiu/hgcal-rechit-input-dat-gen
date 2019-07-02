#include "interface/SimclusterConverter.h"

#include <numeric>
#include <algorithm>

using std::size_t;


SimclusterConverter::SimclusterConverter( std::vector<float> * eta, std::vector<std::vector<int>> * hits_idx, std::vector<std::vector<float>> * frac, bool sorted ):
                                            _eta( eta ), _hits_idx( hits_idx ), _frac( frac ) { }

/**
 * sorts vector A according to the order of values in another vector B
 * return : sorted A
 **/
template< typename T, typename U >
std::vector<T> SimclusterConverter::sortVecAByVecB( std::vector<T> & a, std::vector<U> & b ){

	std::vector<std::pair<T,U>> zipped(a.size());
	for( size_t i = 0; i < a.size(); i++ ) zipped[i] = std::make_pair( a[i], b[i] ); // zip the two vectors (A,B)

	std::sort(zipped.begin(), zipped.end(), []( auto & lop, auto & rop ) { return lop.second < rop.second; }); // sort according to B

	std::vector<T> sorted;
	std::transform(zipped.begin(), zipped.end(), std::back_inserter(sorted), []( auto & pair ){ return pair.first; }); // extract sorted A

	return sorted;
}

template std::vector<std::vector<int>> SimclusterConverter::sortVecAByVecB( std::vector<std::vector<int>> & a, std::vector<float> & b ); // int vec instance
template std::vector<std::vector<float>> SimclusterConverter::sortVecAByVecB( std::vector<std::vector<float>> & a, std::vector<float> & b ); // float vec instance

