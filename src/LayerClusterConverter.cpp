/*
 * LayerClusterConverter.cpp
 *
 *  Created on: 10 Jul 2019
 *      Author: jkiesele
 */

#include <stdexcept>
#include "../interface/LayerClusterConverter.h"
#include "Math/Vector3D.h"

class distribution_xy{
public:
    distribution_xy():mean_x(0), mean_y(0),m_x(0),m_y(0),v_x(0),v_y(0),s_x(0),s_y(0),counter_(0){}

    void addXY(const float& x, const float& y,  const float& w){
        counter_+=w;

        if(!mean_x){
            m_x += x*w;
            return;
        }
        float diff_x = (x-mean_x);
        float diff_y = (y-mean_y);
        v_x += diff_x*diff_x*w;
        v_y += diff_y*diff_y*w;

        s_x += diff_x*diff_x*diff_x*w;
        s_y += diff_y*diff_y*diff_y*w;
    }
    void calculateMeanXYandReset(){
        if(!counter_) return;
        mean_x=m_x/counter_;
        mean_y=m_y/counter_;
        counter_=0;
    }

    float getMeanX()const{return mean_x;}
    float getMeanY()const{return mean_y;}
    float getVarX() const{if(counter_) return v_x/counter_; else return 0;}
    float getVarY() const{if(counter_) return v_y/counter_; else return 0;}
    float getSkewX()const{if(counter_) return s_x/counter_; else return 0;}
    float getSkewY()const{if(counter_) return s_y/counter_; else return 0;}

    std::vector<float> vectorisedOutput()const{
        std::vector<float> out(6);
        out.at(0)=getMeanX();
        out.at(1)=getMeanY();
        out.at(2)=getVarX();
        out.at(3)=getVarY();
        out.at(4)=getSkewX();
        out.at(5)=getSkewY();
        return out;
    }

private:
    float mean_x, mean_y;
    float m_x,m_y;
    float v_x,v_y;
    float s_x,s_y;
    float counter_;
};




std::vector<float>  LayerClusterConverter::createLayerClusterFeatures(const int lc_index)const{
    if(init_<3)
        throw std::logic_error("LayerClusterConverter::createLayerClusterFeatures: first set all: rechits, simclusterconv, layer clusters exactly once per event");


    ROOT::Math::DisplacementVector3D<ROOT::Math::CylindricalEta3D<double> > RhoEtaPhiVector;
    RhoEtaPhiVector.SetXYZ(lc_x_->at(lc_index),lc_y_->at(lc_index),lc_z_->at(lc_index));

    std::vector<float>  out;
    out.push_back(lc_energy_->at(lc_index));
    out.push_back(RhoEtaPhiVector.eta());
    out.push_back(RhoEtaPhiVector.phi());
    out.push_back(RhoEtaPhiVector.theta());
    out.push_back(RhoEtaPhiVector.R());
    out.push_back(lc_x_     ->at(lc_index));
    out.push_back(lc_y_     ->at(lc_index));
    out.push_back(lc_z_     ->at(lc_index));
    auto distr = calculateFeatDistributions(lc_rechits_->at(lc_index));
    out.insert(out.end(), distr.begin(),distr.end());
    return out;

}

std::vector<float> LayerClusterConverter::createLayerClusterTruth(const int lc_index)const{
    if(init_<3)
            throw std::logic_error("LayerClusterConverter::createLayerClusterTruth: first set all: rechits, simclusterconv, layer clusters exactly once per event");


    return calculateTruthDistributions(lc_rechits_->at(lc_index));

}

void LayerClusterConverter::setRechits(
            const std::vector<float> *   rh_energy,
            const std::vector<float> *   rh_x,
            const std::vector<float> *   rh_y,
            const std::vector<float> *   rh_z,
            const std::vector<float> *   rh_time,
            const std::vector<int>   *   rh_detid ){

    add_init();

    rh_energy_ = rh_energy;
    rh_x_      = rh_x;
    rh_y_      = rh_y;
    rh_z_      = rh_z;
    rh_time_   = rh_time;
    rh_detid_  = rh_detid;
}

void LayerClusterConverter::setLayerClusters(
        const std::vector<float> *   lc_energy,
        const std::vector<float> *   lc_x,
        const std::vector<float> *   lc_y,
        const std::vector<float> *   lc_z,
        const std::vector<float> *   lc_eta,
        const std::vector<float> *   lc_phi,
        const std::vector<std::vector<int> >  *   lc_rechits){

    add_init();

    lc_energy_ = lc_energy;
    lc_x_      = lc_x;
    lc_y_      = lc_y;
    lc_z_      = lc_z;
    lc_eta_      = lc_eta;
    lc_phi_      = lc_phi;
    lc_rechits_= lc_rechits;
}


void LayerClusterConverter::add_init(){
    if(init_>2){ //new round
        m_layerclusterfeatures_.clear();
        m_layerclustertruth_.clear();
        init_=0;
    }
    init_++;
}

std::vector<float> LayerClusterConverter::calculateFeatDistributions(
        const std::vector<int>   &   rh_indices
) const{

    distribution_xy space;
    distribution_xy time_distr;

    //require rechit_time>0
    for(const auto& i:rh_indices){
        space.addXY(rh_x_->at(i),rh_y_->at(i),rh_energy_->at(i));
        if(rh_time_->at(i)>=0)
            time_distr.addXY(rh_time_->at(i),0,rh_energy_->at(i));
    }

    std::vector<float>  out=space.vectorisedOutput();
    out.push_back(time_distr.getMeanX());
    out.push_back(time_distr.getVarX());
    out.push_back(time_distr.getSkewX());

    return out;
}

std::vector<float> LayerClusterConverter::calculateTruthDistributions(
            const std::vector<int>   &   rh_indices
            ) const{

    const int nsim = simconv_->numSimclusters();

    std::vector<distribution_xy> mean_simfracs(nsim);

    for(const auto& i: rh_indices){
        auto fracs = simconv_->getClusterIdxAndFracForHit(i).second;
        for(int sc=0;sc<nsim;sc++){
            mean_simfracs.at(sc).addXY(fracs.at(sc),0,rh_energy_->at(i));
        }
    }
    std::vector<float> out;
    for(int sc=0;sc<nsim;sc++){
        mean_simfracs.at(sc).calculateMeanXYandReset();
        out.push_back(mean_simfracs.at(sc).getMeanX());
    }
    return out;
}







