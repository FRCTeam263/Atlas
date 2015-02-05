#ifndef SRC_SYSTEMS_DEFINES_H_
#define SRC_SYSTEMS_DEFINES_H_

//CAN Motors
#define FRONT_LEFT_MOTOR 1
#define FRONT_RIGHT_MOTOR 0
#define BACK_LEFT_MOTOR 2
#define BACK_RIGHT_MOTOR 5

#define CAN_LIFT_MOTOR 4
#define SHORT_LIFT_MOTOR_1 7
#define SHORT_LIFT_MOTOR_2 6

//DIO
#define SHORT_BOTTOM_LS 3
#define SHORT_TOP_LS 2
#define CAN_BOTTOM_LS 1
#define CAN_TOP_LS 0
#define LONG_TOTE_SENSOR 4
#define SHORT_TOTE_SENSOR 5

//Analog Input
#define GYRO 0

//Const
const int elevatorShortLevels[5] = {0, 416, 1360, 2700, 3750};
const int elevatorCanLevels[4] = {0, 500, 1000, 3750};


#endif /* SRC_SYSTEMS_DEFINES_H_ */
