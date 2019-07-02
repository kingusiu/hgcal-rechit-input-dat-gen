/*
 * RechitNtupleConverter
 *
 *  Created on: 01 July 2019
 *      Author: kiwoznia
 */


#ifndef rechitntupleconverter_H_
#define rechitntupleconverter_H_

#include <vector>
#include <functional>
#include <cmath>


class RechitNtupleConverter {

    std::vector<float> * _energy;
    std::vector<float> * _x;
    std::vector<float> * _y;
    std::vector<int> * _detid;
    std::vector<float> * _phi;
    std::vector<float> * _eta;
    std::vector<float> _theta;


    /**
     * computes angle theta from angle eta as 2 * arctan ( e^-eta )
     **/
    std::function< float( float )> computeTheta = []( float eta ){ return 2. * atan( exp( -eta ) ); };

    public:

    RechitNtupleConverter( std::vector<float> * en, std::vector<float> * xx, std::vector<float> * yy, std::vector<int> * ii, std::vector<float> * pp, std::vector<float> * et );

    std::vector<float> * energy() { return _energy; }
    std::vector<float> * x() { return _x; }
    std::vector<float> * y() { return _y; }
    std::vector<int> * detid() { return _detid; }
    std::vector<float> * phi() { return _phi; }
    std::vector<float> * eta() { return _eta; }

    int numRechits(){ return _energy->size(); }

    std::vector<float> getRechitFeatures( int rechit_idx );

};


#endif /* rechitntupleconverter_H_ */