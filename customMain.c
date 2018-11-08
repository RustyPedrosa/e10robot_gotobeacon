#include "Main.h"

void goToBeacon(int color);
void turnOffBeacon(void);
void dropArm(int stepSize, int stepDelay);

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

      // Raise arm
      SetServo(PIN_MOTOR_SERVO, ARM_UP);

      // Go to red beacon and stop
      goToBeacon(0);

      // Turn off beacon (loop)
      turnOffBeacon();

      //Back up so we don't hit red beacon
      SetMotor(PIN_MOTOR_LEFT, forward_speed * -1);
      SetMotor(PIN_MOTOR_RIGHT, forward_speed);
      Wait(1000);

      // Go to green beacon and stop
      goToBeacon(1);

      // Capture green beacon
      {
            int arm_position;
            // Drop arm
            for (arm_position = ARM_UP; arm_position > ARM_DOWN; arm_position -= 10)
            {
                  SetServo(PIN_MOTOR_SERVO, limit_pwm(arm_position));
                  Wait(100);
            }
      }

      // Leave arena
      {
            int ultrasonic_left_distance;
            int ultrasonic_right_distance;

            // Turn on right ultrasonic
            StartUltrasonic(PIN_DIO_ULTRASONIC_RIGHT_OUTPUT, PIN_DIO_ULTRASONIC_RIGHT_INPUT);
            // Turn on left ultrasonic
            StartUltrasonic(PIN_DIO_ULTRASONIC_LEFT_OUTPUT, PIN_DIO_ULTRASONIC_LEFT_INPUT);

            int direction = 0; // 0 = unknown, -1 = go left, 1 = go right
            while (1)
            {
                  ultrasonic_left_distance = GetUltrasonicCm(
                      PIN_DIO_ULTRASONIC_LEFT_OUTPUT,
                      PIN_DIO_ULTRASONIC_LEFT_INPUT);

                  ultrasonic_right_distance = GetUltrasonicCm(
                      PIN_DIO_ULTRASONIC_RIGHT_OUTPUT,
                      PIN_DIO_ULTRASONIC_RIGHT_INPUT);

                  // PrintToScreen("Left dist: %d, Right dist: %d\n", ultrasonic_left_distance, ultrasonic_right_distance);

                  switch (direction)
                  {
                  case 0: // Haven't found a wall yet

                        // Go straight until we find a wall
                        left_speed = forward_speed * -1;
                        right_speed = forward_speed;
                        break;

                  default:
                        break;
                  }
                  while (ultrasonic_left_distance < 15 && ultrasonic_right_distance < 15)
                  {
                  }

                  // If approaching on ONE side, turn away from it
                  // If approaching on BOTH sides, turn sharply
                  SetMotor(PIN_MOTOR_LEFT, forward_speed * -1);
                  SetMotor(PIN_MOTOR_RIGHT, forward_speed);

                  Wait(2000);
            }
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

void dropArm(int stepSize, int stepDelay)
{
      int arm_position;
      for (arm_position = ARM_UP; arm_position > ARM_DOWN; arm_position += stepSize)
      {
            SetServo(PIN_MOTOR_SERVO, limit_pwm(arm_position));
            Wait(stepDelay);
      }
}

void turnOffBeacon(void)
{
      // Turn off red beacon
      while (1)
      {
            // Drop arm
            dropArm(-10, 100);

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
}