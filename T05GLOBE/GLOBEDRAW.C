/* FILE: GLOBEDRAW.C
 * PROGRAMMER: DS6
 * DATE: 05.06.2015
 * PURPOSE: Globe drawing declaration module.
*/
#include <math.h>
#include <windows.h>
#include <time.h>
#include "globe.h"
/* Функция поворота точки вокруг оси X */
/*VEC RotateX( VEC P, DOUBLE AngleDegree )
{
  DOUBLE a = AngleDegree * PI / 180, si = sin(a), co = cos(a);
  VEC r;

  r.X = P.X;
  r.Y = P.Y * co - P.Z * si;
  r.Z = P.Y * si + P.Z * co;
  return r;
} *//* End of 'RotateX' function */

/* Функция векторного произведения */
/*VEC VecCrossVec( VEC A, VEC B )
{
  VEC r;

  r.X = A.Y * B.Z - A.Z * B.Y;
  r.Y = A.Z * B.X - A.X * B.Z;
  r.Z = A.X * B.Y - A.Y * B.X;
  return r;
}*/ /* End of 'VecCrossVec' function */

/* Функция вычитания векторов */
/*VEC VecSubVec( VEC A, VEC B )
{
  VEC r;

  r.X = A.X - B.X;
  r.Y = A.Y - B.Y;
  r.Z = A.Z - B.Z;
  return r;
}*/ /* End of 'VecSubVec' function */

/* Рисование четырехугольника */
/*VOID DrawQuad( HDC hDC, VEC P0, VEC P1, VEC P2, VEC P3, INT W, INT H )
{
  VEC Norm = VecCrossVec(VecSubVec(P3, P0), VecSubVec(P1, P0));
  POINT pnts[4];

  /* back-face culling */
 /* if (Norm.Z < 0)
    return;

  pnts[0].x = P0.X + W / 2;
  pnts[0].y = -P0.Y + H / 2;

  pnts[1].x = P1.X + W / 2;
  pnts[1].y = -P1.Y + H / 2;

  pnts[2].x = P2.X + W / 2;
  pnts[2].y = -P2.Y + H / 2;

  pnts[3].x = P3.X + W / 2;
  pnts[3].y = -P3.Y + H / 2;


  Polygon(hDC, pnts, 4);
}*/ /* End of 'DrawQuad' function */

VOID DrawGlobe( HDC hDC, INT W, INT H )
{
  DOUBLE phi, theta, R = 200, t = clock() / (DOUBLE)CLOCKS_PER_SEC;
  INT i, j, x, y, z;   

  for (i = 0; i < N; i++)
  {
    theta = i / (N - 1.0) * PI;
    for (j = 0; j < M; j++)
    {
      phi = j / (M - 1.0) * 2 * PI + t * 0.228;

      x = (int)(R * sin(theta) * sin(phi));
      y = (int)(R * cos(theta));
      z = (int)(R * sin(theta) * cos(phi));     
      x += W / 2;
      y += H / 2;
      if (z > 0)
        Ellipse(hDC, x - 5, y - 5, x + 5, y + 5);
    }
  }
}



/*END OF 'GLOBEDRAW.C' FILE*/