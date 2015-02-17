#include "WPILib.h"
#include "Systems/MecanumDrive.h"
#include "Systems/LiftSystem.h"
#include "Systems/Autonomous.h"

class Omega: public SampleRobot
{
public:
	Joystick *drivePad;
	Joystick *gamePad;
	Joystick *autonMode;

	MecanumDrive *drive;
	LiftSystem *lifter;

	/*DigitalOutput *vexSonicTrigger;
	DigitalInput *vexSonicEchoDetector;
	Ultrasonic *sonicSensor;*/

	AutonomousSystem *auton;

	Omega()
	{
		drivePad = new Joystick(0);
		gamePad = new Joystick(1);
		autonMode = new Joystick(2);

		drive = new MecanumDrive();
		lifter = new LiftSystem();

		auton = new AutonomousSystem();

		/*vexSonicTrigger = new DigitalOutput(8);		// labeled input on vex
		vexSonicEchoDetector = new DigitalInput(9);	// labeled output on vex
		sonicSensor = new Ultrasonic(vexSonicTrigger, vexSonicEchoDetector);

		sonicSensor->SetAutomaticMode(true);*/

		/*CameraServer::GetInstance()->SetQuality(25);
		CameraServer::GetInstance()->StartAutomaticCapture("cam0");*/
	}

	~Omega(){
		delete drivePad;
		delete gamePad;
		delete autonMode;
		delete drive;
		delete lifter;
		delete auton;
	}

	void Autonomous()
	{
		drive->mecanumGyro->Reset();
		while(IsAutonomous() && IsEnabled()){
			//drive->AutonDriveStraight(false, 0.5, true);
			//drive->AutonDriveStraight(true, 0.5);
			auton->Run3ToteAuto(drive, lifter);
			//auton->Run1Tote1CanAuto(drive, lifter);
			//drive->AutonTurn(-auton->turnOutput->ComputeNextMotorSpeedCommand(drive->mecanumGyro->GetAngle(), 174.5));
			//printf("Angle: %f\n", drive->mecanumGyro->GetAngle());
		}
	}

	void OperatorControl()
	{
		while (IsOperatorControl() && IsEnabled())
		{
			drive->Drive(drivePad);
			lifter->RunLifter(gamePad);
			printf("Tote: %f\n", lifter->shortLiftMotor1->Get());
			//printf("Angle: %f\n", drive->mecanumGyro->GetAngle());
			//printf("FL: %f\t FR: %f \t BL: %f\t BR: %f\t Avg: %d\n", drive->FLMotor->GetPosition(), drive->FRMotor->GetPosition(), drive->BLMotor->GetPosition(), drive->BRMotor->GetPosition(), drive->AverageLeftStrafe());
			//printf("WideEncoder: %f\t ShortEncoder: %f\n", lifter->canLiftMotor->GetPosition(), lifter->shortLiftMotor1->GetPosition());
		}
	}
};

START_ROBOT_CLASS(Omega);
