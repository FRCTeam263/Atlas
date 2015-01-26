#include "WPILib.h"
#include "Systems/MecanumDrive.h"
#include "Systems/LiftSystem.h"
#include "Systems/CanGrabber.h"
#include "Systems/Autonomous.h"

const int numberOfElevatorLevels = 7;
const int elevatorLevels[numberOfElevatorLevels] = { 0, 800, 1000, 1800, 2000,
		2800, 3000 };

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
			printf("Encoder: %f\n", lifter->longLiftMotor1->GetPosition() * -1);
			while((lifter->longLiftMotor1->GetPosition() * -1) < 800){
				lifter->SetSpeed(-0.5, true, false);
			}
			lifter->SetSpeed(0, false, false, true);
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



/*Pokemon Battle 2015
 *
 * A wild Mercado appeared.
 * Trainer Bryan sent out Chris.
 * Go Chris!
 * Mercado used splash.
 * It does nothing.
 * Chris used RKO.
 * It's super effective.
 * Mercado fainted.
 * Chris gains 1337 EXP.
 *
 *
 *
 * A while later....
 * Trainer Josh challenges Trainer Bryan.
 * "I'm Asian!" - Trainer Josh
 * Trainer Josh sent out Raj.
 * Trainer Bryan sent out Mr. Wentzel
 * Raj used CAD
 * It failed.
 * Mr. Wentzel used Long Speech
 * Raj fell asleep.
 * Trainer Josh recalls Raj.
 * Come back Raj!
 * Trainer Josh sent out Mercado.
 * Go show them your worth Mercado!
 * Mr. Wentzel used Salad.
 * Mercado is poisoned.
 * Mercado is hurt by the poison.
 * Mercado used Eat Bread.
 * He is no longer effected by poison and gains full HP.
 * Mr. Wentzel used Extensive Knowledge of Aeronautical Engineering.
 * It's a Critical Hit!
 * Mercado fainted.
 * Trainer Josh sent out Mateo.
 * Show them dis dik Mateo!
 * Trainer Bryan recalls Mr. Wentzel.
 * Come back Mr. Wentzel!
 * Trainer Bryan sent out Chris.
 * Mateo used Racist Jokes.
 *
 */
