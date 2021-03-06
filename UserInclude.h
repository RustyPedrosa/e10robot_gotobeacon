// UserInclude.h : header file
#ifndef USERINCLUDE_H_
#define USERINCLUDE_H_

#include <stdbool.h>

/******************************************************************************
 * Constants
 *****************************************************************************/
// Pin numbers ----------------------------------------
// ADC pins
// DIO pins
// #define PIN_DIO_LEFTBUMPER 1
// #define PIN_DIO_RIGHTBUMPER 2
#define PIN_DIO_LIMITSWITCH 1

#define PIN_DIO_ULTRASONIC_RIGHT_OUTPUT 3 //

#define PIN_DIO_ULTRASONIC_RIGHT_INPUT 2 // Output to Control

#define PIN_DIO_ULTRASONIC_LEFT_OUTPUT 5 //

#define PIN_DIO_ULTRASONIC_LEFT_INPUT 4 //

// Motor pins
#define PIN_MOTOR_LEFT 8
#define PIN_MOTOR_RIGHT 9
#define PIN_MOTOR_SERVO 2

// Arm positions
#define ARM_UP 127
#define ARM_DOWN -24

#define WALL_FOLLOW_DISTANCE_CM 10

#define DISTANCE_CRASH_AVOID 40
#define DISTANCE_STEER_START 80

void customMain(void);
void Read_PD(void);
int expose_and_read(bool);
void find_max(void);
void move(void);
void stopDriving(void);
int limit_pwm(int temp);

#endif // USERINCLUDE_H_
