/*
 * helpers.h
 *
 *  Created on: 22 Jul 2019
 *      Author: jkiesele
 */

#ifndef HGCAL_RECHIT_INPUT_DAT_GEN_INTERFACE_HELPERS_H_
#define HGCAL_RECHIT_INPUT_DAT_GEN_INTERFACE_HELPERS_H_
namespace helpers{


template<class T>
T deltaPhi( T a, T b ){

    const T pi = 3.14159265358979323846;
    T delta = (a - b);
    while (delta >= pi)  delta-= 2.* pi;
    while (delta < -pi)  delta+= 2.* pi;
    return delta;
}

}


#endif /* HGCAL_RECHIT_INPUT_DAT_GEN_INTERFACE_HELPERS_H_ */
