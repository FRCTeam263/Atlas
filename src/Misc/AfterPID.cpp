/*
 * AfterPID.cpp
 *
 *  Created on: Jan 10, 2015
 *      Author: Administrator
 */

#include "AfterPID.h"

AfterPID::AfterPID(void) {
	kP = 0;
	kI = 0;
	kD = 0;

	I_err = 0;
	Prev_P_err = 0;
	P_err = 0;
	D_err = 0;

	Output = 0;
	PrevOutput = 0;

	s_Multiplier = 0;
}

void AfterPID::SetPID(float P, float I, float D, float Multiplier) {
	kP = P;
	kI = I;
	kD = D;

	s_Multiplier = Multiplier;
}

float AfterPID::GetOutput(float current, float target, float deadband, bool zeroIInDeadband) {
	P_err = target - current;
	I_err += P_err;
	D_err = P_err - Prev_P_err;

    // Check whether we've crossed over the target position.
	// (previous position error is opposite sign of current position error)
	if ((Prev_P_err < 0 && P_err >= 0) || (Prev_P_err >= 0 && P_err < 0)) {
		// CLear the Integration error if we're within the dead band.
		if(zeroIInDeadband)
			I_err = 0;
	}

	// Step foward our state so n becomes n+1
	Prev_P_err = P_err;

	float out;

	// Determine whether current position error is within the dead band
	if (P_err >= (deadband * -1) && P_err < deadband && zeroIInDeadband) {
		// Set motor velocity to zero, and clear the integration error
		out = 0;
		I_err = 0;
	} else {
		//
		out = ((kP * P_err) + (kI * I_err) - (kD * D_err));
	}

	if (out < -1)
		out = -1;
	else if (out > 1)
		out = 1;
	else if(out == 0 && zeroIInDeadband)
		I_err = 0;

	out *= s_Multiplier;

	return out;
}

void AfterPID::CalcEvolPID(float InputMin, float InputMax, float OutputMin, float OutputMax, bool SetI){

	kP = (OutputMax - OutputMin) / (2 * (InputMax - InputMin));
	if(SetI == false){
		kI = 0;
	}
	else if(SetI == true){
		kI = kP;
	}
	kD = 0;

}

void AfterPID::ResetPID()
{
	I_err = 0;
	Prev_P_err = 0;
	P_err = 0;
	D_err = 0;
}
