#include "Main.h"

void goToBeacon(int color);
void turnOffBeacon(void);
void dropArm(int stepSize, int stepDelay);
void exitArena(void);

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
      dropArm(-10, 100);

      // Leave arena
      exitArena();
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

void exitArena(void)
{
      const int BASE_SPEED = 50;
      int forward_speed;
      int steer_speed; // Motor near wall goes faster, motor away from wall goes slower
      int bonus_speed;

      int ultrasonic_left_distance;
      int ultrasonic_right_distance;

      // Initialize ultrasonics
      StartUltrasonic(PIN_DIO_ULTRASONIC_RIGHT_OUTPUT, PIN_DIO_ULTRASONIC_RIGHT_INPUT);
      StartUltrasonic(PIN_DIO_ULTRASONIC_LEFT_OUTPUT, PIN_DIO_ULTRASONIC_LEFT_INPUT);

      while (1)
      {
            // Read ultrasonics
            ultrasonic_left_distance = GetUltrasonicCm(
                PIN_DIO_ULTRASONIC_LEFT_OUTPUT,
                PIN_DIO_ULTRASONIC_LEFT_INPUT);

            ultrasonic_right_distance = GetUltrasonicCm(
                PIN_DIO_ULTRASONIC_RIGHT_OUTPUT,
                PIN_DIO_ULTRASONIC_RIGHT_INPUT);

            // Reset values that will be calculated
            forward_speed = 0;
            steer_speed = 0;
            bonus_speed = 0;

            // If really close to a wall (head on or approaching a corner)
            if (ultrasonic_left_distance < 5 && ultrasonic_right_distance < 5)
            {
                  // Steer only, no forward speed
                  steer_speed = BASE_SPEED;
            }
            else
            {
                  // Go forward at least at base_speed
                  forward_speed = BASE_SPEED;

                  // TODO: Which one should be negative?
                  if (ultrasonic_left_distance < 15)
                  {
                        // Steer away if approaching wall on left
                        steer_speed = (15 - ultrasonic_left_distance) * 3;
                  }
                  else if (ultrasonic_right_distance < 15)
                  {
                        // Steer away if approaching wall on right
                        steer_speed = -(15 - ultrasonic_left_distance) * -3;
                  }
                  else
                  {
                        // No walls nearby - go extra fast
                        bonus_speed = 25;
                  }
            }

            // TODO: Which one should be negative?
            SetMotor(PIN_MOTOR_LEFT, 0 + steer_speed + forward_speed + bonus_speed);
            SetMotor(PIN_MOTOR_RIGHT, 0 + steer_speed - forward_speed - bonus_speed);
      }
}