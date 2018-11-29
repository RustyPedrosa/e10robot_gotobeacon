#include "Main.h"
#include "UserInclude.h"

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
      // ambient_level = 200;    // used in 'move'
      ambient_level = 150;    // used in 'move'
      slow_level = 5000;      // used in 'move'
      stop_level = 6000;      // used in 'move'
      expose_time = 3;        // used in expose_and_read
      steer_sensitivity = 20; // used in 'move'
      forward_speed = 35;     // forward speed, used in 'move'
      slow_speed = 25;        // slow speed, used in 'move'
      spin_speed = 50;        // spin speed (for searching mode), used in 'move'

      PrintToScreen("Initialization: Lifting arm.\n");
      // Raise arm
      SetServo(PIN_MOTOR_SERVO, ARM_UP);

      PrintToScreen("Going to red beacon then stopping.\n");
      // Go to red beacon and stop
      goToBeacon(0);

      PrintToScreen("Turn off red beacon.\n");
      // Turn off beacon (loop)
      turnOffBeacon();

      //Back up so we don't hit red beacon
      PrintToScreen("Reversing away from red beacon.\n");
      SetMotor(PIN_MOTOR_LEFT, forward_speed * -1);
      SetMotor(PIN_MOTOR_RIGHT, forward_speed);
      Wait(2000);

      PrintToScreen("Going to green beacon.\n");
      // Go to green beacon and stop
      goToBeacon(1);

      PrintToScreen("Dropping arm.\n");
      // Capture green beacon
      dropArm(-10, 100);

      PrintToScreen("Exiting arena.\n");
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

float getPercent(float value, float start, float end, bool zeroToOneOnly)
{
      float retVal = (value - start) / (end - start);

      if (zeroToOneOnly == true)
      {
            if (retVal < 0)
            {
                  retVal = 0;
            }
            if (retVal > 1)
            {
                  retVal = 1;
            }
      }

      return retVal;
}

void exitArena(void)
{
      const int BASE_SPEED = 50;
      int forward_speed;
      int steer_speed_left;  // Motor near wall goes faster, motor away from wall goes slower
      int steer_speed_right; // Motor near wall goes faster, motor away from wall goes slower
      int bonus_speed;

      int ultrasonic_left_distance;
      int ultrasonic_right_distance;

      PrintToScreen("Initializing ultrasonics.\n");
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

            PrintToScreen("Ultrasonic distances: Left = %i, Right = %i.\n", ultrasonic_left_distance, ultrasonic_right_distance);
            // Reset values that will be calculated
            forward_speed = BASE_SPEED;
            steer_speed_left = 0;
            steer_speed_right = 0;
            bonus_speed = 0;

            // If really close to a wall (head on or approaching a corner)
            if (ultrasonic_left_distance < DISTANCE_CRASH_AVOID && ultrasonic_right_distance < DISTANCE_CRASH_AVOID)
            {
                  PrintToScreen("Both ultrasonics < 5.  Will stop and steer only.\n");

                  // Steer only, no forward speed
                  steer_speed_left = BASE_SPEED;
                  steer_speed_right = BASE_SPEED;
                  forward_speed = 0;
            }
            else
            {
                  PrintToScreen("Not dangerously close to a wall...\n");
                  // Go forward at least at base_speed
                  forward_speed = BASE_SPEED;

                  // TODO: Which one should be negative?
                  if (ultrasonic_left_distance < DISTANCE_STEER_START)
                  {
                        // Steer away if approaching wall on left
                        PrintToScreen("Left ultrasonic < 15...\n");
                        steer_speed_left = (1.0 - getPercent(ultrasonic_left_distance, DISTANCE_CRASH_AVOID, DISTANCE_STEER_START, true)) * BASE_SPEED;
                        // forward_speed = 0;
                  }
                  else if (ultrasonic_right_distance < DISTANCE_STEER_START)
                  {
                        // Steer away if approaching wall on right
                        PrintToScreen("Right ultrasonic < 15...\n");
                        steer_speed_right = (1.0 - getPercent(ultrasonic_right_distance, DISTANCE_CRASH_AVOID, DISTANCE_STEER_START, true)) * BASE_SPEED;
                        // forward_speed = 0;
                  }
                  else
                  {
                        // No walls nearby - go extra fast
                        bonus_speed = 25;
                  }
            }

            // TODO: Which one should be negative?
            PrintToScreen("Setting outputs");

            SetMotor(PIN_MOTOR_LEFT, limit_pwm(0 + steer_speed_right - forward_speed - bonus_speed));
            SetMotor(PIN_MOTOR_RIGHT, limit_pwm(0 - steer_speed_left + forward_speed + bonus_speed));

            // Wait(50);
      }
}