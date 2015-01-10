#ifndef SRC_SYSTEMS_MECANUMDRIVE_H_
#define SRC_SYSTEMS_MECANUMDRIVE_H_

#include "WPILib.h"
#include "LiftSystem.h"
#include "Utilities.h"

class MecanumDrive{
public:
	MecanumDrive(void);
	~MecanumDrive(void);

	void Drive(Joystick *drivePad);
	void AutonDrive(Joystick *box, LiftSystem *lifter);

	Talon *FLMotor;
	Talon *FRMotor;
	Talon *BLMotor;
	Talon *BRMotor;

	Encoder *FLWheel;
	/*Encoder *BLWheel;
	Encoder *FRWheel;
	Encoder *BRWheel;*/

private:
	Utilities *utilities;

	float FLSpeed;
	float FRSpeed;
	float BLSpeed;
	float BRSpeed;
};



#endif /* SRC_SYSTEMS_MECANUMDRIVE_H_ */
