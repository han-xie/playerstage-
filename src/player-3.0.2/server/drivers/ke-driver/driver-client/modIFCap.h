#ifndef _MODIFCAP_H
#define _MODIFCAP_H
class Cap{
public:
	void SetYellow();
	void SetRed();
	void SetGreen();
	int capGetCenterX();
	int capGetCenterY();
	int capGetSum();
public:
	Cap(CYZXInter *pcyzxi);
	~Cap();
private:
	CYZXInter *pcyzxi;
};
#endif
