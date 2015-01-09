#include "WPILib.h"
#include "Systems/MecanumDrive.h"
#include "Systems/LiftSystem.h"

Joystick *drivePad;
Joystick *gamePad;
MecanumDrive *drive;
LiftSystem *lifter;

class Robot: public SampleRobot
{
public:
	Robot()
	{
		drivePad = new Joystick(0);
		gamePad = new Joystick(1);
		drive = new MecanumDrive();
		lifter = new LiftSystem();
	}

	void Autonomous()
	{
	}

	void OperatorControl()
	{
		while (IsOperatorControl() && IsEnabled())
		{
			drive->Drive(drivePad);
			lifter->RunLift(drivePad);
		}
	}
};

START_ROBOT_CLASS(Robot);
