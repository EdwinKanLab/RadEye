#include "EyeManual.hpp"

#include <iostream>
#include <unistd.h>
#include <SFML/Window/Keyboard.hpp>

using namespace std;
using namespace sf;




EyeManual::EyeManual(int16_t steps_per_rot, uint8_t numFracs, uint8_t* Fracs, PiComm* comm)
//EyeManual::EyeManual(int16_t steps_per_rot, uint8_t numFracs, uint8_t* Fracs)
{
	this->steps_per_rot = steps_per_rot;
	this->numFracs = numFracs;
	this->Fracs = Fracs;
	this->comm = comm;
}
void EyeManual::start(){
	int l, r, u, d;
	int h, v;
	Keyboard::Key L = Keyboard::Left;
	Keyboard::Key R = Keyboard::Right;
	Keyboard::Key U = Keyboard::Up;
	Keyboard::Key D = Keyboard::Down;

	int disp = 1;
	h = 0;
	v = 0;

	while(1){
		l = Keyboard::isKeyPressed(L)? 1:0;
		r = Keyboard::isKeyPressed(R)? 2:0;
		u = Keyboard::isKeyPressed(U)? 4:0;
		d = Keyboard::isKeyPressed(D)? 8:0;
		
		switch(l+r+u+d){
			case 0:
				h = 0;
				v = 0;
				cout << "No Input \n" << flush;
				break;

			case 1:
				cout << "Moving Left \n" << flush;
				h = -1*disp;				
				break;

			case 2:
				cout << "Moving Right \n" << flush;
				h = disp;				
				break;

			case 4:
				cout << "Moving Up \n" << flush;
				v = -1*disp;				
				break;

			case 5:
				cout << "Moving Left and Up \n" << flush;
				h = -1*disp;
				v = -1*disp;
				break;

			case 6:
				cout << "Moving Right and Up \n" << flush;
				h = disp;
				v = -1*disp;				
				break;

			case 8:
				cout << "Moving Down \n" << flush;
				v = disp;
				break;

			case 9:
				cout << "Moving Left and Down \n" << flush;
				h = -1*disp;
				v = disp;				
				break;

			case 10:
				cout << "Moving Right and Down \n" << flush;
				h = disp;
				v = disp;				
				break;

			default:
				cout << "Static" << flush;
				break;
			}
		this->setRots(h,v);
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
