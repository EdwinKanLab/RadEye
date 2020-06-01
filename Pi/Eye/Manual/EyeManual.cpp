#include "EyeManual.hpp"

#include <iostream>
using namespace std;
EyeManual::EyeManual(int16_t steps_per_rot, uint8_t numFracs, uint8_t* Fracs, PiComm* comm)
//EyeManual::EyeManual(int16_t steps_per_rot, uint8_t numFracs, uint8_t* Fracs)
{
	this->steps_per_rot = steps_per_rot;
	this->numFracs = numFracs;
	this->Fracs = Fracs;
	this->comm = comm;
}
void EyeManual::start(){
	int h, v;
	while(1){
		cout << "Enter Horizontal Angle: ";
		cin >> h;
		cout << "Enter Vertical Angle: ";
		cin >> v;
		cout <<"Sending " << h << " horizontal angle and "<< v << " vertical angle to uC. \n";	
		this->setRots(h, v);
		this->sendTouC();
	}

}

void EyeManual::setRots(float angle_a, float angle_b)

{
	// x/400*Fracs[0] = angle/360 => x = 400*angle/360

	float rot0 = angle_a * ((float) (this->steps_per_rot));
	rot0 *= (float) this->Fracs[0];
	rot0 /= 360;
	this->rots[0] = (int16_t) rot0;

	float rot1 = angle_b * ((float) (this->steps_per_rot));
	rot1 *= (float) this->Fracs[0];
	rot1 /= 360;
	this->rots[1] = (int16_t) rot1;

}

void EyeManual::sendTouC(){

		comm->setData(comm, this->rots);
		comm->writeCycle(comm);
		(comm->ENPIN)->writeLow((comm->ENPIN));

		//cout << "Horizontal " << this->rots[0] << " Vertical " << this->rots[1] << "\n";
		cout << "Data sent.\n";
		cout << "\n";
	
}
