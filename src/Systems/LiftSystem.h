#ifndef SRC_SYSTEMS_LIFTSYSTEM_H_
#define SRC_SYSTEMS_LIFTSYSTEM_H_

#include "WPILib.h"
#include "Utilities.h"

class LiftSystem{
public:
	LiftSystem(void);
	~LiftSystem(void);

	void RunLift(Joystick *gamePad);
	void ResetLifter(void);
	void SetZero(void);

	Victor *liftMotor;

	Encoder *liftDistance;

private:
	DigitalInput *BottomLS;
	Utilities *utilities;

};

#endif /* SRC_SYSTEMS_LIFTSYSTEM_H_ */
