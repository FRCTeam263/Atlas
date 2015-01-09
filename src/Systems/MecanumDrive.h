#ifndef SRC_SYSTEMS_MECANUMDRIVE_H_
#define SRC_SYSTEMS_MECANUMDRIVE_H_

#include "WPILib.h"
#include "Utilities.h"

class MecanumDrive{
public:
	MecanumDrive(void);
	~MecanumDrive(void);

	void Drive(Joystick *drivePad);
	void AutonDrive(Joystick *box);

private:
	Talon *FLMotor;
	Talon *FRMotor;
	Talon *BLMotor;
	Talon *BRMotor;

	Utilities *utilities;

	Encoder *FLWheel;
	/*Encoder *BLWheel;
	Encoder *FRWheel;
	Encoder *BRWheel;*/

	typedef enum{Step1, Step2, Step3, Step4, Step5, Step6} AutoMode;

	AutoMode autoMode;

	float FLSpeed;
	float FRSpeed;
	float BLSpeed;
	float BRSpeed;
};



#endif /* SRC_SYSTEMS_MECANUMDRIVE_H_ */
