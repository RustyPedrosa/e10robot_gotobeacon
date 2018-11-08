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
#define PIN_DIO_1_INPUT true

#define PIN_DIO_ULTRASONIC_RIGHT_OUTPUT 2 //
#define PIN_DIO_2_INPUT true

#define PIN_DIO_ULTRASONIC_RIGHT_INPUT 3 // Output to Control
#define PIN_DIO_3_INPUT false

#define PIN_DIO_ULTRASONIC_LEFT_OUTPUT 4 //
#define PIN_DIO_4_INPUT true

#define PIN_DIO_ULTRASONIC_LEFT_INPUT 6 //
#define PIN_DIO_6_INPUT false

// Motor pins
#define PIN_MOTOR_LEFT 8
#define PIN_MOTOR_RIGHT 9
#define PIN_MOTOR_SERVO 2

// Arm positions
#define ARM_UP 0
#define ARM_DOWN 127

void customMain(void);
void Read_PD(void);
int expose_and_read(bool);
void find_max(void);
void move(void);
int limit_pwm(int temp);

#endif // USERINCLUDE_H_
