/*
 * LayerClusterConverter.h
 *
 *  Created on: 10 Jul 2019
 *      Author: jkiesele
 */

#ifndef HGCAL_RECHIT_INPUT_DAT_GEN_INTERFACE_LAYERCLUSTERCONVERTER_H_
#define HGCAL_RECHIT_INPUT_DAT_GEN_INTERFACE_LAYERCLUSTERCONVERTER_H_

#include <vector>
#include "SimclusterConverter.h"

class LayerClusterConverter{

public:
    LayerClusterConverter():
        init_(0),
        simconv_(0),

        rh_energy_(0),
        rh_x_     (0),
        rh_y_     (0),
        rh_z_     (0),
        rh_time_  (0),
        rh_detid_ (0),

        lc_energy_(0),
        lc_x_(0),
        lc_y_(0),
        lc_z_(0),
        lc_eta_(0),
        lc_phi_(0),
        lc_rechits_(0){}

    ~LayerClusterConverter(){}

    void setSimClusterConverter(SimclusterConverter * simconv){
        add_init();
        simconv_=simconv;
    }

    void setRechits(
            const std::vector<float> *   rh_energy,
            const std::vector<float> *   rh_x,
            const std::vector<float> *   rh_y,
            const std::vector<float> *   rh_z,
            const std::vector<float> *   rh_time,
            const std::vector<int>   *   rh_detid );

    void setLayerClusters(
            const std::vector<float> *   lc_energy,
            const std::vector<float> *   lc_x,
            const std::vector<float> *   lc_y,
            const std::vector<float> *   lc_z,
            const std::vector<float> *   lc_eta,
            const std::vector<float> *   lc_phi,
            const std::vector<std::vector<int> >  *   lc_rechits
    );

    std::vector<float>  createLayerClusterFeatures(const int lc_index)const;

    std::vector<float>  createLayerClusterTruth(const int lc_index)const;

    //compat to RecHitConverter

    /*const*/ std::vector<float> * eta(){ return const_cast<std::vector<float> *>(lc_eta_); };//FIXME in WindowEtaPhi
    /*const*/ std::vector<float> * phi(){ return const_cast<std::vector<float> *>(lc_phi_); };

private:

    void add_init();

    /**
     * Calculates mean, variance, skew in each dimension x, y, z, weighted by energy and time
     */
    std::vector<float> calculateFeatDistributions(
            const std::vector<int>   &   rh_indices
            ) const;

    std::vector<float> calculateTruthDistributions(
            const std::vector<int>   &   rh_indices
            ) const;

    int init_;//0, 1, 2, 3 = no, N_coll_set

    SimclusterConverter * simconv_;

    const std::vector<float> * rh_energy_;
    const std::vector<float> * rh_x_;
    const std::vector<float> * rh_y_;
    const std::vector<float> * rh_z_;
    const std::vector<float> * rh_time_;
    const std::vector<int>   * rh_detid_;

    const std::vector<float> *   lc_energy_;
    const std::vector<float> *   lc_x_;
    const std::vector<float> *   lc_y_;
    const std::vector<float> *   lc_z_;
    const std::vector<float> *   lc_eta_;
    const std::vector<float> *   lc_phi_;
    const std::vector<std::vector<int> >  *   lc_rechits_;



    mutable std::vector<std::vector<float> > m_layerclusterfeatures_;
    mutable std::vector<std::vector<float> > m_layerclustertruth_;

};



#endif /* HGCAL_RECHIT_INPUT_DAT_GEN_INTERFACE_LAYERCLUSTERCONVERTER_H_ */
