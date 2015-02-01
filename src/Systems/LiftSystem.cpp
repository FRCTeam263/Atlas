#include "LiftSystem.h"

LiftSystem::LiftSystem(void){
/*<<<<<<< HEAD
	shortLiftMotor1 = new CANTalon(7);
	shortLiftMotor2 = new CANTalon(6);
	longLiftMotor1 = new CANTalon(4);
	longLiftMotor2 = new CANTalon(3);

	shortBottomLS = new DigitalInput(2);
	shortTopLS = new DigitalInput(3);
	longBottomLS = new DigitalInput(1);
	longTopLS = new DigitalInput(0);
	longToteTouchSensor = new DigitalInput(4);
	shortToteTouchSensor = new DigitalInput(5);
=======*/
	shortLiftMotor1 = new CANTalon(SHORT_LIFT_MOTOR_1);
	shortLiftMotor2 = new CANTalon(SHORT_LIFT_MOTOR_2);
	longLiftMotor1 = new CANTalon(LONG_LIFT_MOTOR_1);
	longLiftMotor2 = new CANTalon(LONG_LIFT_MOTOR_2);

	shortBottomLS = new DigitalInput(SHORT_BOTTOM_LS);
	shortTopLS = new DigitalInput(SHORT_TOP_LS);
	longBottomLS = new DigitalInput(LONG_BOTTOM_LS);
	longTopLS = new DigitalInput(LONG_TOP_LS);
	longToteTouchSensor = new DigitalInput(LONG_TOTE_SENSOR);
	shortToteTouchSensor = new DigitalInput(SHORT_TOTE_SENSOR);

	utilities = new Utilities();
	lifterOutput = new ElevatorSpeedAlgorithm();

	shortLiftMotor1->SetFeedbackDevice(CANTalon::QuadEncoder);
	shortLiftMotor1->SetSensorDirection(true);
	longLiftMotor1->SetFeedbackDevice(CANTalon::QuadEncoder);
	longLiftMotor1->SetSensorDirection(true);

	longLiftMotor2->SetControlMode(CANTalon::kFollower);
	longLiftMotor2->Set(4);
	shortLiftMotor2->SetControlMode(CANTalon::kFollower);
	shortLiftMotor2->Set(7);

	shortLiftMotor1->SetPosition(0);
	longLiftMotor1->SetPosition(0);

	shortEncoderDistance = shortLiftMotor1->GetPosition();
	longEncoderDistance = longLiftMotor1->GetPosition();
}

LiftSystem::~LiftSystem(void){
	delete shortLiftMotor1;
	delete shortLiftMotor2;
	delete longLiftMotor1;
	delete longLiftMotor2;

	delete shortBottomLS;
	delete shortTopLS;
	delete longBottomLS;
	delete longTopLS;
	delete shortToteTouchSensor;
	delete longToteTouchSensor;

	delete utilities;
	delete lifterOutput;
}

void LiftSystem::TestLifter(Joystick *gamePad){
	//printf("LB: %d\t LT: %d\t SB: %d\t ST: %d\n", longBottomLS->Get(),longTopLS->Get(), shortBottomLS->Get(), shortTopLS->Get());
	if(longBottomLS->Get() == true){
		longLiftMotor1->SetPosition(0);
	}
	if(shortBottomLS->Get() == true){
		shortLiftMotor1->SetPosition(0);
	}

	if(longBottomLS->Get() == true && gamePad->GetRawButton(11)){
		longLiftMotor1->Set(0);
		longLiftMotor1->SetPosition(0);
	}
	else if(longTopLS->Get() == true && gamePad->GetRawButton(12)){
		longLiftMotor1->Set(0);
	}
	else if((longTopLS->Get() == false || longTopLS->Get() == true) && gamePad->GetRawButton(11)){
		longLiftMotor1->Set(0.7);
	}
	else if((longBottomLS->Get() == false || longBottomLS->Get() == true) && gamePad->GetRawButton(12)){
		longLiftMotor1->Set(-1);
	}
	else{
		longLiftMotor1->Set(0);
	}
	//printf("MotorShort: %f\n", shortLiftMotor1->Get());
	if(shortBottomLS->Get() == true && gamePad->GetRawButton(13) == true){
		shortLiftMotor1->Set(0);
		shortLiftMotor1->SetPosition(0);
	}
	else if(shortTopLS->Get() == true && gamePad->GetRawButton(14) == true){
		shortLiftMotor1->Set(0);
	}
	else if((shortTopLS->Get() == false || shortTopLS->Get() == true) && gamePad->GetRawButton(13)){
		shortLiftMotor1->Set(0.4);
	}
	else if((shortBottomLS->Get() == false || shortBottomLS->Get() == true) && gamePad->GetRawButton(14)){
		shortLiftMotor1->Set(-0.8);
	}
	else{
		shortLiftMotor1->Set(0);
	}
}

void LiftSystem::RunLongLift(Joystick *gamePad){
	float MotorOutput = 0;
	static int LongLevel = 0;

	if(longBottomLS->Get() == true){
		shortLiftMotor1->SetPosition(0);
	}

	if(gamePad->GetRawButton(11) == true){
		LongLevel = 0;
	}
	else if(gamePad->GetRawButton(12) == true){
		LongLevel = 1;
	}
	else if(gamePad->GetRawButton(13) == true){
		LongLevel = 2;
	}
	else if(gamePad->GetRawButton(14) == true){
		LongLevel = 3;
	}
	else if(gamePad->GetRawButton(15) == true){
		LongLevel = 4;
	}

	MotorOutput = lifterOutput->ComputeNextMotorSpeedCommand(longLiftMotor1->GetPosition(), elevatorLongLevels[LongLevel]);

	longLiftMotor1->Set(MotorOutput * -1);
}

void LiftSystem::RunShortLift(Joystick *gamePad){
	float MotorOutput = 0;
	static int ShortLevel = 0;

	if(shortBottomLS->Get() == true){
		shortLiftMotor1->SetPosition(0);
	}

	if(gamePad->GetRawButton(6) == true){
		ShortLevel = 0;
	}
	else if(gamePad->GetRawButton(7) == true){
		ShortLevel = 1;
	}
	else if(gamePad->GetRawButton(8) == true){
		ShortLevel = 2;
	}
	else if(gamePad->GetRawButton(9) == true){
		ShortLevel = 3;
	}
	else if(gamePad->GetRawButton(10) == true){
		ShortLevel = 4;
	}

	MotorOutput = lifterOutput->ComputeNextMotorSpeedCommand(shortLiftMotor1->GetPosition(), elevatorShortLevels[ShortLevel]);

	shortLiftMotor1->Set(MotorOutput * -1);
}

void LiftSystem::ResetLifter(){
	if(shortBottomLS->Get() == false){
		shortLiftMotor1->Set(1);
	}
	else if(shortBottomLS->Get() == true){
		shortLiftMotor1->Set(0);
	}

	if(longBottomLS->Get() == false){
		longLiftMotor1->Set(1);
	}
	else if(longBottomLS->Get() == true){
		longLiftMotor1->Set(0);
	}
}

void LiftSystem::SetSpeed(float Speed, bool longSide, bool shortSide, bool BothSides){
	if(longSide == true){
		longLiftMotor1->Set(Speed);
		shortLiftMotor1->Set(0);
	}
	else if(shortSide == true){
		shortLiftMotor1->Set(Speed);
		longLiftMotor1->Set(0);
	}
	else if(BothSides == true){
		shortLiftMotor1->Set(Speed);
		longLiftMotor1->Set(Speed);
	}
	else{
		shortLiftMotor1->Set(0);
		longLiftMotor1->Set(0);
	}
}

void LiftSystem::SetZero(void){
	longLiftMotor1->Set(0);
	shortLiftMotor1->Set(0);
}
