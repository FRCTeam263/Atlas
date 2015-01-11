#include "Autonomous.h"

AutonomousSystem::AutonomousSystem(){
	autoMode = Reset;

	toteTouchSensor = new DigitalInput(9);
	bottomLiftLS = new DigitalInput(1);

	lifterPID = new AfterPID();
	lifterPID->SetPID(1, 0, 0);
	lifterPID->CalcEvolPID(0, 1000, 0, 0.5, 0.5);
}

AutonomousSystem::~AutonomousSystem(){

}

void AutonomousSystem::Run3ToteAuto(MecanumDrive *drive, LiftSystem *lifter){

	float LifterDistance = lifter->liftDistance->GetDistance();
	float WheelEncoder = drive->FLWheel->GetDistance();
	//float WheelEncoder = drive->FLMotor->GetPosition();

	float LifterSetpoint = 0;

	if(autoMode == Reset){
		if(bottomLiftLS->Get()){
			lifter->liftDistance->Reset();
			drive->FLWheel->Reset();

			lifterPID->ResetPID();

			LifterSetpoint = 0;

			autoMode = Lift1Tote;
		}
		else if(!bottomLiftLS->Get()){
			LifterSetpoint = -1;
		}
	}

	if(autoMode == Lift1Tote){

		if(LifterDistance <= 100){//lift tote up
			LifterSetpoint = 0.5;
		}

		if((LifterDistance >= 100 && LifterDistance <= 150) && WheelEncoder <= 100){//rotate left 90. and lift to 1.5 totes high lift > 150, wheel > 100
			drive->AutonTurn(0.5);

			LifterSetpoint = 0.5;
		}

		if(LifterDistance >= 150 && (WheelEncoder >= 100 && WheelEncoder <= 1000)){//drive Forward x distance to 2nd tote. and set to step 2.
			drive->AutonDriveStraight(true, 0.5);

			LifterSetpoint = 0;
		}

		if(LifterDistance >= 150 && WheelEncoder >= 1000){
			autoMode = Stack1Tote;
		}
	}

	if(autoMode == Stack1Tote){

		if(WheelEncoder >= 1000){//when reachs 2nd tote, reset encoder value.
			drive->FLWheel->Reset();
		}

		if(LifterDistance >= 150 && WheelEncoder <= 100){//rotate -90 so 1st tote is above 2nd tote
			drive->AutonTurn(-0.5);
		}

		if(LifterDistance >= 90 && WheelEncoder >= 100){//lowers 1st tote onto 2nd tote
			LifterSetpoint = -0.5;
			drive->FLWheel->Reset();
		}

		if(((LifterDistance >= 0 && LifterDistance <= 95) && WheelEncoder >= -20)){//drive back to wheel <= -20 lift <= 0. set mode to step3
			drive->AutonDriveStraight(true, -0.5);

			if(!bottomLiftLS->Get()){
				LifterSetpoint = -1;
			}
		}
		if(LifterDistance <= 10 && WheelEncoder <= -20){
			autoMode = Lift2Totes;
		}
	}

	if(autoMode == Lift2Totes){

		if(!toteTouchSensor->Get()){//drive forward to 2nd tote.
			drive->AutonDriveStraight(true, 0.5);
			drive->FLWheel->Reset();
		}

		if(LifterDistance <= 110){//move totes up
			LifterSetpoint = 0.5;
		}

		if((LifterDistance >= 110 && LifterDistance <= 160) && WheelEncoder <= 100){//rotate and move totes higher
			drive->AutonTurn(0.5);

			LifterSetpoint = 0.5;
		}

		if(LifterDistance >= 150 && (WheelEncoder >= 100 && WheelEncoder <= 1000)){//move forward to 3rd tote
			drive->AutonDriveStraight(true, 0.5);
		}

		if(LifterDistance >= 150 && WheelEncoder >= 1000){
			autoMode = Stack2Totes;
		}
	}
	if(autoMode == Stack2Totes){

		if(WheelEncoder >= 1000){//when reaches 3rd tote, reset encoder value.
			drive->FLWheel->Reset();
		}

		if(LifterDistance >= 150 && WheelEncoder <= 100){//rotate -90 so 1st tote is above 3rd tote
			drive->AutonTurn(-0.5);		}

		if(LifterDistance >= 90 && WheelEncoder >= 100){//lowers totes onto 3rd tote
			LifterSetpoint = -0.5;
			drive->FLWheel->Reset();
		}

		if(((LifterDistance >= 0 && LifterDistance <= 95) && WheelEncoder >= -20)){//drive back to wheel <= -20 lift <= 0. set mode to step3
			drive->AutonDriveStraight(true, -0.5);

			if(!bottomLiftLS->Get()){
				LifterSetpoint = -1;
			}
		}
		if(LifterDistance <= 10 && WheelEncoder <= -20){
			autoMode = Lift3Totes;
		}
	}

	if(autoMode == Lift3Totes){

		if(!toteTouchSensor->Get()){//drive forward to tote stack.
			drive->AutonDriveStraight(true, 0.5);
			drive->FLWheel->Reset();
		}

		if(LifterDistance <= 110){//move totes up
			LifterSetpoint = 0.5;
		}

		if(WheelEncoder >= -200){
			drive->AutonDriveStraight(true, -0.5);
		}
		if(WheelEncoder <= -200 && (LifterDistance >= 0 && !bottomLiftLS->Get())){
			LifterSetpoint = -0.5;
		}

		if(WheelEncoder <= -200 && bottomLiftLS->Get()){
			autoMode = DriveToAutoZone;
		}
	}

	if(autoMode == DriveToAutoZone && bottomLiftLS->Get() && WheelEncoder >= -300){
		drive->AutonDriveStraight(true, -1);
	}

	lifter->liftMotor->Set(LifterSetpoint);
}

void AutonomousSystem::Run2ToteAuto(MecanumDrive *drive, LiftSystem *lifter){
	float LifterDistance = lifter->liftDistance->GetDistance();
	float WheelEncoder = drive->FLWheel->GetDistance();
	//float WheelEncoder = drive->FLMotor->GetPosition();

	float LifterSetpoint = 0;

	if(autoMode == Reset){
		if(bottomLiftLS->Get()){
			lifter->liftDistance->Reset();
			drive->FLWheel->Reset();

			lifterPID->ResetPID();

			LifterSetpoint = 0;

			autoMode = Lift1Tote;
		}
		else if(!bottomLiftLS->Get()){
			LifterSetpoint = -1;
		}
	}

	if(autoMode == Lift1Tote){

		if(LifterDistance <= 100){//lift tote up
			LifterSetpoint = 0.5;
		}

		if((LifterDistance >= 100 && LifterDistance <= 150) && WheelEncoder <= 100){//rotate left 90. and lift to 1.5 totes high lift > 150, wheel > 100
			drive->AutonTurn(0.5);

			LifterSetpoint = 0.5;
		}

		if(LifterDistance >= 150 && (WheelEncoder >= 100 && WheelEncoder <= 1000)){//drive Forward x distance to 2nd tote. and set to step 2.
			drive->AutonDriveStraight(true, 0.5);

			LifterSetpoint = 0;
		}

		if(LifterDistance >= 150 && WheelEncoder >= 1000){
			autoMode = Stack1Tote;
		}
	}

	if(autoMode == Stack1Tote){

		if(WheelEncoder >= 1000){//when reachs 2nd tote, reset encoder value.
			drive->FLWheel->Reset();
		}

		if(LifterDistance >= 150 && WheelEncoder <= 100){//rotate -90 so 1st tote is above 2nd tote
			drive->AutonTurn(-0.5);
		}

		if(LifterDistance >= 90 && WheelEncoder >= 100){//lowers 1st tote onto 2nd tote
			LifterSetpoint = -0.5;
			drive->FLWheel->Reset();
		}

		if(((LifterDistance >= 0 && LifterDistance <= 95) && WheelEncoder >= -20)){//drive back to wheel <= -20 lift <= 0. set mode to step3
			drive->AutonDriveStraight(true, -0.5);

			if(!bottomLiftLS->Get()){
				LifterSetpoint = -1;
			}
		}

		if(LifterDistance <= 10 && WheelEncoder <= -20){
			autoMode = Lift2Totes;
		}
	}

	if(autoMode == Lift2Totes){

		if(!toteTouchSensor->Get()){//drive forward to 2nd tote.
			drive->AutonDriveStraight(true, 0.5);
			drive->FLWheel->Reset();
		}

		if(LifterDistance <= 110){//move totes up
			LifterSetpoint = 0.5;
		}

		if(WheelEncoder >= -500){
			drive->AutonDriveStraight(true, -1);
		}

		if(WheelEncoder <= -500){
			drive->SetZero();
		}
	}

	lifter->liftMotor->Set(LifterSetpoint);
}
