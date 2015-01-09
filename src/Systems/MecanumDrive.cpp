#include "MecanumDrive.h"

MecanumDrive::MecanumDrive(){
	FRMotor = new Talon(2);
	FLMotor = new Talon(1);
	BRMotor = new Talon(3);
	BLMotor = new Talon(0);

	FLWheel = new Encoder(0, 1, false, Encoder::k2X);
	/*BLWheel = new Encoder(2, 3, false, Encoder::k1X);
	FRWheel = new Encoder(4, 5, false, Encoder::k1X);
	BRWheel = new Encoder(6, 7, false, Encoder::k1X);*/

	utilities = new Utilities();

	autoMode = Step1;

	FRMotor->Set(0);
	BRMotor->Set(0);
	FLMotor->Set(0);
	BLMotor->Set(0);

	FLSpeed = 0;
	FRSpeed = 0;
	BLSpeed = 0;
	BRSpeed = 0;
}

MecanumDrive::~MecanumDrive(){
	delete FRMotor;
	delete FLMotor;
	delete BLMotor;
	delete BRMotor;

	delete FLWheel;

	delete utilities;

}

void MecanumDrive::Drive(Joystick *drivePad){
	float YDrive = drivePad->GetY() * -1;
	float XDrive = drivePad->GetX() * -1;
	float Rotate = -drivePad->GetThrottle() + drivePad->GetTwist();

	printf("Throttle: %f\t Twist: %f\t Trigger: %d\n", drivePad->GetThrottle(), drivePad->GetTwist(), drivePad->GetTrigger());

	YDrive = utilities->DeadBand(YDrive, 0.1);
	XDrive = utilities->DeadBand(XDrive, 0.1);
	Rotate = utilities->DeadBand(Rotate, 0.1);

	FLSpeed = XDrive + YDrive + Rotate;
	FRSpeed = -XDrive + YDrive - Rotate;
	BLSpeed = -XDrive + YDrive + Rotate;
	BRSpeed = XDrive + YDrive - Rotate;

	float max = 0;

	if(fabs(FLSpeed) > max){
		max = fabs(FLSpeed);
	}
	if(fabs(FRSpeed) > max){
		max = fabs(FRSpeed);
	}
	if(fabs(BLSpeed) > max){
		max = fabs(BLSpeed);
	}
	if(fabs(BRSpeed) > max){
		max = fabs(BRSpeed);
	}
	if(max > 1){
		FLSpeed = FLSpeed / max;
		FRSpeed = FRSpeed / max;
		BLSpeed = BLSpeed / max;
		BRSpeed = BRSpeed / max;
	}

	FLMotor->Set(FLSpeed * -1);
	FRMotor->Set(FRSpeed);
	BLMotor->Set(BLSpeed * -1);
	BRMotor->Set(BRSpeed);
}

void MecanumDrive::AutonDrive(Joystick *box){

	if(box->GetRawButton(1)){//3 Totes Auto

		if((FLWheel->GetDistance() < 100) && (autoMode == Step1)){
			FLMotor->Set(0.15);
			BLMotor->Set(-0.15);
			FRMotor->Set(-0.15);
			BRMotor->Set(0.15);
		}

		if(FLWheel->GetDistance() > 100){
			autoMode = Step2;
			FLWheel->Reset();
			/*FRWheel->Reset();
			BLWheel->Reset();
			BRWheel->Reset();*/
		}

		if((FLWheel->GetDistance() < 100) && (autoMode == Step2)){

		}
	}
	else if(box->GetRawButton(2)){//2 Can/2 Tote Auto

	}
	else if(box->GetRawButton(3)){

	}
	else{
		FLMotor->Set(0);
		FRMotor->Set(0);
		BLMotor->Set(0);
		BRMotor->Set(0);
	}
}
