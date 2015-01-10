/*
 * AfterPID.h
 *
 *  Created on: Jan 10, 2015
 *      Author: Administrator
 */

#ifndef SRC_MISC_AFTERPID_H_
#define SRC_MISC_AFTERPID_H_

class AfterPID {
public:
	AfterPID(float P, float I, float D, float Multiplier = 1);

	float GetOutput(float current, float targetAngle, float deadband, bool zeroIInDeadband = true);
	void SetPID(float P, float I, float D, float Multiplier);

	void ResetPID();

private:
	float kP, kI, kD;
	float I_err, Prev_P_err;
	float P_err;
	float D_err;

	float s_Multiplier;
};

#endif /* SRC_MISC_AFTERPID_H_ */
