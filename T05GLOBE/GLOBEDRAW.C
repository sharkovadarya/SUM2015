/* FILE: GLOBEDRAW.C
 * PROGRAMMER: DS6
 * DATE: 05.06.2015
 * PURPOSE: Globe drawing declaration module.
*/
#include <math.h>
#include <windows.h>
#include <time.h>
#include "globe.h"
#include "image.h"
static VEC Grid[N][M];
static IMAGE GlobeImage;

/* Функция поворота точки вокруг оси X */
VEC RotateX( VEC P, DOUBLE AngleDegree )
{
  DOUBLE a = AngleDegree * PI / 180, si = sin(a), co = cos(a);
  VEC r;

  r.X = P.X;
  r.Y = P.Y * co - P.Z * si;
  r.Z = P.Y * si + P.Z * co;
  return r;
} /* End of 'RotateX' function */



/* Рисование четырехугольника */
VOID DrawQuad( HDC hDC, VEC P0, VEC P1, VEC P2, VEC P3, INT W, INT H )
{

  POINT pnts[4];



  pnts[0].x = P0.X + W / 2;
  pnts[0].y = -P0.Y + H / 2;

  pnts[1].x = P1.X + W / 2;
  pnts[1].y = -P1.Y + H / 2;

  pnts[2].x = P2.X + W / 2;
  pnts[2].y = -P2.Y + H / 2;

  pnts[3].x = P3.X + W / 2;
  pnts[3].y = -P3.Y + H / 2;

  if ((pnts[0].x - pnts[1].x) * (pnts[0].y + pnts[1].y) +
    (pnts[1].x - pnts[2].x) * (pnts[1].y + pnts[2].y) +
    (pnts[2].x - pnts[3].x) * (pnts[2].y + pnts[3].y) +
    (pnts[3].x - pnts[0].x) * (pnts[3].y + pnts[0].y) < 0)
  return;



  Polygon(hDC, pnts, 4);
} /* End of 'DrawQuad' function */

VOID DrawGlobe( HDC hDC, INT W, INT H )
{   
  INT i, j, x, y, ix, iy;   

  for (i = 0; i < N - 1; i++)
  {
    for (j = 0; j < M - 1; j++)
    {     
      BuildGlobe();
      ix = j * (GlobeImage.W - 1) / (M - 1);
      iy = i * (GlobeImage.H - 1) / (N - 1);
      x = W / 2 + Grid[i][j].X;
      y = H / 2 - Grid[i][j].Y; 
      if (GlobeImage.hBm == NULL)
        ImageLoad(&GlobeImage, "globe.bmp");
      SelectObject(hDC, GetStockObject(DC_BRUSH));
      SetDCBrushColor(hDC, ImageGetP(&GlobeImage, ix, iy));       
    
      DrawQuad(hDC, Grid[i][j], Grid[i + 1][j], Grid[i + 1][j + 1], Grid[i][j + 1], W, H);      
    }
  }
}

VOID BuildGlobe( void )
{
    DOUBLE phi, theta, R = 200, t = clock() / (DOUBLE)CLOCKS_PER_SEC;
  INT i, j;   

  for (i = 0; i < N; i++)
  {
    theta = i / (N - 1.0) * PI;
    for (j = 0; j < M; j++)
    {
      phi = j / (M - 1.0) * 2 * PI + t * 2.28;

      Grid[i][j].X = (int)(R * sin(theta) * sin(phi));
      Grid[i][j].Y = (int)(R * cos(theta));
      Grid[i][j].Z = (int)(R * sin(theta) * cos(phi));         
      
    }
  } 
}


/*END OF 'GLOBEDRAW.C' FILE*/