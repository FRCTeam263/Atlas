#ifndef SRC_SYSTEMS_CANGRABBER_H_
#define SRC_SYSTEMS_CANGRABBER_H_

#include "WPILib.h"

class CanGrabber {
public:
	CanGrabber();
	~CanGrabber();

	void Extend(Joystick *gamePad);

private:
	CANTalon *grabber;
	DoubleSolenoid *canExtend;
};

#endif /* SRC_SYSTEMS_CANGRABBER_H_ */
