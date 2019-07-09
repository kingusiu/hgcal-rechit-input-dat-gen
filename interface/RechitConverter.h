/*
 * RechitConverter
 *
 *  Created on: 01 July 2019
 *      Author: Kinga Anna Wozniak
 */


#ifndef rechitconverter_H_
#define rechitconverter_H_

#include <vector>
#include <functional>
#include <cmath>


class RechitConverter {

    std::vector<float> * _energy;
    std::vector<float> * _x;
    std::vector<float> * _y;
    std::vector<float> * _z;
    std::vector<int> * _detid;
    std::vector<float> * _phi;
    std::vector<float> * _eta;
    //std::vector<float> _theta;
    std::vector<float> *_time;


    /**
     * computes angle theta from angle eta as 2 * arctan ( e^-eta )
     **/
    std::function< float( float )> computeTheta = []( float eta ){ return 2. * atan( exp( -eta ) ); };

    public:

    RechitConverter( std::vector<float> * en, std::vector<float> * xx, std::vector<float> * yy, std::vector<float> * zz, std::vector<int> * ii, std::vector<float> * pp, std::vector<float> * et, std::vector<float> * tt );

    int numRechits(){ return _energy->size(); }

    std::vector<float> getFeaturesForHit( int rechit_idx );

    std::vector<float> * eta(){ return _eta; };
    std::vector<float> * phi(){ return _phi; };

};


#endif /* rechitconverter_H_ */