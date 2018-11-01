#include "Main.h"

int expose_and_read ( void )
{
      int intensity; 

      SetDigitalOutput ( 11 , 1 ) ; // close shuter, clear film, increment counter
      Wait ( 5 ) ; // Wait 5mS for things to settle
      SetDigitalOutput ( 11 , 0 ) ; // open shutter for exposure
      Wait ( expose_time  ) ; // expose time = 3mS to 8mS (can be adjusted)
      intensity = GetAnalogInput ( 1 ) ; // get the IR intensity reading
      return intensity ;
}

