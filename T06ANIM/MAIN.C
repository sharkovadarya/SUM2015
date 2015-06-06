/* FILE: MAIN.C
 * PROGRAMMER: DS6
 * DATE: 06.06.2015
 * PURPOSE: test the functions from the newly added library.
*/

#include <stdio.h>
#include "vec.h"

void main( void )
{
  DBL a;
  VEC V1, V2;

  
  V1.X = 2.0, V1.Y = 2.8, V1.Z = 22.8, V2.X = 2.28, V2.Y = 0.228, V2.Z = 0.0228;
  a = VecLen2(V1); 
  printf("%lf", a);
 
}


/* End of 'MAIN.C' file. */