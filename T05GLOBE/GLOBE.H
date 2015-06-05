/* FILE: GLOBE.H
 * PROGRAMMER: DS6
 * DATE: 05.06.2015
 * PURPOSE: declare functions, constants, etc. needed for this project.
*/

#ifndef __GLOBE_H_
#define __GLOBE_H_

#include <windows.h>

/* Тип хранения координат точки в пространстве */
typedef struct
{
  DOUBLE X, Y, Z;
} VEC; 

#define N 30
#define M 30
#define PI 3.14159265358979323846

VOID DrawQuad( HDC hDC, VEC P0, VEC P1, VEC P2, VEC P3, INT W, INT H );

VEC RotateX( VEC P, DOUBLE AngleDegree );

VEC VecCrossVec( VEC A, VEC B );

VEC VecSubVec( VEC A, VEC B );     

VOID DrawGlobe( HDC hDC, INT W, INT H );

VOID BuildGlobe( void );

    

#endif /*__GLOBE_H_*/

/*END OF 'GLOBE.H' FILE*/