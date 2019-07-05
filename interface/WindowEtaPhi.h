
#ifndef windowetaphi_H_
#define windowetaphi_H_

#include <vector>


class WindowEtaPhi {

        static std::pair<float,float> getMinMaxOfIndexSubset( std::vector<int> * indices, std::vector<float> * values );

        static bool hitIsInWindow( float eta, float phi, std::pair<float,float> eta_window, std::pair<float,float> phi_window );

    public :

        static std::vector<int> getHitIndicesInEtaPhiWindow( std::vector<int> * hit_indices_of_clusters, std::vector<float> * rechit_eta, std::vector<float> * rechit_phi, double eta_phi_padding );


};

#endif /* windowetaphi_H_ */