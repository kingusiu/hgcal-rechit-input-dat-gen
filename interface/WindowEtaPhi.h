
#ifndef windowetaphi_H_
#define windowetaphi_H_

#include <vector>
#include <algorithm>

class WindowEtaPhi {

    std::pair<float,float> eta_window, phi_window;

        std::pair<float,float> getMinMaxOfIndexSubset( std::vector<int> * indices,
                std::vector<float> * values , bool isphi);

    public :
        float deltaPhi(const float & a, const float & b)const;

        bool hitIsInWindow( float eta, float phi)const;


        std::vector<int> createWindowAndGetHitIndicesInEtaPhiWindow( std::vector<int> * hit_indices_of_clusters, std::vector<float> * rechit_eta,
                std::vector<float> * rechit_phi, double eta_phi_padding);


};

#endif /* windowetaphi_H_ */
