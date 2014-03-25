/*
 * modIF.cpp
 *
 *  Created on: Nov 4, 2013
 *      Author: keevi7c3l
 */

#include "modIFInfrProxSen.h"

infrProxSen::infrProxSen(CYZXInter *p){
	this->pcyzxi=p;
}

infrProxSen::~infrProxSen(){

}

int infrProxSen::getValue(unsigned int index){
	return this->pcyzxi->infrProxSen(index);
}


