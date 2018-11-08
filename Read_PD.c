#include "Main.h"
#include <stdbool.h>

void Read_PD(void)
{
      PD0 = expose_and_read(true);
      PD1 = expose_and_read(false);
      PD2 = expose_and_read(false);
      PD3 = expose_and_read(false);
      PD4 = expose_and_read(false);
      PD5 = expose_and_read(false);
      PD6 = expose_and_read(false);
      PD7 = expose_and_read(false);
      PD_sum = PD0 + PD1 + PD2 + PD3 + PD4 + PD5 + PD6 + PD7;
}

int expose_and_read(bool reset_counter)
{
      int intensity;

      SetDigitalOutput(11, 1); // close shuter, clear film, increment counter
      if (reset_counter)
      {
            SetDigitalOutput(12, 1); // initialize conter value to '0'
            SetDigitalOutput(12, 0); // aloww counter to count
      }
      Wait(5);                       // Wait 5mS for things to settle
      SetDigitalOutput(11, 0);       // open shutter for exposure
      Wait(expose_time);             // expose time = 3mS to 8mS (can be adjusted)
      intensity = GetAnalogInput(1); // get the IR intensity reading
      return intensity;
}
