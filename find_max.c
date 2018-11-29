#include "Main.h"

void find_max ( void )
{
      max_val = PD0 ;
      max_no =0;
      if ( PD1 > max_val  )
      {
            max_val = PD1;
            max_no =1;
      }
      if ( PD2 > max_val )
      {
            max_val = PD2;
            max_no =2;
      }
      if ( PD3 > max_val )
      {
            max_val = PD3;
            max_no =3;
      }
      if ( PD4 > max_val )
      {
            max_val = PD4;
            max_no =4;
      }
      if ( PD5 > max_val )
      {
            max_val = PD5;
            max_no =5;
      }
      if ( PD6 > max_val )
      {
            max_val = PD6;
            max_no =6;
      }
      if ( PD7 > max_val )
      {
            max_val = PD7 ;
            max_no =7;
      }
}

