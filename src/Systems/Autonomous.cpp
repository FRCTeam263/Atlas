#include "Autonomous.h"

AutonomousSystem::AutonomousSystem(){
	autoMode = LiftCan;
	toteLifterOutput = new ElevatorSpeedAlgorithm(0.3, 0.02, 25, 1, 1, 0.005, 5, 5);
	canLifterOutput = new ElevatorSpeedAlgorithm(0.3, 0.02, 10, 1, 0.5, 0.005, 5, 5);
	driveOutput = new ElevatorSpeedAlgorithm(0.3, 0.02, 25, 1, 1, 0.005, 5, 5);
	turnOutput = new ElevatorSpeedAlgorithm(0.3, 0.005, 1, 1, 1, 0.0005, 2, 25);//0.3, 0.02, 1, 1, 1, 0.0001, 2, 50 works at half speed
	timer = new Timer();//0.3, 0.005, 1, 1, 1, 0.0005, 2, 25 works at full speed but isnt full speed
}

AutonomousSystem::~AutonomousSystem(){
	delete toteLifterOutput;
	delete canLifterOutput;
	delete driveOutput;
	delete turnOutput;
	delete timer;
}

void AutonomousSystem::Run3ToteAuto(MecanumDrive *drive, LiftSystem *lifter){
	float toteLifterDistance = lifter->shortLiftMotor1->GetPosition();
	float WheelEncoder = drive->FLMotor->GetPosition();
	float canLifterDistance = lifter->canLiftMotor->GetPosition();
	static bool TurnReached = false;

	float toteLifterSetpoint = 0;
	float canLifterSetpoint = 0;

	if(lifter->shortBottomLS->Get() == true){
		lifter->shortLiftMotor1->SetPosition(0);
	}
	if(lifter->canBottomLS->Get() == true){
		lifter->canLiftMotor->SetPosition(0);
	}

	printf("Tote: %f\t Can: %f\t Wheel: %f\t Avg: %d\t Turn: %d\t Angle: %f\t Strafe: %d\t, TurnReach: %d\n", toteLifterDistance, canLifterDistance, WheelEncoder, drive->AverageEncoders(), drive->AverageTurnRightEncoders(), drive->mecanumGyro->GetAngle(), drive->AverageLeftStrafe(), TurnReached);

	switch(autoMode){
	case Reset:
		if(lifter->shortBottomLS->Get() == true && lifter->canBottomLS->Get() == true){
			lifter->shortLiftMotor1->SetPosition(0);
			drive->FLMotor->SetPosition(0);

			toteLifterSetpoint = 0;
			canLifterSetpoint = 0;

			autoMode = Lift1Tote;
		}
		else if(lifter->shortBottomLS->Get() == false){
			toteLifterSetpoint = -1;
		}
		else if(lifter->canBottomLS->Get() == false){
			canLifterSetpoint = -1;
		}
		drive->FLMotor->SetPosition(0);
		drive->FRMotor->SetPosition(0);
		drive->BLMotor->SetPosition(0);
		drive->BRMotor->SetPosition(0);
		break;
	case LiftCan:
		if(canLifterDistance < elevatorCanLevels[5]){
			canLifterSetpoint = canLifterOutput->ComputeNextMotorSpeedCommand(canLifterDistance, elevatorCanLevels[3]);
			drive->ResetEncoders();
			drive->mecanumGyro->Reset();
			if(canLifterDistance >= elevatorCanLevels[3]){
				autoMode = Lift1Tote;
			}
			printf("canlift");
		}
		else{
			drive->SetZero();
			lifter->SetZero();
		}
		break;
	case Lift1Tote:
		if((canLifterDistance >= elevatorCanLevels[5]) && (WheelEncoder < autonDrive1[1]) && (toteLifterDistance < elevatorShortLevels[3])){
			drive->AutonDriveStraight(true, driveOutput->ComputeNextMotorSpeedCommand(WheelEncoder, autonDrive1[1]) * -1);
			if(WheelEncoder > autonDrive1[1]){
				drive->AutonDriveStraight(false, 0);
			}
			printf("drivefwd");
		}
		else if(canLifterDistance >= elevatorCanLevels[5] && WheelEncoder >= autonDrive1[1] && toteLifterDistance < elevatorShortLevels[3]){

			toteLifterSetpoint = toteLifterOutput->ComputeNextMotorSpeedCommand(toteLifterDistance, elevatorShortLevels[3]);
			printf("lifttote");

		}
		else if((canLifterDistance >= elevatorCanLevels[5]) && (WheelEncoder >= autonDrive1[1]) && (WheelEncoder < autonDrive1[2]) && (toteLifterDistance >= (elevatorShortLevels[3]) - 40)){
			autoMode = Stack1Tote;
			printf("StackEntered");
		}
		else {
			drive->SetZero();
			lifter->SetZero();
		}
		break;

	case Stack1Tote:
		if(((WheelEncoder >= autonDrive1[1]) && ((drive->AverageTurnRightEncoders() < 6000) || (drive->mecanumGyro->GetAngle() < 360))) && TurnReached == false){
			drive->AutonTurn(-turnOutput->ComputeNextMotorSpeedCommand(drive->mecanumGyro->GetAngle(), 175));
			timer->Start();
			if(timer->HasPeriodPassed(4)){
				TurnReached = true;
				drive->AutonTurn(0);
				drive->SetZero();
				timer->Stop();
				printf("true");
			}
			printf("turn");
		}
		else if(TurnReached == true){
			drive->AutonDriveStraight(false, -driveOutput->ComputeNextMotorSpeedCommand(WheelEncoder, autonDrive1[4])/1.5);
			if(WheelEncoder >= autonDrive1[4]){
				drive->SetZero();
				autoMode = LineUp2Tote;
			}
			printf("drivefwd2");
		}
		break;
	case LineUp2Tote:
		if(drive->AverageLeftStrafe() < 185){
			drive->AutonDriveStraight(false, 0.3, true);
			printf("Strafe");
			if(drive->AverageLeftStrafe() >= 185){
				drive->SetZero();
				autoMode = Drop1Tote;
				printf("DropEntered1");
			}
		}
		else if(drive->AverageLeftStrafe() >= 185){
			drive->SetZero();
			autoMode = Drop1Tote;
			printf("DropEntered2");
		}
		else{
			drive->SetZero();
			lifter->SetZero();
		}
		break;
	case Drop1Tote:
		if(WheelEncoder >= autonDrive1[3] && toteLifterDistance >= elevatorShortLevels[0]){
			toteLifterSetpoint = -1;
			drive->SetZero();
			if(lifter->shortBottomLS->Get() == true){
				toteLifterSetpoint = 0;
				drive->ResetEncoders();
				autoMode = Lift2Totes;
				printf("changed");
			}
			printf("drop1tote");
		}
		else{
			lifter->SetZero();
			drive->SetZero();
		}
		break;
	case Lift2Totes:
		if(drive->AverageEncoders() < autonDrive2[1]){
			drive->AutonDriveStraight(false, -0.5);
			if(drive->AverageEncoders() >= autonDrive2[1]){
				drive->SetZero();
			}
			printf("lineuptotes");
		}
		else if(drive->AverageEncoders() >= autonDrive2[1] && toteLifterDistance < elevatorShortLevels[5]){
			drive->SetZero();
			toteLifterSetpoint = toteLifterOutput->ComputeNextMotorSpeedCommand(toteLifterDistance, elevatorShortLevels[5]);
			if(toteLifterDistance > elevatorShortLevels[5]){
				toteLifterSetpoint = 0;
			}
			printf("lift2totes");
		}
		else if(drive->AverageEncoders() >= autonDrive2[1] && toteLifterDistance >= elevatorShortLevels[5]){
			printf("EnterCan");
			timer->Stop();
			autoMode = DriveTo3rdTote;
		}
		else{
			drive->SetZero();
			lifter->SetZero();
		}
		break;
	/*case RotateTo3Tote:
		if(){

		}
		else{
			drive->SetZero();
			lifter->SetZero();
		}
		break;*/
	/*case DriveTo3rdTote:
		if(true){
			drive->AutonTurn(0.5);
			if(drive->mecanumGyro->GetAngle() <= 0){
				drive->SetZero();
				drive->AutonTurn(0);
			}
			timer->Start();
			if(timer->HasPeriodPassed(3)){
				TurnReached = false;
				drive->SetZero();
				printf("false");
			}
			printf("turn2");
		}
		else{
			drive->SetZero();
			lifter->SetZero();
		}
		break;*/
		/*
	case KnockOverCan:
		if(drive->mecanumGyro->GetAngle() > 95){
			drive->AutonTurn(0.3);
			if(drive->mecanumGyro->GetAngle() <= 95){
				drive->SetZero();
			}
		}
		else{
			drive->SetZero();
			lifter->SetZero();
		}
*/
		/*
		else if((WheelEncoder >= autonDrive[3]) && (WheelEncoder < autonDrive[4]) && toteLifterDistance == 0){
			drive->AutonDriveStraight(false, -driveOutput->ComputeNextMotorSpeedCommand(WheelEncoder, autonDrive[4]));
			printf("linetotes");
		}*/

		/*else if((canLifterDistance >= elevatorCanLevels[4]) && (WheelEncoder > autonDrive[2] && WheelEncoder < autonDrive[3]) && (toteLifterDistance >= elevatorShortLevels[2])){
			drive->AutonDriveStraight(false, driveOutput->ComputeNextMotorSpeedCommand(WheelEncoder, autonDrive[3]));
			if(WheelEncoder > autonDrive[3]){
				drive->SetZero();
			}
		}*/
		/*if(toteLifterDistance <= elevatorShortLevels[1]){//lift tote up
			toteLifterSetpoint = toteLifterOutput->ComputeNextMotorSpeedCommand(toteLifterDistance, elevatorShortLevels[2]);
		}*/

		/*else if((LifterDistance >= elevatorShortLevels[2] && LifterDistance <= elevatorShortLevels[3]) && WheelEncoder <= 100){//rotate left 90. and lift to 1.5 totes high lift > 150, wheel > 100
			drive->AutonTurn(0.5);

			toteLifterSetpoint = elevatorShortLevels[3];
		}
		break;*/
	}

	lifter->SetToteSpeed(toteLifterSetpoint);
	lifter->SetCanSpeed(canLifterSetpoint);
}

void AutonomousSystem::Run1Tote1CanAuto(MecanumDrive *drive, LiftSystem *lifter){
	float toteLifterDistance = lifter->shortLiftMotor1->GetPosition();
	float WheelEncoder = drive->FLMotor->GetPosition();
	float canLifterDistance = lifter->canLiftMotor->GetPosition();
	static bool TurnReached = false;

	float toteLifterSetpoint = 0;
	float canLifterSetpoint = 0;

	if(lifter->shortBottomLS->Get() == true){
		lifter->shortLiftMotor1->SetPosition(0);
	}
	if(lifter->canBottomLS->Get() == true){
		lifter->canLiftMotor->SetPosition(0);
	}

	printf("Tote: %f\t Can: %f\t Wheel: %f\t Avg: %d\t Turn: %d\t Angle: %f\t Strafe: %d\t, TurnReach: %d\n", toteLifterDistance, canLifterDistance, WheelEncoder, drive->AverageEncoders(), drive->AverageTurnRightEncoders(), drive->mecanumGyro->GetAngle(), drive->AverageLeftStrafe(), TurnReached);

	switch(autoMode){
	case Reset:
		if(lifter->shortBottomLS->Get() == true && lifter->canBottomLS->Get() == true){
			lifter->shortLiftMotor1->SetPosition(0);
			drive->FLMotor->SetPosition(0);

			toteLifterSetpoint = 0;
			canLifterSetpoint = 0;

			autoMode = Lift1Tote;
		}
		else if(lifter->shortBottomLS->Get() == false){
			toteLifterSetpoint = -1;
		}
		else if(lifter->canBottomLS->Get() == false){
			canLifterSetpoint = -1;
		}
		drive->FLMotor->SetPosition(0);
		drive->FRMotor->SetPosition(0);
		drive->BLMotor->SetPosition(0);
		drive->BRMotor->SetPosition(0);
		break;

	case Lift1Tote:
		if(canLifterDistance < elevatorCanLevels[4]){
			canLifterSetpoint = canLifterOutput->ComputeNextMotorSpeedCommand(canLifterDistance, elevatorCanLevels[4]);
			drive->ResetEncoders();
			drive->mecanumGyro->Reset();
			printf("canlift");
		}
		else if((canLifterDistance >= elevatorCanLevels[4]) && (WheelEncoder < autonDrive1[1]) && (toteLifterDistance < elevatorShortLevels[3])){

			drive->AutonDriveStraight(true, driveOutput->ComputeNextMotorSpeedCommand(WheelEncoder, autonDrive1[1]) * -1);
			if(WheelEncoder > autonDrive1[1]){
				drive->AutonDriveStraight(false, 0);
			}
			printf("drivefwd");
		}
		else if(canLifterDistance >= elevatorCanLevels[4] && WheelEncoder >= autonDrive1[1] && toteLifterDistance < elevatorShortLevels[3]){

			toteLifterSetpoint = toteLifterOutput->ComputeNextMotorSpeedCommand(toteLifterDistance, elevatorShortLevels[3]);
			printf("lifttote");

		}
		else if((canLifterDistance >= elevatorCanLevels[4]) && (WheelEncoder >= autonDrive1[1]) && (WheelEncoder < autonDrive1[2]) && (toteLifterDistance >= (elevatorShortLevels[3]) - 40)){
			autoMode = Stack1Tote;
			printf("StackEntered");
		}
		else {
			drive->SetZero();
			lifter->SetZero();
		}
		break;

	case Stack1Tote:
		if(((WheelEncoder >= autonDrive1[1]) && ((drive->AverageTurnRightEncoders() < autonDrive1[2]) || (drive->mecanumGyro->GetAngle() < 220))) && TurnReached == false){
			drive->AutonTurn(-turnOutput->ComputeNextMotorSpeedCommand(drive->mecanumGyro->GetAngle(), 90)/2);
			timer->Start();
			if(timer->HasPeriodPassed(2.5)){
				TurnReached = true;
				drive->AutonTurn(0);
				drive->SetZero();
				timer->Stop();
				printf("true");
			}
			printf("turn");
		}
		else if(TurnReached == true){
			drive->AutonDriveStraight(false, -driveOutput->ComputeNextMotorSpeedCommand(WheelEncoder, autonDrive1[5]) / 2);
			if(WheelEncoder >= autonDrive1[5]){
				drive->SetZero();
				autoMode = Drop1Tote;
				timer->Stop();
			}
			printf("drivefwd2");
		}
		break;
	case Drop1Tote:
			if(true){
				drive->AutonTurn(-turnOutput->ComputeNextMotorSpeedCommand(drive->mecanumGyro->GetAngle(), 180) / 2);
			}
			if(lifter->shortBottomLS->Get() == false && lifter->canBottomLS->Get() == false){
				toteLifterSetpoint = 0.8;
				canLifterSetpoint = 0.4;
			}
			break;
	}

	lifter->SetToteSpeed(toteLifterSetpoint);
	lifter->SetCanSpeed(canLifterSetpoint);

}

void AutonomousSystem::RunNothing(MecanumDrive *drive, LiftSystem *lifter){
	drive->SetZero();
	lifter->SetZero();
}
