#ifndef SRC_SYSTEMS_LIFTSYSTEM_H_
#define SRC_SYSTEMS_LIFTSYSTEM_H_

#include "WPILib.h"
#include "Utilities.h"
#include "../Misc/AfterPID.h"

class LiftSystem{
public:
	LiftSystem(void);
	~LiftSystem(void);

	void RunLongLift(Joystick *gamePad);
	void RunShortLift(Joystick *gamePad);
	void ResetLifter(void);
	void SetZero(void);
	void TestLifter(Joystick *gamePad);
	void SetSpeed(float Speed, bool longSide, bool shortSide, bool BothSides = false);
	float ElevatorSpeed(float Setpoint);

	CANTalon *shortLiftMotor1;
	CANTalon *shortLiftMotor2;
	CANTalon *longLiftMotor1;
	CANTalon *longLiftMotor2;

	DigitalInput *shortBottomLS;
	DigitalInput *longBottomLS;
	DigitalInput *shortTopLS;
	DigitalInput *longTopLS;
	DigitalInput *shortToteTouchSensor;
	DigitalInput *longToteTouchSensor;

	AfterPID *shortPID;
	AfterPID *longPID;
private:
	Utilities *utilities;

	float shortEncoderDistance;
	float longEncoderDistance;

};

#endif /* SRC_SYSTEMS_LIFTSYSTEM_H_ */
