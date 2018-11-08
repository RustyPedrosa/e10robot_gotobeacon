#include "Main.h"

/******************************************************************************
 * Missions:
 * 1. Drive to red beacon
 * 2. Press button (arm down, arm up, repeat until confirmed off)
 * 3. Drive to green beacon
 * 4. Capture beacon (arm down)
 * 5. Exit arena (find a wall, follow it)
 *****************************************************************************/
void customMain(void)
{
      ambient_level = 200;    // used in 'move'
      slow_level = 5000;      // used in 'move'
      stop_level = 6000;      // used in 'move'
      expose_time = 3;        // used in expose_and_read
      steer_sensitivity = 20; // used in 'move'
      forward_speed = 35;     // forward speed, used in 'move'
      slow_speed = 25;        // slow speed, used in 'move'
      spin_speed = 50;        // spin speed (for searching mode), used in 'move'

      freq = 0;                   // 0=1khz (red), 1=10kHz(green beacon)
      SetDigitalOutput(10, freq); // turn to 1kHz (red beacon)

      // Raise arm
      SetServo(PIN_MOTOR_SERVO, 127);

      while (GetDigitalInput(PIN_DIO_LIMITSWITCH)) // while limit switch unpressed
      {
            Read_PD();
            find_max();
            move();
      }
      SetServo(PIN_MOTOR_SERVO, 120);
}
