#include "LiftSystem.h"

LiftSystem::LiftSystem(void){
	shortLiftMotor1 = new CANTalon(1);
	shortLiftMotor2 = new CANTalon(2);
	longLiftMotor1 = new CANTalon(7);
	longLiftMotor2 = new CANTalon(8);

	shortBottomLS = new DigitalInput(0);
	shortTopLS = new DigitalInput(1);
	longBottomLS = new DigitalInput(2);
	longTopLS = new DigitalInput(3);
	longToteTouchSensor = new DigitalInput(4);
	shortToteTouchSensor = new DigitalInput(5);

	utilities = new Utilities();
	shortPID = new AfterPID();
	longPID = new AfterPID();

	shortPID->SetPID(1, 0, 0);
	longPID->SetPID(1, 0, 0);

	shortLiftMotor1->SetFeedbackDevice(CANTalon::QuadEncoder);
	shortLiftMotor1->SetSensorDirection(true);
	longLiftMotor1->SetFeedbackDevice(CANTalon::QuadEncoder);
	longLiftMotor1->SetSensorDirection(true);

	longLiftMotor2->SetControlMode(CANTalon::kFollower);
	longLiftMotor2->Set(7);
	shortLiftMotor2->SetControlMode(CANTalon::kFollower);
	shortLiftMotor2->Set(1);

	shortLiftMotor1->SetPosition(0);
	longLiftMotor1->SetPosition(0);

	shortEncoderDistance = shortLiftMotor1->GetPosition();
	longEncoderDistance = longLiftMotor1->GetPosition();
}

LiftSystem::~LiftSystem(void){
	delete shortLiftMotor1;
	delete shortLiftMotor2;
	delete longLiftMotor1;
	delete longLiftMotor2;

	delete shortBottomLS;
	delete shortTopLS;
	delete longBottomLS;
	delete longTopLS;
	delete shortToteTouchSensor;
	delete longToteTouchSensor;

	delete utilities;
}

void LiftSystem::TestLifter(Joystick *gamePad){
	if(longBottomLS->Get() == true){
		longLiftMotor1->SetPosition(0);
	}
	if(shortBottomLS->Get() == true){
		shortLiftMotor1->SetPosition(0);
	}

	if(longBottomLS->Get() == true && gamePad->GetRawButton(16)){
		longLiftMotor1->Set(0);
	}
	else if(longTopLS->Get() == true && gamePad->GetRawButton(15)){
		longLiftMotor1->Set(0);
	}
	else if((longTopLS->Get() == false || longTopLS->Get() == true) && gamePad->GetRawButton(16)){
		longLiftMotor1->Set(0.4);
	}
	else if((longBottomLS->Get() == false || longBottomLS->Get() == true) && gamePad->GetRawButton(15)){
		longLiftMotor1->Set(-0.8);
	}
	else{
		longLiftMotor1->Set(0);
	}

	if(shortBottomLS->Get() == true && gamePad->GetRawButton(7)){
		shortLiftMotor1->Set(0);
	}
	else if(shortTopLS->Get() == true && gamePad->GetRawButton(8)){
		shortLiftMotor1->Set(0);
	}
	else if((shortTopLS->Get() == false || shortTopLS->Get() == true) && gamePad->GetRawButton(7)){
		shortLiftMotor1->Set(0.5);
	}
	else if((shortBottomLS->Get() == false || shortBottomLS->Get() == true) && gamePad->GetRawButton(8)){
		shortLiftMotor1->Set(-0.5);
	}
	else{
		shortLiftMotor1->Set(0);
	}
}

void LiftSystem::RunLongLift(Joystick *gamePad){
	if(longBottomLS->Get() == true){
		longLiftMotor1->SetPosition(0);
	}

	if(gamePad->GetRawButton(1)){
		while(longEncoderDistance < 750){
			SetSpeed(-0.8, true, false);
		}
		SetSpeed(0, true, false);
	}
	else if(gamePad->GetRawButton(2)){
		while(longEncoderDistance < 1500){
			SetSpeed(-0.8, true, false);
		}
		SetSpeed(0, true, false);
	}
	else if(gamePad->GetRawButton(3)){
		while(longEncoderDistance < 2900){
			SetSpeed(-0.8, true, false);
		}
		SetSpeed(0, true, false);
	}
	else if(gamePad->GetRawButton(6)){
		while(longEncoderDistance > 750){
			SetSpeed(0.4, true, false);
		}
		SetSpeed(0, true, false);
	}
	else if(gamePad->GetRawButton(7)){
		while(longEncoderDistance > 1500){
			SetSpeed(0.4, true, false);
		}
		SetSpeed(0, true, false);
	}
	else if(gamePad->GetRawButton(8)){
		while(longEncoderDistance > 2900){
			SetSpeed(0.4, true, false);
		}
		SetSpeed(0, true, false);
	}
}

void LiftSystem::RunShortLift(Joystick *gamePad){
	float MotorOutput = 0;
	static int ShortLevel = 0;

	if(shortBottomLS->Get() == true){
		shortLiftMotor1->SetPosition(0);
	}

	if(gamePad->GetRawButton(1) == true){
		ShortLevel = 0;
	}
	else if(gamePad->GetRawButton(2) == true){
		ShortLevel = 1;
	}


	if(ShortLevel == 0){
		MotorOutput = 0;
		while(shortEncoderDistance > 0){
			MotorOutput = 1;
		}
	}
	else if(ShortLevel == 1){
		MotorOutput = 0;
		while(shortEncoderDistance < 950){
			MotorOutput = -0.8;
		}
		while(shortEncoderDistance > 1050){
			MotorOutput = 0.4;
		}
	}

	if(shortTopLS->Get() == true && MotorOutput > 0){
		MotorOutput = 0;
	}
	else if(shortBottomLS->Get() == true && MotorOutput < 0){
		MotorOutput = 0;
	}

	shortLiftMotor1->Set(MotorOutput);
}

void LiftSystem::ResetLifter(){
	if(shortBottomLS->Get() == false){
		shortLiftMotor1->Set(1);
	}
	else if(shortBottomLS->Get() == true){
		shortLiftMotor1->Set(0);
	}

	if(longBottomLS->Get() == false){
		longLiftMotor1->Set(1);
	}
	else if(longBottomLS->Get() == true){
		longLiftMotor1->Set(0);
	}
}

void LiftSystem::SetSpeed(float Speed, bool longSide, bool shortSide, bool BothSides){
	if(longSide == true){
		longLiftMotor1->Set(Speed);
		shortLiftMotor1->Set(0);
	}
	else if(shortSide == true){
		shortLiftMotor1->Set(Speed);
		longLiftMotor1->Set(0);
	}
	else if(BothSides == true){
		shortLiftMotor1->Set(Speed);
		longLiftMotor1->Set(Speed);
	}
	else{
		shortLiftMotor1->Set(0);
		longLiftMotor1->Set(0);
	}
}

float LiftSystem::ElevatorSpeed(float Setpoint){
	const float minVelocityInPercentThatOvercomesMotorInertia = 0.25;  // must be determined by empirical measurement.
	const float accelerationStepSizeInPercent = 0.05;  // Increment used to adjust velocity per per time unit.
	const int deadbandInEncoderCounts = 100;  // Arrive "close enough".  Specify [0..n] to affect range of [-n .. n]
	float currentVelocityCommandInPercent = 0;  // range -1.0 .. 1.0
	int lastEncoderCount = 0; // used to compute actual velocity of current time interval.
	bool firstTimeCalled = true;

	float CurrentEncoder = longLiftMotor1->GetPosition();

	if (firstTimeCalled)
	{
		firstTimeCalled = false;
		// have to allow lastEncoderCount to be initialized with real measurement at first call for time n-1.
	}
	else
	{
		int measuredVelocityInEncoderCounts = CurrentEncoder - lastEncoderCount;
		int remainingEncoderCountsToTarget = Setpoint - CurrentEncoder;

		if ( fabs(remainingEncoderCountsToTarget) > deadbandInEncoderCounts) {

			// Check current speed against how many more counts remain to go, be sure to
			// provide (add) room for ramp down.  The current velocity times the number of steps it will
			// take to ramp down to zero would be the distance as if we stayed constant at that velocity.
			// Simply divide by two to account for the ramp, results in the total distance travelled during
			// the ramp down.  Thus V * (V/steps) / 2.
			float roomForRampDownInEncoderCounts =
					fabs(measuredVelocityInEncoderCounts) *
					(fabs(measuredVelocityInEncoderCounts) / accelerationStepSizeInPercent + 1) / 2;
			// compare room required to room remaining
			if ( (fabs(measuredVelocityInEncoderCounts) + roomForRampDownInEncoderCounts) <
					fabs(remainingEncoderCountsToTarget) ) {

				// Speed up.  If at zero, jump to get motor past its inertial threshold, then
				// thereafter incrementally ramp velocity up.
				if (currentVelocityCommandInPercent < minVelocityInPercentThatOvercomesMotorInertia)
					currentVelocityCommandInPercent = minVelocityInPercentThatOvercomesMotorInertia;
				else
					currentVelocityCommandInPercent +=
							((remainingEncoderCountsToTarget > 0) ? 1 : -1) * accelerationStepSizeInPercent;
			}
			else {
				// Not enough runway to continue at this velocity, must slow down.  Keep
				// slowing down until reach inertial threshold of motor, then just drop to
				// zero since the motor stopped spinning anyway.  (Threshold handle later)
				currentVelocityCommandInPercent -=
						((remainingEncoderCountsToTarget > 0) ? 1 : -1) * accelerationStepSizeInPercent;
			}
		}
		else {
			currentVelocityCommandInPercent = 0;
		}

		// Keep motor command within motor range -1.0 .. 1.0
		// If within deadband, snap it to zero.
		if (currentVelocityCommandInPercent > 1.0)
			currentVelocityCommandInPercent = 1.0;
		else if (currentVelocityCommandInPercent < -1.0)
			currentVelocityCommandInPercent = -1.0;
		else if (fabs(currentVelocityCommandInPercent) < minVelocityInPercentThatOvercomesMotorInertia)
			currentVelocityCommandInPercent = 0.0;
	}

	// Remember current encoder count and current motor command for next iteration.
	lastEncoderCount = CurrentEncoder;
	return currentVelocityCommandInPercent;
}
