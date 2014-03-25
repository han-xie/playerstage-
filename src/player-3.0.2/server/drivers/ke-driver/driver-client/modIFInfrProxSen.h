/*
 * modIF.h
 *
 *  Created on: Nov 4, 2013
 *      Author: keevi7c3l
 */

#ifndef MODIFINFRPROXSEN_H_
#define MODIFINFRPROXSEN_H_

#include "CYZXInter.h"

//红外接近传感器端口号
class infrProxSen{
public:
	int getValue(unsigned int index=0);
public:
	infrProxSen(CYZXInter *pcyzxi);
	~infrProxSen();
private:
	CYZXInter *pcyzxi;
};

#endif /* MODIF_H_ */
