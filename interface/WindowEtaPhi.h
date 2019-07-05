
#ifndef windowetaphi_H_
#define windowetaphi_H_

#include <vector>


class WindowEtaPhi {

        std::pair<float,float> getMinMax( std::vector<float> * values );

    public :

        std::vector<int> getHitIndicesInEtaPhiWindow( std::vector<int> * hit_indices_of_clusters, std::vector<float> * rechit_eta, std::vector<float> * rechit_phi, double eta_phi_padding );


};

#endif /* windowetaphi_H_ */