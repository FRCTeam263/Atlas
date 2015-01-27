#include "WPILib.h"
#include "Systems/MecanumDrive.h"
#include "Systems/LiftSystem.h"
#include "Systems/CanGrabber.h"
#include "Systems/Autonomous.h"

const int numberOfElevatorLevels = 7;
const int elevatorLevels[numberOfElevatorLevels] = {0, 800, 1000, 1800, 2000,
		2800, 3000};

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

		CameraServer::GetInstance()->SetQuality(50);
		CameraServer::GetInstance()->StartAutomaticCapture("cam0");
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
			lifter->TestLifter(gamePad);
			printf("Encoder: %f\n", lifter->longLiftMotor1->GetPosition() * -1);
			//lifter->RunShortLift(gamePad);
			lifter->RunLongLift(gamePad);
			//grabber->Extend(drivePad);
			/*while (gamePad->GetRawButton(0) == false) {
				if (gamePad->GetRawButton(1)) {
					if (++currentElevatorLevel > numberOfElevatorLevels)
						currentElevatorLevel = numberOfElevatorLevels;
				}

				if (gamePad->GetRawButton(2)) {
					if (--currentElevatorLevel < 0)
						currentElevatorLevel = 0;
				}

				lifter->SetSpeed(lifter->ElevatorSpeed(elevatorLevels[currentElevatorLevel]),
						true, false);

			}*/
		}
	}
};

START_ROBOT_CLASS(Omega);
