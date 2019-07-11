/*
 * WindowEtaPhi.h
 *
 *  Created on: 05 July 2019
 *      Author: Kinga Anna Wozniak
 */


#ifndef windowetaphi_H_
#define windowetaphi_H_

#include <vector>


class WindowEtaPhi {

        std::pair<float,float> _eta_window;
        std::pair<float,float> _phi_window;

        std::pair<float,float> getMinMaxOfIndexSubset( std::vector<int> * indices, std::vector<float> * values );


        float deltaPhi( float a, float b );

    public :
        //for layer clusters
        bool hitIsInWindow( float eta, float phi );

        WindowEtaPhi( std::vector<int> * hit_indices_of_clusters, double eta_phi_margin, std::vector<float> * rechit_eta, std::vector<float> * rechit_phi );

        std::vector<int> getHitIndicesInEtaPhiWindow( std::vector<int> * hit_indices_of_clusters, std::vector<float> * rechit_eta, std::vector<float> * rechit_phi );


};

#endif /* windowetaphi_H_ */
