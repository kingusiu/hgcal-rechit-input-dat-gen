
#ifndef simclusterconverter_H_
#define simclusterconverter_H_

#include <vector>


class SimclusterConverter {

    std::vector<float> * _eta;
    std::vector<std::vector<int>> * _hits_idx;
    std::vector<std::vector<float>> * _frac;    

    public:

    SimclusterConverter( std::vector<float> * eta, std::vector<std::vector<int>> * hits_idx, std::vector<std::vector<float>> * frac, bool sorted = true );

    int numSimclusters( ){ return _hits_idx->size(); }

    template< typename T, typename U >
    std::vector<T> sortVecAByVecB( std::vector<T> & a, std::vector<U> & b ); // TODO: move to util class if more types are added

};

#endif /* simclusterconverter_H_ */