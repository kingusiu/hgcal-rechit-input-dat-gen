#include "interface/WindowEtaPhi.h"

#include <algorithm>
#include <iostream>

WindowEtaPhi::WindowEtaPhi( std::vector<int> * hit_indices_of_clusters, double eta_phi_margin, std::vector<float> * rechit_eta, std::vector<float> * rechit_phi ){

    std::pair<float,float> min_max_eta = getMinMaxOfIndexSubset( hit_indices_of_clusters, rechit_eta, false );
    std::pair<float,float> min_max_phi = getMinMaxOfIndexSubset( hit_indices_of_clusters, rechit_phi, true );

    _eta_window = { min_max_eta.first-eta_phi_margin, min_max_eta.second+eta_phi_margin };
    _phi_window = { min_max_phi.first-eta_phi_margin, min_max_phi.second+eta_phi_margin };

}

float WindowEtaPhi::deltaPhi( float a, float b ){

    const float pi = 3.14159265358979323846;
    float delta = (a - b);
    while (delta >= pi)  delta-= 2* pi;
    while (delta < -pi)  delta+= 2* pi;
    return delta;
}


std::pair<float,float> WindowEtaPhi::getMinMaxOfIndexSubset( std::vector<int> * indices, std::vector<float> * values, bool isPhi ){

    float min = values->at(indices->at(0));
    float max = min;

    for( int i : *indices ){
        bool smaller = isPhi ? deltaPhi(values->at(i), min) < 0 : values->at(i) < min;
        if( smaller ) min = values->at(i);
        bool greater = isPhi ? deltaPhi(values->at(i), max) > 0 : values->at(i) > max;
        if( greater ) max = values->at(i);
    }
    
    return std::pair<float,float>{ min, max };
}

bool WindowEtaPhi::hitIsInWindow( float eta, float phi ){
    return eta >= _eta_window.first && eta <= _eta_window.second  && deltaPhi(phi, _phi_window.first) >= 0 && deltaPhi(phi, _phi_window.second) <=0 ;
}

std::vector<int> WindowEtaPhi::getHitIndicesInEtaPhiWindow( std::vector<int> * hit_indices_of_clusters, std::vector<float> * rechit_eta, std::vector<float> * rechit_phi ){

    std::vector<int> indices_in_window;
    
    for(int i = 0; i < rechit_eta->size(); i++ ){
        if( hitIsInWindow( rechit_eta->at(i), rechit_phi->at(i) ) ) indices_in_window.push_back( i );
    }

    return indices_in_window;
}