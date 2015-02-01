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
	// This routine creates blackout periods for each joystick button
		// to ensure quick repeated calls do not result in taking the same
		// action multiple times.  The users press and release of a joystick
		// button probably lasts much longer than the OperatorControl loop iteration.
			static bool firstTimeCalled = true;
			const clock_t DelayBeforeRecognizeNextPress = 0.5 * CLOCKS_PER_SEC;
			const int maxButtons = 20;
			static clock_t ButtonTimeLastPressed[maxButtons];
			bool currentPressedState = false;

			if (firstTimeCalled)
			{
				// Make sure all buttons blackout periods are expired.
				firstTimeCalled = false;
				clock_t firstTimestamp = clock() - DelayBeforeRecognizeNextPress;
				for (int i = 1; i < maxButtons; i++)
				{
					ButtonTimeLastPressed[i] = firstTimestamp;
				}
				currentPressedState = HID->GetRawButton(RawButton);
				ButtonTimeLastPressed[RawButton] = firstTimestamp + DelayBeforeRecognizeNextPress;
				printf("Button[%d] Pressed. Value = %d\n",RawButton,currentPressedState);
			}
			else
			{
				// Only allow caller to see the actual button state when its respective
				// blackout period has expired.  Allow user to see this only once, and reset the
				// blackout timer.
				if ((clock() - ButtonTimeLastPressed[RawButton]) > DelayBeforeRecognizeNextPress)
				{
					currentPressedState = HID->GetRawButton(RawButton);
					ButtonTimeLastPressed[RawButton] = clock();
					printf("Button[%d] Pressed. Value = %d\n",RawButton,currentPressedState);
				}
				else
				{
					currentPressedState = false;
				}
			}
			return currentPressedState;
}

