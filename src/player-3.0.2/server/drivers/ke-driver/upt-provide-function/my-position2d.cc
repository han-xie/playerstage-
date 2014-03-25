/*
 * my-position2d.cc
 *
 *  Created on: Jul 22, 2013
 *      Author: keevi7c3l
 */

#include <iostream>
#include  "background.h"
int main(int argc, char * argv[])
{
    MFInit();
    MFSetPortDirect(0x00000FFF);
    MFDigiInit(100);
    DelayMS(100);
    MFADInit(100);
    MFSetServoMode(8,1);
    MFSetServoMode(9,1);

    MFSetServoRotaSpd(8,500);
    MFSetServoRotaSpd(9,-500);

    int t;
    std::cout<<"any key stop\n";
    std::cin>>t;

    MFSetServoRotaSpd(8,0);
    MFSetServoRotaSpd(9,0);

    return 0;
}
