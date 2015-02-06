#include "Utilities.h"

Utilities::Utilities(void){

}

Utilities::~Utilities(void){

}

float Utilities::DeadBand(float input, float deadband){

	if(fabs(input) <= deadband){
		return 0.0f;
	}

	return input;
}

float Utilities::MaxValue(float input, float min, float max){

	if(input > max){
		input = max;
	}
	else if(input < min){
		input = min;
	}
	return input;
}

bool Utilities::GetJoystickButton(int RawButton, Joystick *HID){
	static int lastButtonValue = 0;
	int currentButtonValue = 0;
	bool currentPressedState = false;

	currentButtonValue = HID->GetRawButton(RawButton);
	if ((lastButtonValue == 0) && (currentButtonValue == 1)) {
		currentPressedState = true;
	}
	else{
		currentPressedState = false;
	}
	lastButtonValue = currentButtonValue;

	return currentPressedState;
}

