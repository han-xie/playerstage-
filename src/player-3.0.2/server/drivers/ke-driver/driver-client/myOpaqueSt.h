/*
 * myOpaqueSt.h
 *
 *  Created on: Jul 12, 2013
 *      Author: keevi7c3l
 */

#ifndef MYOPAQUEST_H_
#define MYOPAQUEST_H_

/*type -1 opaque
 *
 *  void MFCapSetH(int inHmax,int inHmin);  -subtype 1
	 void MFCapSetS(int inSmax,int inSmin);  -2
	 void MFCapSetI(int inImax,int inImin);  -3
	 void MFCapSetMode(int inMode);          -4
	 int MFCapGetCenterX();                  -5
	 int MFCapGetCenterY();                  -6
	 int MFCapGetSum();                      -7
 * */
struct myOpaqueSt{
	int type;
	int subtype;
	int p1;
	int p2;
};


#endif /* MYOPAQUEST_H_ */
