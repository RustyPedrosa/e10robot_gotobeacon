#include "Main.h"

void goToBeacon(int color);

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

      int arm_position;

      // Raise arm
      SetServo(PIN_MOTOR_SERVO, ARM_UP);

      // Go to red beacon and stop
      goToBeacon(0);

      // Turn off red beacon
      while (1)
      {
            // Drop arm
            // SetServo(PIN_MOTOR_SERVO, ARM_DOWN);
            for (arm_position = ARM_UP; arm_position > ARM_DOWN; arm_position -= 10)
            {
                  SetServo(PIN_MOTOR_SERVO, limit_pwm(arm_position));
                  Wait(100);
            }

            // Raise arm
            SetServo(PIN_MOTOR_SERVO, ARM_UP);
            Wait(500);

            // exit loop if beacon is off
            Read_PD();
            if (PD_sum <= ambient_level)
            {
                  break;
            }
      }

      //Back up so we don't hit red beacon
      SetMotor(PIN_MOTOR_LEFT, forward_speed * -1);
      SetMotor(PIN_MOTOR_RIGHT, forward_speed);
      Wait(1000);

      // Go to green beacon and stop
      goToBeacon(1);

      // Capture green beacon
      {
            // Drop arm
            for (arm_position = ARM_UP; arm_position > ARM_DOWN; arm_position -= 10)
            {
                  SetServo(PIN_MOTOR_SERVO, limit_pwm(arm_position));
                  Wait(100);
            }
      }

      //Back up so we don't hit red beacon
      Wait(1000);

      // Leave arena
      while (1)
      {
            // Reverse
            SetMotor(PIN_MOTOR_LEFT, forward_speed * -1);
            SetMotor(PIN_MOTOR_RIGHT, forward_speed);

            // If within 5cm of a wall, turn away from it slightly
            }
}

/**
 * 0=1khz (red), 1=10kHz(green beacon)
 */
void goToBeacon(int color)
{
      SetDigitalOutput(10, color);
      while (GetDigitalInput(PIN_DIO_LIMITSWITCH)) // while limit switch unpressed
      {
            Read_PD();
            find_max();
            move();
      }

      // Stop driving
      stopDriving();
}