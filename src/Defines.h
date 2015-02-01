#ifndef SRC_SYSTEMS_DEFINES_H_
#define SRC_SYSTEMS_DEFINES_H_

//CAN Motors
#define FRONT_LEFT_MOTOR 1
#define FRONT_RIGHT_MOTOR 0
#define BACK_LEFT_MOTOR 2
#define BACK_RIGHT_MOTOR 5

#define LONG_LIFT_MOTOR_1 4
#define LONG_LIFT_MOTOR_2 3
#define SHORT_LIFT_MOTOR_1 7
#define SHORT_LIFT_MOTOR_2 6

//DIO
#define SHORT_BOTTOM_LS 2
#define SHORT_TOP_LS 3
#define LONG_BOTTOM_LS 1
#define LONG_TOP_LS 0
#define LONG_TOTE_SENSOR 4
#define SHORT_TOTE_SENSOR 5

//Analog Input
#define GYRO 0

//Const
const int elevatorShortLevels[5] = {0, 750, 1500, 2300, 2900};


#endif /* SRC_SYSTEMS_DEFINES_H_ */
