
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
};

#endif /* simclusterconverter_H_ */