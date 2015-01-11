#include "LiftSystem.h"

LiftSystem::LiftSystem(void){
	liftMotor = new Victor(4);

	liftDistance = new Encoder(8, 9, false, Encoder::k4X);

	BottomLS = new DigitalInput(1);

	utilities = new Utilities;

	liftMotor->Set(0);
}

LiftSystem::~LiftSystem(void){
	delete liftMotor;
}

void LiftSystem::RunLift(Joystick *gamePad){
	if(gamePad->GetRawButton(1)){
		liftMotor->Set(0.5);
	}
	else if(gamePad->GetRawButton(2)){
		liftMotor->Set(-0.5);
	}
	else{
		liftMotor->Set(0);
	}
}

void LiftSystem::ResetLifter(){
	if(!BottomLS->Get()){
		liftMotor->Set(-1.0);
	}
	if(BottomLS->Get()){
		liftMotor->Set(0);
	}
}
