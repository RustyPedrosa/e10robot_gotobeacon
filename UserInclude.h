// UserInclude.h : header file
#ifndef USERINCLUDE_H_
#define USERINCLUDE_H_

// TODO: add user code here
#define PIN_DIO_LEFTBUMPER 1
#define PIN_DIO_RIGHTBUMPER 2
#define PIN_DIO_LIMITSWITCH 5
#define PIN_MOTOR_LEFT 8
#define PIN_MOTOR_RIGHT 9
#define PIN_MOTOR_SERVO 2

void Read_PD(void);
int expose_and_read(void);
void find_max(void);
void move(void);
int limit_pwm(int temp);

#endif // USERINCLUDE_H_
