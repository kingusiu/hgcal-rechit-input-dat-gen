#include "interface/WindowEtaPhi.h"

#include <algorithm>
#include <iostream>


float WindowEtaPhi::deltaPhi(const float & a, const float & b)const{
    const float pi = 3.14159265358979323846;
    float delta = (a -b);
    while (delta >= pi)  delta-= 2* pi;
    while (delta < -pi)  delta+= 2* pi;
    return delta;
}

std::pair<float,float> WindowEtaPhi::getMinMaxOfIndexSubset( std::vector<int> * indices, std::vector<float> * values , bool isphi){

    float min = values->at(indices->at(0));
    float max = min;

    if(!isphi){
        for( int i : *indices ){
            if(values->at(i) < min ) min = values->at(i);
            if(values->at(i) > max ) max = values->at(i);
        }}
    else{
        for( int i : *indices ){
            if(deltaPhi(values->at(i), min) < 0 ) min = values->at(i);
            if(deltaPhi(values->at(i), max) > 0 ) max = values->at(i);
        }
    }

    return std::pair<float,float>{ min, max };
}

bool WindowEtaPhi::hitIsInWindow( float eta, float phi)const{
    return eta >= eta_window.first && eta <= eta_window.second && deltaPhi(phi, phi_window.first)>=0 && deltaPhi(phi, phi_window.second)<=0;
}

std::vector<int> WindowEtaPhi::createWindowAndGetHitIndicesInEtaPhiWindow( std::vector<int> * hit_indices_of_clusters, std::vector<float> * rechit_eta,
        std::vector<float> * rechit_phi, double eta_phi_margin){

    std::pair<float,float> min_max_eta = getMinMaxOfIndexSubset( hit_indices_of_clusters, rechit_eta , false);
    std::pair<float,float> min_max_phi = getMinMaxOfIndexSubset( hit_indices_of_clusters, rechit_phi , true);

    eta_window = { min_max_eta.first-eta_phi_margin, min_max_eta.second+eta_phi_margin };
    phi_window = { min_max_phi.first-eta_phi_margin, min_max_phi.second+eta_phi_margin };

    std::vector<int> indices_in_window;
    
    for(int i = 0; i < (int)rechit_eta->size(); i++ ){
        if( hitIsInWindow( rechit_eta->at(i), rechit_phi->at(i)) ) indices_in_window.push_back( i );
    }

    return indices_in_window;
}
