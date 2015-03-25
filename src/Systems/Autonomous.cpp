#include "Autonomous.h"

AutonomousSystem::AutonomousSystem(){
	autoMode = LiftCan;
	toteLifterOutput = new ElevatorSpeedAlgorithm(0.3, 0.02, 25, 1, 1, 0.005, 5, 5);
	canLifterOutput = new ElevatorSpeedAlgorithm(0.3, 0.02, 10, 1, 0.5, 0.005, 5, 5);
	driveOutput = new ElevatorSpeedAlgorithm(0.3, 0.02, 25, 1, 0.8, 0.005, 5, 5);
	turnOutput = new ElevatorSpeedAlgorithm(0.3, 0.02, 1, 1, 1, 0.0001, 2, 50);//0.3, 0.02, 1, 1, 1, 0.0001, 2, 50 works at half speed
	timer = new Timer();//0.3, 0.005, 1, 1, 1, 0.0005, 2, 25 works at full speed but isnt full speed
}

AutonomousSystem::~AutonomousSystem(){
	delete toteLifterOutput;
	delete canLifterOutput;
	delete driveOutput;
	delete turnOutput;
	delete timer;
}

void AutonomousSystem::Run3Tote1CanAuto(MecanumDrive *drive, LiftSystem *lifter){
	float toteLifterDistance = lifter->toteLiftMotor1->GetPosition();
	float WheelEncoder = drive->FLMotor->GetPosition();
	float canLifterDistance = lifter->canLiftMotor->GetPosition();
	static bool TurnReached = false;

	float toteLifterSetpoint = 0;
	float canLifterSetpoint = 0;

	if(lifter->toteBottomLS->Get() == true){
		lifter->toteLiftMotor1->SetPosition(0);
	}
	if(lifter->canBottomLS->Get() == true){
		lifter->canLiftMotor->SetPosition(0);
	}

	printf("Wheel: %f\t Avg: %d\t Turn: %d\t Angle: %f\t Strafe: %d\t, TurnReach: %d\n", WheelEncoder, drive->AverageEncoders(), drive->AverageTurnRightEncoders(), drive->NavX->GetYaw(), drive->AverageLeftStrafe(), TurnReached);

	switch(autoMode){
	case LiftCan:
		if(drive->AverageEncoders() < 400){
			drive->AutonDriveStraight(true, -0.6);
			if(canLifterDistance < 550){
				canLifterSetpoint = 0.8;
			}
			else if(canLifterDistance >= 550){
				canLifterSetpoint = 0;
			}
			printf("canlift");
		}
		else if(drive->AverageEncoders() >= 400){
			drive->SetZero();
			autoMode = Rotate1Tote;
		}
		else{
			toteLifterSetpoint = 0;
			canLifterSetpoint = 0;
			drive->AutonTurn(0);
			drive->SetZero();
			lifter->SetZero();
			printf("Wrong1");
		}
		break;
	case Rotate1Tote:
		if(TurnReached == false){
			drive->AutonTurn(-driveOutput->ComputeNextMotorSpeedCommand(drive->AverageTurnRightEncoders(), 4325) / 1.5);
			if(toteLifterDistance < elevatorShortLevels[3]){
				toteLifterSetpoint = toteLifterOutput->ComputeNextMotorSpeedCommand(toteLifterDistance, elevatorShortLevels[3]);
			}
			timer->Start();
			if(timer->HasPeriodPassed(2.8)){
				TurnReached = true;
				drive->AutonTurn(0);
				drive->SetZero();
				toteLifterSetpoint = 0;
				timer->Stop();
				printf("true");
			}
			printf("turn");
		}
		else if(TurnReached == true){
			drive->AutonDriveStraight(false, -driveOutput->ComputeNextMotorSpeedCommand(WheelEncoder, 6640) / 2);
			if(WheelEncoder >= 6440){
				drive->SetZero();
				autoMode = Drop1Tote;
			}
			printf("drivefwd2");
		}
		else {
			toteLifterSetpoint = 0;
			canLifterSetpoint = 0;
			drive->AutonTurn(0);
			drive->SetZero();
			lifter->SetZero();
			printf("Wrong3");
		}
		break;
	case StrafeRight:
		if(drive->AverageLeftStrafe() > 100){
			drive->AutonDriveStraight(false, -0.4, true);
			printf("Strafe");
			if(drive->AverageLeftStrafe() <= 100){
				drive->SetZero();
				autoMode = Drop1Tote;
				printf("DropEntered1");
			}
		}
		else if(drive->AverageLeftStrafe() <= 100){
			drive->SetZero();
			autoMode = Drop1Tote;
			printf("DropEntered2");
		}
		else{
			toteLifterSetpoint = 0;
			canLifterSetpoint = 0;
			drive->AutonTurn(0);
			drive->SetZero();
			lifter->SetZero();
			printf("Wrong4");
		}
		break;
	case Drop1Tote:
		if(lifter->toteLiftMotor1->GetPosition() > 805){
			toteLifterSetpoint = -1;
			drive->SetZero();
			canLifterSetpoint = 0;
			printf("drop1tote");
		}
		else if(lifter->toteLiftMotor1->GetPosition() <= 805){
			toteLifterSetpoint = 0;
			drive->ResetEncoders();
			timer->Stop();
			autoMode = Lineup2Totes;
			printf("changed");
		}
		else {
			toteLifterSetpoint = 0;
			canLifterSetpoint = 0;
			drive->AutonTurn(0);
			drive->SetZero();
			lifter->SetZero();
			printf("Wrong5");
		}
		break;
	case Lineup2Totes:
		if(drive->AverageEncoders() < 400){
			drive->AutonDriveStraight(false, -0.5);
			timer->Stop();
			if(drive->AverageEncoders() >= 160){
				drive->SetZero();
				autoMode = DriveTo3Totes;
			}
			printf("lineuptotes");
		}
		else if(drive->AverageEncoders() >= 150 && toteLifterDistance >= 400){
			printf("EnterAutoRotate");
			timer->Stop();
			autoMode = DriveTo3Totes;
		}
		else {
			toteLifterSetpoint = 0;
			canLifterSetpoint = 0;
			drive->AutonTurn(0);
			drive->SetZero();
			lifter->SetZero();
			printf("Wrong6");
		}
		break;
	case DriveTo3Totes:
		if(drive->AverageEncoders() < 3600){
			if(drive->AverageEncoders() < 2200){
				drive->AutonDriveStraight(false, -0.7, false);
				if(drive->AverageEncoders() >= 1000 && drive->AverageEncoders() <= 2300){
					toteLifterSetpoint = -1;
				}
				else if(drive->AverageEncoders() >= 2300){
					toteLifterSetpoint = toteLifterOutput->ComputeNextMotorSpeedCommand(toteLifterDistance, elevatorShortLevels[3]);
				}
			}
			else if(drive->AverageEncoders() >= 2200){
				drive->AutonDriveStraight(false, -0.3);
				if(toteLifterDistance < 2800){
					toteLifterSetpoint = 0.85;
				}
				else if(toteLifterDistance >= 2800){
					toteLifterSetpoint = 0;
				}
			}
		}
		else if(drive->AverageEncoders() >= 3600){
			drive->SetZero();
			drive->ResetEncoders();
			toteLifterSetpoint = 0;
			autoMode = Lineup3Totes;
		}
		else{
			toteLifterSetpoint = 0;
			canLifterSetpoint = 0;
			drive->AutonTurn(0);
			drive->SetZero();
			lifter->SetZero();
			printf("Wrong7");
		}
		break;
	case Lineup3Totes:
		if(drive->AverageEncoders() < 3850){
			drive->AutonDriveStraight(false, -0.4);
			timer->Stop();
			if(drive->AverageEncoders() >= 600){
				drive->SetZero();
				autoMode = StrafeRight3Tote;
			}
			printf("lineuptotes");
		}
		else if(drive->AverageEncoders() >= 150 && toteLifterDistance >= 3850){
			printf("EnterAutoRotate");
			timer->Stop();
			autoMode = StrafeRight3Tote;
		}
		else {
			toteLifterSetpoint = 0;
			canLifterSetpoint = 0;
			drive->AutonTurn(0);
			drive->SetZero();
			lifter->SetZero();
			printf("Wrong6");
		}
		break;
	case StrafeRight3Tote:
		if(drive->AverageLeftStrafe() > -120){
			drive->AutonDriveStraight(false, -0.3, true);
			timer->Start();
			printf("Strafe");
			if(drive->AverageLeftStrafe() <= -120){
				drive->SetZero();
				if(timer->HasPeriodPassed(1)){
					autoMode = Stack3Totes;
				}
				printf("DropEntered1");
			}
		}
		else if(drive->AverageLeftStrafe() <= -120){
			drive->SetZero();
			if(timer->HasPeriodPassed(1)){
				autoMode = Stack3Totes;
			}
			printf("DropEntered2");
		}
		else{
			toteLifterSetpoint = 0;
			canLifterSetpoint = 0;
			drive->AutonTurn(0);
			drive->SetZero();
			lifter->SetZero();
			printf("Wrong4");
		}
		break;
	case Stack3Totes:
		if(lifter->toteLiftMotor1->GetPosition() > 805){
			toteLifterSetpoint = -0.8;
			drive->SetZero();
			canLifterSetpoint = 0;
			printf("drop3tote");
		}
		else if(lifter->toteLiftMotor1->GetPosition() <= 805){
			toteLifterSetpoint = 0;
			drive->ResetEncoders();
			timer->Stop();
			autoMode = RotateToAutoZone;
			printf("3totechanged");
		}
		else {
			toteLifterSetpoint = 0;
			canLifterSetpoint = 0;
			drive->AutonTurn(0);
			drive->SetZero();
			lifter->SetZero();
			printf("Wrong8");
		}
		break;
	case RotateToAutoZone:
		if(drive->AverageTurnLeftEncoders() < 2200){
			timer->Start();
			if(timer->HasPeriodPassed(0.05)){
				drive->AutonTurn(0.3);
				if(drive->AverageTurnLeftEncoders() >= 2200){
					drive->SetZero();
				}
			}
			printf("TurnToAutoZone");
		}
		else if(drive->AverageTurnLeftEncoders() >= 2200){
			drive->SetZero();
			autoMode = DriveToAutoZone;
		}
		else{
			toteLifterSetpoint = 0;
			canLifterSetpoint = 0;
			drive->AutonTurn(0);
			drive->SetZero();
			lifter->SetZero();
			printf("Wrong7");
		}
		break;
	case DriveToAutoZone:
		if(drive->AverageEncoders() < 3680){
			drive->AutonDriveStraight(false, -0.5);
			if(toteLifterDistance > 0){
				toteLifterSetpoint = -1;
			}
			else if(toteLifterDistance <= 0){
				toteLifterSetpoint = 0;
			}
			printf("DriveToAuto");
		}
		else if(drive->AverageEncoders() >= 3680){
			drive->SetZero();
			autoMode = Score3Totes;
			printf("InAuto");
		}
		break;
	case Score3Totes:
		if(drive->AverageEncoders() > 2000){
			drive->AutonDriveStraight(false, 0.5);
		}
		else if(drive->AverageEncoders() <= 2000){
			drive->SetZero();
		}
	}
	canLifterSetpoint += 0.05;
	if(lifter->canTopLS->Get() && canLifterSetpoint > 0){
		canLifterSetpoint = 0;
	}
	lifter->SetToteSpeed(toteLifterSetpoint);
	lifter->SetCanSpeed(canLifterSetpoint);
}

void AutonomousSystem::Run1Tote2CanAuto(MecanumDrive *drive, LiftSystem *lifter){
	float toteLifterDistance = lifter->toteLiftMotor1->GetPosition();
	float WheelEncoder = drive->FLMotor->GetPosition();
	float canLifterDistance = lifter->canLiftMotor->GetPosition();
	static bool TurnReached = false;

	float toteLifterSetpoint = 0;
	float canLifterSetpoint = 0;

	if(lifter->toteBottomLS->Get() == true){
		lifter->toteLiftMotor1->SetPosition(0);
	}
	if(lifter->canBottomLS->Get() == true){
		lifter->canLiftMotor->SetPosition(0);
	}

	printf("Wheel: %f\t Avg: %d\t Turn: %d\t Angle: %f\t Strafe: %d\t, TurnReach: %d\n", WheelEncoder, drive->AverageEncoders(), drive->AverageTurnRightEncoders(), drive->NavX->GetYaw(), drive->AverageLeftStrafe(), TurnReached);

	switch(autoMode){
	case LiftCan:
		if(canLifterDistance < elevatorCanLevels[2]){
			canLifterSetpoint = canLifterOutput->ComputeNextMotorSpeedCommand(canLifterDistance, elevatorCanLevels[5]);
			drive->ResetEncoders();
			if(canLifterDistance >= 1700){
				autoMode = Lift1Tote;
			}
			printf("canlift");
		}
		else if(canLifterDistance >= 1700 || lifter->canTopLS->Get()){
			autoMode = Lift1Tote;
		}
		else{
			toteLifterSetpoint = 0;
			canLifterSetpoint = 0;
			drive->AutonTurn(0);
			drive->SetZero();
			lifter->SetZero();
			printf("Wrong1");
		}
		break;
	case Lift1Tote:
		if((WheelEncoder < autonDrive1[1]) && (toteLifterDistance < elevatorShortLevels[3])){
			drive->AutonDriveStraight(false, driveOutput->ComputeNextMotorSpeedCommand(WheelEncoder, autonDrive1[1]) * -1);
			if(WheelEncoder > autonDrive1[1]){
				drive->AutonDriveStraight(false, 0);
			}
			printf("drivefwd");
		}
		else if(WheelEncoder >= autonDrive1[1] && toteLifterDistance < elevatorShortLevels[3]){
			toteLifterSetpoint = toteLifterOutput->ComputeNextMotorSpeedCommand(toteLifterDistance, elevatorShortLevels[4]);
			printf("lifttote");

		}
		else if((WheelEncoder >= autonDrive1[1]) && (WheelEncoder < autonDrive1[2]) && (toteLifterDistance >= (elevatorShortLevels[3]) - 40)){
			autoMode = Rotate1Tote;
			printf("StackEntered");
		}
		else {
			toteLifterSetpoint = 0;
			canLifterSetpoint = 0;
			drive->AutonTurn(0);
			drive->SetZero();
			lifter->SetZero();
			printf("Wrong2");
		}
		break;
	}
	canLifterSetpoint += 0.05;
	if(lifter->canTopLS->Get() && canLifterSetpoint > 0){
		canLifterSetpoint = 0;
	}
	lifter->SetToteSpeed(toteLifterSetpoint);
	lifter->SetCanSpeed(canLifterSetpoint);
}

void AutonomousSystem::Run2Tote1CanAuto(MecanumDrive *drive, LiftSystem *lifter){
	float toteLifterDistance = lifter->toteLiftMotor1->GetPosition();
	float WheelEncoder = drive->FLMotor->GetPosition();
	float canLifterDistance = lifter->canLiftMotor->GetPosition();
	static bool TurnReached = false;

	float toteLifterSetpoint = 0;
	float canLifterSetpoint = 0;

	if(lifter->toteBottomLS->Get() == true){
		lifter->toteLiftMotor1->SetPosition(0);
	}
	if(lifter->canBottomLS->Get() == true){
		lifter->canLiftMotor->SetPosition(0);
	}

	printf("Wheel: %f\t Avg: %d\t Turn: %d\t Angle: %f\t Strafe: %d\t, TurnReach: %d\n", WheelEncoder, drive->AverageEncoders(), drive->AverageTurnRightEncoders(), drive->NavX->GetYaw(), drive->AverageLeftStrafe(), TurnReached);

	switch(autoMode){
	case LiftCan:
		if(canLifterDistance < elevatorCanLevels[2]){
			canLifterSetpoint = canLifterOutput->ComputeNextMotorSpeedCommand(canLifterDistance, elevatorCanLevels[5]);
			drive->ResetEncoders();
			if(canLifterDistance >= 1700){
				autoMode = Lift1Tote;
			}
			printf("canlift");
		}
		else if(canLifterDistance >= 1700 || lifter->canTopLS->Get()){
			autoMode = Lift1Tote;
		}
		else{
			toteLifterSetpoint = 0;
			canLifterSetpoint = 0;
			drive->AutonTurn(0);
			drive->SetZero();
			lifter->SetZero();
			printf("Wrong1");
		}
		break;
	case Lift1Tote:
		if((WheelEncoder < autonDrive1[1]) && (toteLifterDistance < elevatorShortLevels[3])){
			drive->AutonDriveStraight(false, driveOutput->ComputeNextMotorSpeedCommand(WheelEncoder, autonDrive1[1]) * -1);
			if(WheelEncoder > autonDrive1[1]){
				drive->AutonDriveStraight(false, 0);
			}
			printf("drivefwd");
		}
		else if(WheelEncoder >= autonDrive1[1] && toteLifterDistance < elevatorShortLevels[3]){
			toteLifterSetpoint = toteLifterOutput->ComputeNextMotorSpeedCommand(toteLifterDistance, elevatorShortLevels[3]);
			printf("lifttote");

		}
		else if((WheelEncoder >= autonDrive1[1]) && (WheelEncoder < autonDrive1[2]) && (toteLifterDistance >= (elevatorShortLevels[3]) - 40)){
			autoMode = Rotate1Tote;
			printf("StackEntered");
		}
		else {
			toteLifterSetpoint = 0;
			canLifterSetpoint = 0;
			drive->AutonTurn(0);
			drive->SetZero();
			lifter->SetZero();
			printf("Wrong2");
		}
		break;

	case Rotate1Tote:
		if(TurnReached == false){
			drive->AutonTurn(-driveOutput->ComputeNextMotorSpeedCommand(drive->AverageTurnRightEncoders(), 4325) / 1.5);
			timer->Start();
			if(timer->HasPeriodPassed(2.8)){
				TurnReached = true;
				drive->AutonTurn(0);
				drive->SetZero();
				timer->Stop();
				printf("true");
			}
			printf("turn");
		}
		else if(TurnReached == true){
			drive->AutonDriveStraight(false, -driveOutput->ComputeNextMotorSpeedCommand(WheelEncoder, 6440) / 2);
			if(WheelEncoder >= 6440){
				drive->SetZero();
				autoMode = StrafeRight;
			}
			printf("drivefwd2");
		}
		else {
			toteLifterSetpoint = 0;
			canLifterSetpoint = 0;
			drive->AutonTurn(0);
			drive->SetZero();
			lifter->SetZero();
			printf("Wrong3");
		}
		break;
	case StrafeRight:
		if(drive->AverageLeftStrafe() > 100){
			drive->AutonDriveStraight(false, -0.4, true);
			printf("Strafe");
			if(drive->AverageLeftStrafe() <= 100){
				drive->SetZero();
				autoMode = Drop1Tote;
				printf("DropEntered1");
			}
		}
		else if(drive->AverageLeftStrafe() <= 100){
			drive->SetZero();
			autoMode = Drop1Tote;
			printf("DropEntered2");
		}
		else{
			toteLifterSetpoint = 0;
			canLifterSetpoint = 0;
			drive->AutonTurn(0);
			drive->SetZero();
			lifter->SetZero();
			printf("Wrong4");
		}
		break;
	case Drop1Tote:
		if(lifter->toteLiftMotor1->GetPosition() > 805){
			toteLifterSetpoint = -1;
			drive->SetZero();
			canLifterSetpoint = 0;
			printf("drop1tote");
		}
		else if(lifter->toteLiftMotor1->GetPosition() <= 805){
			toteLifterSetpoint = 0;
			drive->ResetEncoders();
			timer->Stop();
			autoMode = Lineup2Totes;
			printf("changed");
		}
		else {
			toteLifterSetpoint = 0;
			canLifterSetpoint = 0;
			drive->AutonTurn(0);
			drive->SetZero();
			lifter->SetZero();
			printf("Wrong5");
		}
		break;
	case Lineup2Totes:
		if(drive->AverageEncoders() < 300){
			drive->AutonDriveStraight(false, -0.5);
			timer->Stop();
			if(drive->AverageEncoders() >= 160){
				drive->SetZero();
				autoMode = RotateToAutoZone;
			}
			printf("lineuptotes");
		}
		else if(drive->AverageEncoders() >= 150 && toteLifterDistance >= 500){
			printf("EnterAutoRotate");
			timer->Stop();
			autoMode = RotateToAutoZone;
		}
		else {
			toteLifterSetpoint = 0;
			canLifterSetpoint = 0;
			drive->AutonTurn(0);
			drive->SetZero();
			lifter->SetZero();
			printf("Wrong6");
		}
		break;
	case RotateToAutoZone:
		if(drive->AverageTurnLeftEncoders() < 2200){
			timer->Start();
			if(timer->HasPeriodPassed(0.05)){
				drive->AutonTurn(0.4);
				if(drive->AverageTurnLeftEncoders() >= 2200){
					drive->SetZero();
				}
			}
			printf("TurnToAutoZone");
		}
		else if(drive->AverageTurnLeftEncoders() >= 2200){
			drive->SetZero();
			autoMode = DriveToAutoZone;
		}
		else{
			toteLifterSetpoint = 0;
			canLifterSetpoint = 0;
			drive->AutonTurn(0);
			drive->SetZero();
			lifter->SetZero();
			printf("Wrong7");
		}
		break;
	case DriveToAutoZone:
		if(drive->AverageEncoders() < 6280){
			drive->AutonDriveStraight(false, -0.5);
			printf("DriveToAuto");
		}
		else if(drive->AverageEncoders() >= 6280){
			drive->SetZero();
			autoMode = RotateInAutoZone;
			printf("InAuto");
		}
		break;
	case RotateInAutoZone:
		if(drive->AverageTurnRightEncoders() < -1000){
			drive->AutonTurn(-0.5);
			printf("RotateInAUto");
		}
		else if(drive->AverageTurnRightEncoders() >= -1000){
			drive->SetZero();
			printf("Done");
		}
		break;
	}
	canLifterSetpoint += 0.05;
	if(lifter->canTopLS->Get() && canLifterSetpoint > 0){
		canLifterSetpoint = 0;
	}
	lifter->SetToteSpeed(toteLifterSetpoint);
	lifter->SetCanSpeed(canLifterSetpoint);
}

void AutonomousSystem::Run1Tote1CanAuto(MecanumDrive *drive, LiftSystem *lifter){
	float toteLifterDistance = lifter->toteLiftMotor1->GetPosition();
	float WheelEncoder = drive->FLMotor->GetPosition();
	float canLifterDistance = lifter->canLiftMotor->GetPosition();
	static bool TurnReached = false;

	float toteLifterSetpoint = 0;
	float canLifterSetpoint = 0;

	if(lifter->toteBottomLS->Get() == true){
		lifter->toteLiftMotor1->SetPosition(0);
	}
	if(lifter->canBottomLS->Get() == true){
		lifter->canLiftMotor->SetPosition(0);
	}

	//printf("Tote: %f\t Can: %f\t Wheel: %f\t Avg: %d\t Turn: %d\t Angle: %f\t Strafe: %d\t, TurnReach: %d\n", toteLifterDistance, canLifterDistance, WheelEncoder, drive->AverageEncoders(), drive->AverageTurnRightEncoders(), drive->mecanumGyro->GetAngle(), drive->AverageLeftStrafe(), TurnReached);

	switch(autoMode){
	case LiftCan:
		if(canLifterDistance < elevatorCanLevels[2]){
			canLifterSetpoint = canLifterOutput->ComputeNextMotorSpeedCommand(canLifterDistance, elevatorCanLevels[5]);
			drive->ResetEncoders();
			if(canLifterDistance >= 1700){
				autoMode = Lift1Tote;
			}
			printf("canlift");
		}
		else if(canLifterDistance >= 1700 || lifter->canTopLS->Get()){
			autoMode = Lift1Tote;
		}
		else{
			toteLifterSetpoint = 0;
			canLifterSetpoint = 0;
			drive->AutonTurn(0);
			drive->SetZero();
			lifter->SetZero();
			printf("Wrong1");
		}
		break;
	case Lift1Tote:
		if((WheelEncoder < autonDrive1[1]) && (toteLifterDistance < elevatorShortLevels[3])){
			drive->AutonDriveStraight(false, driveOutput->ComputeNextMotorSpeedCommand(WheelEncoder, autonDrive1[1]) * -1);
			if(WheelEncoder > autonDrive1[1]){
				drive->AutonDriveStraight(false, 0);
			}
			printf("drivefwd");
		}
		else if(WheelEncoder >= autonDrive1[1] && toteLifterDistance < elevatorShortLevels[2]){
			toteLifterSetpoint = toteLifterOutput->ComputeNextMotorSpeedCommand(toteLifterDistance, elevatorShortLevels[2]);
			printf("lifttote");

		}
		else if((WheelEncoder >= autonDrive1[1]) && (WheelEncoder < autonDrive1[2]) && (toteLifterDistance >= (elevatorShortLevels[2]) - 40)){
			autoMode = Rotate1Tote;
			printf("StackEntered");
		}
		else {
			toteLifterSetpoint = 0;
			canLifterSetpoint = 0;
			drive->AutonTurn(0);
			drive->SetZero();
			lifter->SetZero();
			printf("Wrong2");
		}
		break;
	case Rotate1Tote:
		if(TurnReached == false){
			drive->AutonTurn(-drive->turnOutput->ComputeNextMotorSpeedCommand(drive->NavX->GetYaw(), 90) / 2);
			timer->Start();
			if(timer->HasPeriodPassed(5)){
				TurnReached = true;
				drive->AutonTurn(0);
				drive->SetZero();
				timer->Stop();
				printf("true");
			}
			printf("turn");
		}
		else if(TurnReached == true){
			drive->AutonDriveStraight(false, -driveOutput->ComputeNextMotorSpeedCommand(WheelEncoder, 8600));
			if(WheelEncoder >= autonDrive1[5]){
				drive->SetZero();
				autoMode = RotateInAutoZone;
				timer->Stop();
			}
			printf("drivefwd2");
		}
		else if(WheelEncoder >= autonDrive1[5]){
			drive->SetZero();
			autoMode = RotateInAutoZone;
			timer->Stop();
		}
		else{
			toteLifterSetpoint = 0;
			canLifterSetpoint = 0;
			drive->AutonTurn(0);
			drive->SetZero();
			lifter->SetZero();
			printf("Wrong3");
		}
		break;
	case RotateInAutoZone:
		if(drive->AverageTurnRightEncoders() < 3000){
			drive->AutonTurn(-0.5);
			printf("RotateInAUto");
		}
		else if(drive->AverageTurnRightEncoders() >= 3000){
			drive->SetZero();
			printf("Done");
		}
		break;
	}
	lifter->SetToteSpeed(toteLifterSetpoint);
	lifter->SetCanSpeed(canLifterSetpoint);

}
void AutonomousSystem::Run1CanPickup(MecanumDrive *drive, LiftSystem *lifter){
	float canLifterDistance = lifter->canLiftMotor->GetPosition();
	float canLifterSetpoint = 0;

	if(lifter->canBottomLS->Get() == true){
		lifter->canLiftMotor->SetPosition(0);
	}

	//printf("Avg: %d\n", drive->AverageEncoders());

	switch(autoMode){
	case LiftCan:
		if(canLifterDistance < elevatorCanLevels[5]){
			canLifterSetpoint = canLifterOutput->ComputeNextMotorSpeedCommand(canLifterDistance, elevatorCanLevels[5]);
			drive->ResetEncoders();
			if(canLifterDistance >= elevatorCanLevels[2]){
				autoMode = DriveToAutoZone;
			}
			printf("canlift");
		}
		else if(canLifterDistance >= 1000){
			autoMode = DriveToAutoZone;
			printf("Changed");
		}
		else{
			canLifterSetpoint = 0;
			drive->AutonTurn(0);
			drive->SetZero();
			lifter->SetZero();
			printf("Wrong1");
		}
		break;
	case DriveToAutoZone:
		if(drive->AverageEncoders() < 4770){
			drive->AutonDriveStraight(true, -0.5);
			if(drive->AverageEncoders() >= 4770){
				drive->SetZero();
			}
		}
		else if(drive->AverageEncoders() >= 4770){
			drive->SetZero();
			if(lifter->canBottomLS->Get() == true){
				canLifterSetpoint = 0;
				lifter->SetZero();
			}
		}
		else{
			canLifterSetpoint = 0;
			drive->AutonTurn(0);
			drive->SetZero();
			lifter->SetZero();
			printf("Wrong2");
		}
	}
	lifter->SetCanSpeed(canLifterSetpoint);
}
void AutonomousSystem::RunDriveForward(MecanumDrive *drive){
	switch(autoMode){
	case LiftCan:{
		autoMode = DriveToAutoZone;
		break;
	}
	case DriveToAutoZone:
		if(drive->AverageEncoders() < 4500){
			drive->AutonDriveStraight(true, 0.5);
			if(drive->AverageEncoders() >= 4500){
				drive->SetZero();
			}
		}
		else if(drive->AverageEncoders() >= 4500){
			drive->SetZero();
		}
		else {
			drive->AutonTurn(0);
			drive->SetZero();
			printf("Wrong1");
		}
		break;
	}
}

void AutonomousSystem::RunFast1Can(MecanumDrive *drive, LiftSystem *lifter){
	float canLifterDistance = lifter->canLiftMotor->GetPosition();
	float canLifterSetpoint = 0;

	if(lifter->canBottomLS->Get() == true){
		lifter->canLiftMotor->SetPosition(0);
	}

	//printf("Avg: %d\n", drive->AverageEncoders());

	switch(autoMode){
	case LiftCan:
		if(drive->AverageEncoders() < 4770){
			drive->AutonDriveStraight(true, -0.5);
			if(canLifterDistance < 550){
				canLifterSetpoint = 0.8;
			}
			else if(canLifterDistance > 550){
				canLifterSetpoint = 0;
			}
			printf("canlift");
		}
		else if(drive->AverageEncoders() >= 4770){
			drive->SetZero();
		}
		else{
			canLifterSetpoint = 0;
			drive->AutonTurn(0);
			drive->SetZero();
			lifter->SetZero();
			printf("Wrong1");
		}
		break;
	}
	if(canLifterDistance > 550 && canLifterSetpoint > 0){
		canLifterSetpoint = 0;
	}
	lifter->SetCanSpeed(canLifterSetpoint);

}
void AutonomousSystem::RunNothing(MecanumDrive *drive, LiftSystem *lifter){
	drive->SetZero();
	lifter->SetZero();
}
