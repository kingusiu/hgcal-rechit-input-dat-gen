#include "interface/RechitNtupleConverter.h"

#include <algorithm>
#include <numeric>


RechitNtupleConverter::RechitNtupleConverter( std::vector<float> * en, std::vector<float> * xx, std::vector<float> * yy, std::vector<int> * dd, std::vector<float> * pp, std::vector<float> * et ):
                                            _energy( en ), _x( xx ), _y( yy ), _detid( dd ), _phi( pp ), _eta( et ) { }

std::vector<float> RechitNtupleConverter::computeTheta( ){
    std::vector<float> theta;
	std::transform( _eta->begin(), _eta->end(), std::back_inserter( theta ), thetaF );
    return theta;
}