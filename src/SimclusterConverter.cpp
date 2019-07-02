#include "interface/SimclusterConverter.h"

SimclusterConverter::SimclusterConverter( std::vector<float> * eta, std::vector<std::vector<int>> * hits_idx, std::vector<std::vector<float>> * frac, bool sorted ):
                                            _eta( eta ), _hits_idx( hits_idx ), _frac( frac ) { }

