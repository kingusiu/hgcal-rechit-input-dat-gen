#include "interface/RechitConverter.h"

#include <algorithm>
#include <numeric>
#include <iostream>


RechitConverter::RechitConverter( std::vector<float> * en, std::vector<float> * xx, std::vector<float> * yy, std::vector<float> * zz, std::vector<int> * dd, std::vector<float> * pp, std::vector<float> * et ):
                                            _energy( en ), _x( xx ), _y( yy ), _z( zz ), _detid( dd ), _phi( pp ), _eta( et ) { 

                                                    	std::transform( _eta->begin(), _eta->end(), std::back_inserter( _theta ), computeTheta ); // compute theta
                                                        //std::cout << "z min: " << *std::min_element(_z->begin(), _z->end()) << ", z max: " << *std::max_element(_z->begin(), _z->end()) << std::endl;
                                            }

std::vector<float> RechitConverter::getFeaturesForHit( int rechit_idx ){
    return std::vector<float>{ _energy->at(rechit_idx), _x->at(rechit_idx), _y->at(rechit_idx), _z->at(rechit_idx), static_cast<float>(_detid->at(rechit_idx)), _phi->at(rechit_idx), _eta->at(rechit_idx), _theta.at(rechit_idx) };
}