#include "WPILib.h"
#include "Systems/MecanumDrive.h"
#include "Systems/LiftSystem.h"
#include "Systems/Autonomous.h"

class Omega: public SampleRobot
{
public:
	Joystick *drivePad;
	Joystick *gamePad;

	MecanumDrive *drive;
	LiftSystem *lifter;

	AutonomousSystem *auton;

	DigitalInput *auto2tote1Can;
	DigitalInput *auto1Tote1Can;
	DigitalInput *auto1Can;
	DigitalInput *autoDriveFwd;

	Omega()
	{
		drivePad = new Joystick(0);
		gamePad = new Joystick(1);

		drive = new MecanumDrive();
		lifter = new LiftSystem();

		auton = new AutonomousSystem();
		autoDriveFwd = new DigitalInput(6);
		auto1Can = new DigitalInput(7);
		auto1Tote1Can = new DigitalInput(8);
		auto2tote1Can = new DigitalInput(9);


		CameraServer::GetInstance()->SetQuality(50);
		CameraServer::GetInstance()->StartAutomaticCapture("cam0");
	}

	~Omega(){
		delete drivePad;
		delete gamePad;
		delete drive;
		delete lifter;
		delete auton;
	}

	void Autonomous()
	{
		drive->mecanumGyro->Reset();
		while(IsAutonomous() && IsEnabled()){
			if(drive->AverageLeftStrafe() > -150){
				drive->AutonDriveStraight(false, -0.4, true);
			}
			else if(drive->AverageLeftStrafe() <= -150){
				drive->AutonDriveStraight(false, 0);
			}
			/*if(auto2tote1Can->Get() == 0){
				auton->Run2Tote1CanAuto(drive, lifter);
			}
			else if(auto1Tote1Can->Get() == 0){
				auton->Run1Tote1CanAuto(drive, lifter);
			}
			else if(auto1Can->Get() == 0){
				auton->Run1CanPickup(drive, lifter);
			}
			else if(autoDriveFwd->Get() == 0){
				auton->RunDriveForward(drive);
			}
			else{
				auton->RunNothing(drive, lifter);
			}*/
			//printf("Angle: %f\n", drive->mecanumGyro->GetAngle());
		}
	}

	void OperatorControl()
	{
		while (IsOperatorControl() && IsEnabled())
		{
			drive->CalibrateNavX();
			drive->Drive(drivePad);
			lifter->RunLifter(gamePad, drivePad);
			printf("StrafeLeft: %d\n", drive->AverageLeftStrafe());
			//printf("Tote: %f\n", lifter->shortLiftMotor1->Get());
			//printf("Angle: %f\n", drive->mecanumGyro->GetAngle());
			//printf("FL: %f\t FR: %f \t BL: %f\t BR: %f\t Avg: %d\n", drive->FLMotor->GetPosition(), drive->FRMotor->GetPosition(), drive->BLMotor->GetPosition(), drive->BRMotor->GetPosition(), drive->AverageLeftStrafe());
			//printf("WideEncoder: %f\t ShortEncoder: %f\n", lifter->canLiftMotor->GetPosition(), lifter->shortLiftMotor1->GetPosition());
		}
	}
};

START_ROBOT_CLASS(Omega);
