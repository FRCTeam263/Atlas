/*
 * Autonomous.h
 *
 *  Created on: Jan 9, 2015
 *      Author: Administrator
 */

#ifndef SRC_SYSTEMS_AUTONOMOUS_H_
#define SRC_SYSTEMS_AUTONOMOUS_H_

#include "WPILib.h"
#include "LiftSystem.h"
#include "MecanumDrive.h"

class AutonomousSystem{
public:
	AutonomousSystem(void);
	~AutonomousSystem(void);

	void Run3ToteAuto(MecanumDrive *drive, LiftSystem *lifter);

private:
	typedef enum{Step1, Step2, Step3, Step4, Step5, Step6} AutoMode;

	DigitalInput *toteTouchSensor;
	DigitalInput *bottomLiftLS;

	AutoMode autoMode;

};



#endif /* SRC_SYSTEMS_AUTONOMOUS_H_ */
