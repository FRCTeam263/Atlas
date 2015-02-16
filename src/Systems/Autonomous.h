#ifndef SRC_SYSTEMS_AUTONOMOUS_H_
#define SRC_SYSTEMS_AUTONOMOUS_H_

#include "WPILib.h"
#include "LiftSystem.h"
#include "MecanumDrive.h"
#include "ElevatorSpeedAlgorithm.h"

class AutonomousSystem{
public:
	AutonomousSystem(void);
	~AutonomousSystem(void);

	void Run3ToteAuto(MecanumDrive *drive, LiftSystem *lifter);
	void Run1Tote1CanAuto(MecanumDrive *drive, LiftSystem *lifter);
	void RunNothing(MecanumDrive *drive, LiftSystem *lifter);

	ElevatorSpeedAlgorithm *turnOutput;

private:
	typedef enum{Reset, Lift1Tote, Stack1Tote, Lift2Totes, Stack2Totes, Lift3Totes, DriveToAutoZone, Drop1Tote, DriveTo3rdTote, LineUp2Tote, RotateTo3Tote} AutoMode;

	AutoMode autoMode;
	ElevatorSpeedAlgorithm *toteLifterOutput;
	ElevatorSpeedAlgorithm *canLifterOutput;
	ElevatorSpeedAlgorithm *driveOutput;
	Timer *timer;

};



#endif /* SRC_SYSTEMS_AUTONOMOUS_H_ */
