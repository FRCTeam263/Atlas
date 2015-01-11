/*
 * CanGrabber.cpp
 *
 *  Created on: Jan 10, 2015
 *      Author: Administrator
 */

#include <Systems/CanGrabber.h>

CanGrabber::CanGrabber() {
	grabber = new Victor(5);

}

CanGrabber::~CanGrabber() {
	delete grabber;
}

void CanGrabber::Extend(Joystick *gamePad){
	if(gamePad->GetRawButton(3)){
		grabber->Set(0.5);
	}
	else if(gamePad->GetRawButton(4)){
		grabber->Set(-0.5);
	}
	else{
		grabber->Set(0);
	}
}
