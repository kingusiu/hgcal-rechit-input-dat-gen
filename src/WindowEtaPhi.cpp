#include "interface/WindowEtaPhi.h"

#include <algorithm>
#include <iostream>
#include "../interface/helpers.h"

WindowEtaPhi::WindowEtaPhi( std::vector<int> * hit_indices_of_clusters, double eta_phi_margin, std::vector<float> * rechit_eta, std::vector<float> * rechit_phi ){

    std::pair<float,float> min_max_eta = getMinMaxOfIndexSubset( hit_indices_of_clusters, rechit_eta );
    std::pair<float,float> min_max_phi = getMinMaxOfIndexSubset( hit_indices_of_clusters, rechit_phi );

    _eta_window = { min_max_eta.first-eta_phi_margin, min_max_eta.second+eta_phi_margin };
    _phi_window = { min_max_phi.first-eta_phi_margin, min_max_phi.second+eta_phi_margin };

}

float WindowEtaPhi::deltaPhi( float a, float b ){
    return helpers::deltaPhi<float>(a,b);
}


std::pair<float,float> WindowEtaPhi::getMinMaxOfIndexSubset( std::vector<int> * indices, std::vector<float> * values ){

    float min = values->at(indices->at(0));
    float max = min;

    for( int i : *indices ){
        if(values->at(i) < min ) min = values->at(i);
        if(values->at(i) > max ) max = values->at(i);
    }
    
    return std::pair<float,float>{ min, max };
}

bool WindowEtaPhi::hitIsInWindow( float eta, float phi ){
    return eta >= _eta_window.first && eta <= _eta_window.second  && deltaPhi(phi, _phi_window.first) >= 0 && deltaPhi(phi, _phi_window.second) <= 0;
}

std::vector<int> WindowEtaPhi::getHitIndicesInEtaPhiWindow( std::vector<int> * hit_indices_of_clusters, std::vector<float> * rechit_eta, std::vector<float> * rechit_phi ){

    std::vector<int> indices_in_window;
    
    for(int i = 0; i < (int)rechit_eta->size(); i++ ){
        if( hitIsInWindow( rechit_eta->at(i), rechit_phi->at(i) ) ) indices_in_window.push_back( i );
    }

    return indices_in_window;
}
