#include "LiftSystem.h"

LiftSystem::LiftSystem(void){
	shortLiftMotor1 = new CANTalon(SHORT_LIFT_MOTOR_1);
	shortLiftMotor2 = new CANTalon(SHORT_LIFT_MOTOR_2);
	canLiftMotor = new CANTalon(CAN_LIFT_MOTOR);

	shortBottomLS = new DigitalInput(SHORT_BOTTOM_LS);
	shortTopLS = new DigitalInput(SHORT_TOP_LS);
	canBottomLS = new DigitalInput(CAN_BOTTOM_LS);
	canTopLS = new DigitalInput(CAN_TOP_LS);
	longToteTouchSensor = new DigitalInput(LONG_TOTE_SENSOR);
	shortToteTouchSensor = new DigitalInput(SHORT_TOTE_SENSOR);

	utilities = new Utilities();
	toteLifterOutput = new ElevatorSpeedAlgorithm();
	canLifterOutput = new ElevatorSpeedAlgorithm();

	shortLiftMotor1->SetFeedbackDevice(CANTalon::QuadEncoder);
	shortLiftMotor1->SetSensorDirection(true);
	canLiftMotor->SetFeedbackDevice(CANTalon::QuadEncoder);
	canLiftMotor->SetSensorDirection(true);

	shortLiftMotor2->SetControlMode(CANTalon::kFollower);
	shortLiftMotor2->Set(7);

	shortLiftMotor1->SetPosition(0);
	canLiftMotor->SetPosition(0);

	toteEncoderDistance = shortLiftMotor1->GetPosition();
	canEncoderDistance = canLiftMotor->GetPosition();
}

LiftSystem::~LiftSystem(void){
	delete shortLiftMotor1;
	delete shortLiftMotor2;
	delete canLiftMotor;

	delete shortBottomLS;
	delete shortTopLS;
	delete canBottomLS;
	delete canTopLS;
	delete shortToteTouchSensor;
	delete longToteTouchSensor;

	delete utilities;
	delete toteLifterOutput;
	delete canLifterOutput;
}

void LiftSystem::RunLifter(Joystick *gamePad){
	static bool manualEnabled = false;
	if(utilities->GetJoystickButton(16, gamePad)){
		manualEnabled = !manualEnabled;
	}

	//printf("Enabled: %d\n", manualEnabled);

	if(manualEnabled == true){
		if(shortBottomLS->Get() == true){
			shortLiftMotor1->SetPosition(0);
		}
		else if(shortTopLS->Get() == true){
			shortLiftMotor1->SetPosition(3830);
		}

		if(canBottomLS->Get() == true && gamePad->GetRawButton(6)){
			canLiftMotor->Set(0);
			canLiftMotor->SetPosition(0);
		}
		else if(canTopLS->Get() == true && gamePad->GetRawButton(7)){
			canLiftMotor->Set(0);
		}
		else if((canTopLS->Get() == false || canTopLS->Get() == true) && gamePad->GetRawButton(6)){
			canLiftMotor->Set(0.7);
		}
		else if((canBottomLS->Get() == false || canBottomLS->Get() == true) && gamePad->GetRawButton(7)){
			canLiftMotor->Set(-0.7);
		}
		else{
			canLiftMotor->Set(0);
		}

		if(shortBottomLS->Get() == true && gamePad->GetRawButton(8) == true){
			shortLiftMotor1->Set(0);
			shortLiftMotor1->SetPosition(0);
		}
		else if(shortTopLS->Get() == true && gamePad->GetRawButton(9) == true){
			shortLiftMotor1->Set(0);
		}
		else if((shortTopLS->Get() == false || shortTopLS->Get() == true) && gamePad->GetRawButton(8)){
			shortLiftMotor1->Set(0.3);
		}
		else if((shortBottomLS->Get() == false || shortBottomLS->Get() == true) && gamePad->GetRawButton(9)){
			shortLiftMotor1->Set(-0.8);
		}
		else{
			shortLiftMotor1->Set(0);
		}
	}
	else if(manualEnabled == false){
		float canMotorOutput = 0;
		float shortMotorOutput = 0;
		static int ShortLevel = 0;
		static int CanLevel = 0;

		if(shortBottomLS->Get() == true){
			shortLiftMotor1->SetPosition(0);
		}
		if(canBottomLS->Get() == true){
			canLiftMotor->SetPosition(0);
		}
		/*else if(shortTopLS->Get() == true){
			shortLiftMotor1->SetPosition(3830);
		}*/

		if(gamePad->GetRawButton(6) == true){
			ShortLevel = 0;
		}
		else if(gamePad->GetRawButton(7) == true){
			ShortLevel = 1;
		}
		else if(gamePad->GetRawButton(8) == true){//1 tote high
			ShortLevel = 2;
		}
		else if(gamePad->GetRawButton(9) == true){
			ShortLevel = 3;
		}
		else if(gamePad->GetRawButton(10) == true){
			ShortLevel = 4;
		}
		else if(gamePad->GetRawButton(1) == true){
			CanLevel = 0;
		}
		else if(gamePad->GetRawButton(2) == true){
			CanLevel = 1;
		}
		else if(gamePad->GetRawButton(3) == true){
			CanLevel = 2;
		}
		else if(gamePad->GetRawButton(4) == true){
			CanLevel = 3;
		}

		int encoderSetpoint = 0;

		if(utilities->GetJoystickButton(11, gamePad)){
			encoderSetpoint += 350;
		}
		else if(utilities->GetJoystickButton(12, gamePad)){
			encoderSetpoint -= 350;
		}

		if(encoderSetpoint < 0){
			encoderSetpoint = 0;
		}
		else if(encoderSetpoint > 3800){
			encoderSetpoint = 3800;
		}

		shortMotorOutput = toteLifterOutput->ComputeNextMotorSpeedCommand(toteEncoderDistance, encoderSetpoint);
				//toteLifterOutput->ComputeNextMotorSpeedCommand(shortLiftMotor1->GetPosition(), elevatorShortLevels[ShortLevel]);
		canMotorOutput = canLifterOutput->ComputeNextMotorSpeedCommand(canEncoderDistance, elevatorCanLevels[CanLevel]);

		if(shortTopLS->Get() == true && shortMotorOutput > 0){
			shortMotorOutput = 0;
		}
		else if(shortBottomLS->Get() == true && shortMotorOutput < 0){
			shortMotorOutput = 0;
		}
		else if(canBottomLS->Get() == true && canMotorOutput < 0){
			canMotorOutput = 0;
		}
		else if(canTopLS->Get() == true && canMotorOutput > 0){
			canMotorOutput = 0;
		}

		SetToteSpeed(shortMotorOutput);
		SetCanSpeed(canMotorOutput);
	}
	else{
		SetZero();
	}
}

void LiftSystem::ResetLifter(){
	if(shortBottomLS->Get() == false){
		shortLiftMotor1->Set(1);
	}
	else if(shortBottomLS->Get() == true){
		shortLiftMotor1->Set(0);
	}

	if(canBottomLS->Get() == false){
		canLiftMotor->Set(1);
	}
	else if(canBottomLS->Get() == true){
		canLiftMotor->Set(0);
	}
}

void LiftSystem::SetCanSpeed(float Speed){
	canLiftMotor->Set(Speed * -1);
}

void LiftSystem::SetToteSpeed(float Speed){
	shortLiftMotor1->Set(Speed * -1);
}

void LiftSystem::SetZero(void){
	canLiftMotor->Set(0);
	shortLiftMotor1->Set(0);
}

