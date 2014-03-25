#ifndef _MODIFPOS_H
#define _MODIFPOS_H
#include "CYZXInter.h"
class Servo{
public:
	void Forward(int speed = 500);
	void Backward(int speed = 500);
	void stop();
	void Turn(int angle, int inSpeed);
public:
	Servo(CYZXInter *pcyzxi);
	~Servo();
private:
	CYZXInter *pcyzxi;
};
#endif
