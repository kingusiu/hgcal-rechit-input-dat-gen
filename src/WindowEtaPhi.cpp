#include "interface/WindowEtaPhi.h"

#include <algorithm>

std::pair<float,float> WindowEtaPhi::getMinMaxOfIndexSubset( std::vector<int> * indices, std::vector<float> * values ){

    float min = values->at(indices->at(0));
    float max = min;

    for( int i : *indices ){
        if(values->at(i) < min ) min = values->at(i);
        if(values->at(i) > max ) max = values->at(i);
    }
    
    return std::pair<float,float>{ min, max };
}

bool WindowEtaPhi::hitIsInWindow( float eta, float phi, std::pair<float,float> eta_window, std::pair<float,float> phi_window ){
    return eta >= eta_window.first && eta <= eta_window.second && phi >= phi_window.first && phi <= phi_window.second;
}

std::vector<int> WindowEtaPhi::getHitIndicesInEtaPhiWindow( std::vector<int> * hit_indices_of_clusters, std::vector<float> * rechit_eta, std::vector<float> * rechit_phi, double eta_phi_margin ){

    std::pair<float,float> min_max_eta = getMinMaxOfIndexSubset( hit_indices_of_clusters, rechit_eta );
    std::pair<float,float> min_max_phi = getMinMaxOfIndexSubset( hit_indices_of_clusters, rechit_phi );

    std::pair<float,float> eta_window = { min_max_eta.first-eta_phi_margin, min_max_eta.second+eta_phi_margin };
    std::pair<float,float> phi_window = { min_max_phi.first-eta_phi_margin, min_max_phi.second+eta_phi_margin };

    std::vector<int> indices_in_window;
    
    for(int i = 0; i < rechit_eta->size(); i++ ){
        if( hitIsInWindow( rechit_eta->at(i), rechit_phi->at(i), eta_window, phi_window ) ) indices_in_window.push_back( i );
    }

    return indices_in_window;
}