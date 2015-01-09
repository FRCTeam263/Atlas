#include "LiftSystem.h"

LiftSystem::LiftSystem(void){
	liftMotor = new Victor(4);

	utilities = new Utilities;

	liftMotor->Set(0);
}

LiftSystem::~LiftSystem(void){
	delete liftMotor;
}

void LiftSystem::RunLift(Joystick *gamePad){
	if(gamePad->GetRawButton(1)){
		liftMotor->Set(0.8);
	}
	else{
		liftMotor->Set(0);
	}
	if(gamePad->GetRawButton(2)){
		liftMotor->Set(-0.8);
	}
	else{
		liftMotor->Set(0);
	}
}
