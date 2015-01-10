#include "Autonomous.h"

Autonomous::Autonomous(){
	autoMode = Step1;

	toteTouchSensor = new DigitalInput(9);
	bottomLiftLS = new DigitalInput(1);

}

Autonomous::~Autonomous(){

}

void Autonomous::Run3ToteAuto(MecanumDrive *drive, LiftSystem *lifter){

	float LifterDistance = lifter->liftDistance->GetDistance();
	float WheelEncoder = drive->FLWheel->GetDistance();

	if((autoMode == Step1) && (LifterDistance <= 100)){//lift tote up
		lifter->liftMotor->Set(0.5);
		drive->FLWheel->Reset();
	}

	if((autoMode == Step1) && (LifterDistance >= 100 && LifterDistance <= 150) && WheelEncoder <= 100){//rotate left 90. and lift to 1.5 totes high lift > 150, wheel > 100
		drive->FLMotor->Set(0.5);
		drive->BLMotor->Set(0.5);
		drive->FRMotor->Set(0.5);
		drive->BRMotor->Set(0.5);

		lifter->liftMotor->Set(0.5);
	}

	if((autoMode == Step1) && LifterDistance >= 150 && (WheelEncoder >= 100 && WheelEncoder <= 1000)){//drive Forward x distance to 2nd tote. and set to step 2.
		drive->FLMotor->Set(-0.5);
		drive->BLMotor->Set(-0.5);
		drive->FRMotor->Set(0.5);
		drive->BRMotor->Set(0.5);
		autoMode = Step2;
	}

	if(autoMode == Step2 && WheelEncoder >= 1000){//when reachs 2nd tote, reset encoder value.
		drive->FLWheel->Reset();
	}

	if(autoMode == Step2 && (LifterDistance >= 150 && WheelEncoder <= 100)){//rotate -90 so 1st tote is above 2nd tote
		drive->FLMotor->Set(-0.5);
		drive->BLMotor->Set(-0.5);
		drive->FRMotor->Set(-0.5);
		drive->BRMotor->Set(-0.5);
	}

	if(autoMode == Step2 && (LifterDistance >= 90 && WheelEncoder >= 100)){//lowers 1st tote onto 2nd tote
		lifter->liftMotor->Set(-0.5);
		drive->FLWheel->Reset();
	}

	if(autoMode == Step2 && (((LifterDistance >= 0 && LifterDistance <= 95) && WheelEncoder >= -20))){//drive back to wheel <= -20 lift <= 0. set mode to step3
		drive->FLMotor->Set(-0.5);
		drive->BLMotor->Set(-0.5);
		drive->FRMotor->Set(0.5);
		drive->BRMotor->Set(0.5);

		if(!bottomLiftLS->Get()){
			lifter->liftMotor->Set(-1);
		}

		autoMode = Step3;
	}

	if(autoMode == Step3 && !toteTouchSensor->Get()){//drive forward to 2nd tote.
		drive->FLMotor->Set(0.5);
		drive->BLMotor->Set(0.5);
		drive->FRMotor->Set(-0.5);
		drive->BRMotor->Set(-0.5);
		drive->FLWheel->Reset();
	}

	if(autoMode == Step3 && LifterDistance <= 110){//move totes up
		lifter->liftMotor->Set(0.5);
	}

	if(autoMode == Step3 && (LifterDistance >= 110 && LifterDistance <= 160) && WheelEncoder <= 100){//rotate and move totes higher
		drive->FLMotor->Set(0.5);
		drive->BLMotor->Set(0.5);
		drive->FRMotor->Set(0.5);
		drive->BRMotor->Set(0.5);

		lifter->liftMotor->Set(0.5);
	}

	if(autoMode == Step3 && LifterDistance >= 150 && (WheelEncoder >= 100 && WheelEncoder <= 1000)){//move forward to 3rd tote
		drive->FLMotor->Set(-0.5);
		drive->BLMotor->Set(-0.5);
		drive->FRMotor->Set(0.5);
		drive->BRMotor->Set(0.5);

		autoMode = Step4;
	}

	if(autoMode == Step4 && WheelEncoder >= 1000){//when reaches 3rd tote, reset encoder value.
		drive->FLWheel->Reset();
	}

	if(autoMode == Step4 && (LifterDistance >= 150 && WheelEncoder <= 100)){//rotate -90 so 1st tote is above 2nd tote
		drive->FLMotor->Set(-0.5);
		drive->BLMotor->Set(-0.5);
		drive->FRMotor->Set(-0.5);
		drive->BRMotor->Set(-0.5);
	}

	if(autoMode == Step4 && (LifterDistance >= 90 && WheelEncoder >= 100)){//lowers 1st tote onto 2nd tote
		lifter->liftMotor->Set(-0.5);
		drive->FLWheel->Reset();
	}

	if(autoMode == Step4 && (((LifterDistance >= 0 && LifterDistance <= 95) && WheelEncoder >= -20))){//drive back to wheel <= -20 lift <= 0. set mode to step3
		drive->FLMotor->Set(-0.5);
		drive->BLMotor->Set(-0.5);
		drive->FRMotor->Set(0.5);
		drive->BRMotor->Set(0.5);

		if(!bottomLiftLS->Get()){
			lifter->liftMotor->Set(-1);
		}

		autoMode = Step5;
	}

	if(autoMode == Step5 && !toteTouchSensor->Get()){//drive forward to 2nd tote.
		drive->FLMotor->Set(0.5);
		drive->BLMotor->Set(0.5);
		drive->FRMotor->Set(-0.5);
		drive->BRMotor->Set(-0.5);
		drive->FLWheel->Reset();
	}

	if(autoMode == Step5 && LifterDistance <= 110){//move totes up
		lifter->liftMotor->Set(0.5);
	}
}


