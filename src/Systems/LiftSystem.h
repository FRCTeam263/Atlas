#ifndef SRC_SYSTEMS_LIFTSYSTEM_H_
#define SRC_SYSTEMS_LIFTSYSTEM_H_

#include "CANTalon.h"
#include "Joystick.h"
#include "DigitalInput.h"
#include "Utilities.h"
#include "ElevatorSpeedAlgorithm.h"
#include "../Misc/AfterPID.h"
#include "../Defines.h"

class LiftSystem{
public:
	LiftSystem(void);
	~LiftSystem(void);

	void RunLifter(Joystick *gamePad);
	void RunLongLift(Joystick *gamePad);
	void RunShortLift(Joystick *gamePad);
	void ResetLifter(void);
	void SetZero(void);
	void TestLifter(Joystick *gamePad);
	void SetSpeed(float Speed, bool longSide, bool shortSide, bool BothSides = false);

	CANTalon *shortLiftMotor1;
	CANTalon *shortLiftMotor2;
	CANTalon *canLiftMotor;

	DigitalInput *shortBottomLS;
	DigitalInput *canBottomLS;
	DigitalInput *shortTopLS;
	DigitalInput *canTopLS;
	DigitalInput *shortToteTouchSensor;
	DigitalInput *longToteTouchSensor;

private:
	Utilities *utilities;
	ElevatorSpeedAlgorithm *lifterOutput;

	float shortEncoderDistance;
	float longEncoderDistance;

};

#endif /* SRC_SYSTEMS_LIFTSYSTEM_H_ */
