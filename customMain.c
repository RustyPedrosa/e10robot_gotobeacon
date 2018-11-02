#include "Main.h"

void customMain(void)
{
      freq = 0;                   // 0=1khz (red), 1=10kHz(green beacon)
      ambient_level = 200;        // esed in 'move'
      slow_level = 5000;          // used in 'move'
      stop_level = 6000;          // used in 'move'
      expose_time = 3;            // used in expose_and_read
      steer_sensitivity = 20;     // used in 'move'
      forward_speed = 35;         // forward speed, used in 'move'
      slow_speed = 25;            // slow speed, used in 'move'
      spin_speed = 50;            // spin speed (for searching mode), used in 'move'
      SetDigitalOutput(10, freq); // turn to 1kHz (red beacon)

      SetServo(PIN_MOTOR_SERVO, -127);
      while (GetDigitalInput(5)) // while limit switch unpressed
      {
            Read_PD();
            find_max();
            move();
      }
      SetServo(PIN_MOTOR_SERVO, 120);
}
