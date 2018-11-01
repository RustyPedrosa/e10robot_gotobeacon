#include "Main.h"

void move(void)
{
      int temp;
      int steer;
      int error;
      int speed;

      error = 4 - max_no;                // heading direction error, if PD4 ==max_no, no error
      steer = error * steer_sensitivity; // steering effort is proportional to heading error
      speed = forward_speed;             // forward speed
      if (PD_sum < ambient_level)        // If <background noise level => search mode.
      {
            speed = 0;           // search mode => no forward motion
            steer = -spin_speed; // search mode => spin
      }
      if (PD_sum > slow_level) // Beacon is near!
      {
            speed = slow_speed; // Slow down
      }
      if (PD_sum > stop_level) // Found the beacon!
      {
            speed = 0; // Stop
            steer = 0; // no steering
      }
      temp = limit_pwm(0 + steer + speed);
      SetMotor(PIN_MOTOR_LEFT, temp);
      temp = limit_pwm(0 + steer - speed);
      SetMotor(PIN_MOTOR_RIGHT, temp);
}
