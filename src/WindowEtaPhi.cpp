#include "interface/WindowEtaPhi.h"

#include <algorithm>

std::pair<float,float> WindowEtaPhi::getMinMax( std::vector<float> * values ){
    return std::pair<float,float>{ *std::min_element(values->begin(), values->end()), *std::max_element(values->begin(), values->end()) };
}

std::vector<int> WindowEtaPhi::getHitIndicesInEtaPhiWindow( std::vector<int> * hit_indices_of_clusters, std::vector<float> * rechit_eta, std::vector<float> * rechit_phi, double eta_phi_margin ){

    //std::pair<float,float> min_max_eta = 

}