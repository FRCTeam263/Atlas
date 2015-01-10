#include "WPILib.h"
#include "Systems/MecanumDrive.h"
#include "Systems/LiftSystem.h"
#include "Systems/Autonomous.h"

Joystick *drivePad;
Joystick *gamePad;
Joystick *autonMode;
MecanumDrive *drive;
LiftSystem *lifter;

AutonomousSystem *auton;

class Robot: public SampleRobot
{
public:
	Robot()
	{
		drivePad = new Joystick(0);
		gamePad = new Joystick(1);
		autonMode = new Joystick(2);

		drive = new MecanumDrive();
		lifter = new LiftSystem();

		auton = new AutonomousSystem();
	}

	void Autonomous()
	{
		while(IsAutonomous() && IsEnabled()){

			if(autonMode->GetRawButton(1)){
				auton->Run3ToteAuto(drive, lifter);
			}

			else if(autonMode->GetRawButton(2)){

			}
		}

		while(IsOperatorControl() && IsDisabled()){
			lifter->ResetLifter();
			drive->SetZero();
		}
	}

	void OperatorControl()
	{
		while (IsOperatorControl() && IsEnabled())
		{
			drive->Drive(drivePad);
			lifter->RunLift(drivePad);
		}

		while(IsOperatorControl() && IsDisabled()){
			lifter->ResetLifter();
			drive->SetZero();
		}
	}
};

START_ROBOT_CLASS(Robot);
