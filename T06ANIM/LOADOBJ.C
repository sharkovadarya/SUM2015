/* FILENAME: LOADOBJ.C
 * PROGRAMMER: DS6
 * PURPOSE: Load 3D nmodel from *.OBJ files.
 * LAST UPDATE: 06.06.2015
 */

#pragma warning(disable: 4244) 

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <time.h>

#include "vec.h"

/* Global model data */

/* Vertex array */
VEC *ObjV; /* Vertex coordinates */
INT ObjNumOfV; /* Number of model vertices */   



/* Draw object functioln.
 * ARGUMENTS:
 *   - object (*.OBJ) file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if loading is success, FALSE otherwise.
 */
BOOL ObjLoad( CHAR *FileName )
{
  FILE *F;
  INT nv = 0;
  static CHAR Buf[10000];

  /* Open file */
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Count vertices */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
  }

  /* Allocate memory for data */
  if ((ObjV = malloc(sizeof(VEC) * nv)) == NULL)
  {
    fclose(F);
    return FALSE;
  }

  /* Read vertices */
  rewind(F);
  nv = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%lf%lf%lf",
        &ObjV[nv].X, &ObjV[nv].Y, &ObjV[nv].Z);
      ObjV[nv] = VecMulNum(ObjV[nv], 60);
      nv++;
    }
  }

  fclose(F);

  ObjNumOfV = nv;
  return TRUE;
} /* End of 'ObjLoad' function */

/* Draw object functioln.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID ObjDraw( HDC hDC, INT W, INT H )
{
  INT i;  

  for (i = 0; i < ObjNumOfV; i++)                            
  {
    ObjV[i] = VectorTransform(ObjV[i], MatrMulMatr(MatrRotate(sin(clock() / (DBL)(CLOCKS_PER_SEC)) * 2, 2, 2, 8), MatrRotate(2, 2, rand() / 5, 0)));    
    Ellipse(hDC, W / 2 + ObjV[i].X - 5, H / 2 - ObjV[i].Y - 5, W / 2 + ObjV[i].X + 5, H / 2 - ObjV[i].Y + 5); 
  }

} /* End of 'ObjDraw' function */

/* END OF 'LOADOBJ.C' FILE */
