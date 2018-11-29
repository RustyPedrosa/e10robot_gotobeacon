#include "Main.h"

int limit_pwm ( int temp )
{
      int limited; 

      if ( temp > 127 )
      {
            limited =127;
      }
      else if ( temp < -127 )
      {
            limited = -127;
      }
      else
      {
            limited =temp;
      }
      return limited  ;
}

