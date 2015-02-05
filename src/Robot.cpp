#include "WPILib.h"
#include "Systems/MecanumDrive.h"
#include "Systems/LiftSystem.h"
#include "Systems/CanGrabber.h"
#include "Systems/Autonomous.h"

class Omega: public SampleRobot
{
public:
	Joystick *drivePad;
	Joystick *gamePad;
	Joystick *autonMode;

	MecanumDrive *drive;
	LiftSystem *lifter;
	CanGrabber *grabber;

	AutonomousSystem *auton;

	Omega()
	{
		drivePad = new Joystick(0);
		gamePad = new Joystick(1);
		autonMode = new Joystick(2);

		drive = new MecanumDrive();
		lifter = new LiftSystem();
		grabber = new CanGrabber();

		auton = new AutonomousSystem();

		/*CameraServer::GetInstance()->SetQuality(50);
		CameraServer::GetInstance()->StartAutomaticCapture("cam0");*/
	}

	~Omega(){
		delete drivePad;
		delete gamePad;
		delete autonMode;
		delete drive;
		delete lifter;
		delete grabber;
		delete auton;
	}

	void Autonomous()
	{
		while(IsAutonomous() && IsEnabled()){
			if(autonMode->GetRawButton(1) == true){
				auton->Run3ToteAuto(drive, lifter);
			}
			else{
				auton->RunNothing(drive, lifter);
			}
		}
	}

	void OperatorControl()
	{
		while (IsOperatorControl() && IsEnabled())
		{
			drive->Drive(drivePad);
			lifter->RunLifter(gamePad);
			//lifter->TestLifter(gamePad);
			//printf("WideEncoder: %f\t ShortEncoder: %f\n", lifter->canLiftMotor->GetPosition(), lifter->shortLiftMotor1->GetPosition());
			//grabber->Extend(drivePad);
		}
	}
};

START_ROBOT_CLASS(Omega);
