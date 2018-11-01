#include "Main.h"

void Read_PD ( void )
{
      SetDigitalOutput ( 11 , 1 ) ; // close shutter, clear film, and increment counter
      SetDigitalOutput ( 12 , 1 ) ; // initialize conter value to '0'
      SetDigitalOutput ( 12 , 0 ) ; // aloww counter to count
      Wait ( 5 ) ; // 5mS wait for things settling down
      SetDigitalOutput ( 11 , 0 ) ; // open shutter and expose film
      Wait ( expose_time  ) ; // exposer time = 3mS to 8mS
      PD0 = GetAnalogInput ( 1 ) ; // read intensity
      PD1 = expose_and_read ( ) ;
      PD2 = expose_and_read ( ) ;
      PD3 = expose_and_read ( ) ;
      PD4 = expose_and_read ( ) ;
      PD5 = expose_and_read ( ) ;
      PD6 = expose_and_read ( ) ;
      PD7 = expose_and_read ( ) ;
      PD_sum=PD0+PD1+PD2+PD3+PD4+PD5+PD6+PD7;
}

