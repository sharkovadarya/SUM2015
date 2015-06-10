/* FILE: RENDER.C
 * PROGRAMMER: DS6
 * LAST UPDATE: 10.06.2015
 * PURPOSE: render stuff. 
 */

#include <stdio.h>
#include <string.h>

#include "vec.h"
#include "anim.h"
#include "render.h"


/* Матрицы */
MATR
  DS6_RndMatrWorld = DS6_UNIT_MATR,
  DS6_RndMatrView = DS6_UNIT_MATR,
  DS6_RndMatrWorldView = DS6_UNIT_MATR;

/* Параметры проецирования */
DBL
  DS6_RndWp = 3, DS6_RndHp = 3,     /* размеры обрасти проецирования */
  DS6_RndProjDist = 5;              /* расстояние до плоскости проекции */

/* Функция преобразования из мировой системы координат в кадр.
 * АРГУМЕНТЫ:
 *   - исходная точка:
 *       VEC P;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (POINT) координаты в кадре.
 */
POINT DS6_RndWorldToScreen( VEC P )
{
  POINT Ps;
  VEC Pp;

  /* преобразование СК */
  P = VectorTransform(P, DS6_RndMatrWorldView);

  Pp.X = P.X * DS6_RndProjDist / (- P.Z);
  Pp.Y = P.Y * DS6_RndProjDist / (- P.Z);

  Ps.x = DS6_Anim.W / 2 + Pp.X * DS6_Anim.W / DS6_RndWp;
  Ps.y = DS6_Anim.H / 2 - Pp.Y * DS6_Anim.H / DS6_RndHp;

  return Ps;
} /* End of 'DS6_RndWorldToScreen' function */


/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       DS6GOBJ *GObj;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, FALSE иначе.
 */
BOOL DS6_RndGObjLoad( ds6GOBJ *GObj, CHAR *FileName )
{
  FILE *F;
  INT nv = 0, nf = 0;
  static CHAR Buf[10000];

  memset(GObj, 0, sizeof(ds6GOBJ));
  /* Open file */
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Count vertices */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;
  }

  /* Allocate memory for data */
  if ((GObj->V = malloc(sizeof(VEC) * nv + sizeof(INT [3]) * nf)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  GObj->F = (INT (*)[3])(GObj->V + nv);

  /* Read vertices */
  rewind(F);
  nv = nf = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%lf%lf%lf",
        &GObj->V[nv].X, &GObj->V[nv].Y, &GObj->V[nv].Z);
      nv++;
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT a, b, c;

      sscanf(Buf + 2, "%i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &a, &b, &c) == 3 ||
        sscanf(Buf + 2, "%i//%*i %i//%*i %i//%*i", &a, &b, &c) == 3 ||
        sscanf(Buf + 2, "%i/%*i %i/%*i %i/%*i", &a, &b, &c) == 3 ||
        sscanf(Buf + 2, "%i %i %i", &a, &b, &c);

      GObj->F[nf][0] = a - 1;
      GObj->F[nf][1] = b - 1;
      GObj->F[nf][2] = c - 1;
      nf++;
    }
  }

  fclose(F);

  GObj->NumOfV = nv;
  GObj->NumOfF = nf;
  return TRUE;
} /* End of 'DS6_RndGObjLoad' function */

/* Функция отрисовки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       DS6GOBJ *GObj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID DS6_RndGObjDraw( ds6GOBJ *GObj )
{
  INT i;
  POINT *pnts;

  if ((pnts = malloc(sizeof(POINT) * GObj->NumOfV)) == NULL)
    return;

  /* проецируем все точки */
  DS6_RndMatrWorldView = MatrMulMatr(DS6_RndMatrWorld, DS6_RndMatrView);
  for (i = 0; i < GObj->NumOfV; i++)
    pnts[i] = DS6_RndWorldToScreen(GObj->V[i]);

  /* рисуем треугольники */
  for (i = 0; i < GObj->NumOfF; i++)
  {
    INT
      n1 = GObj->F[i][0],
      n2 = GObj->F[i][1],
      n3 = GObj->F[i][2];

    MoveToEx(DS6_Anim.hDC, pnts[n1].x, pnts[n1].y, NULL);
    LineTo(DS6_Anim.hDC, pnts[n2].x, pnts[n2].y);
    LineTo(DS6_Anim.hDC, pnts[n3].x, pnts[n3].y);
    LineTo(DS6_Anim.hDC, pnts[n1].x, pnts[n1].y);
  }

  free(pnts);
} /* End of 'DS6_RndGObjDraw' function */

/* Функция освобождения памяти из-под геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       DS6GOBJ *GObj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID DS6_RndGObjFree( ds6GOBJ *GObj )
{
  free(GObj->V);
  memset(GObj, 0, sizeof(ds6GOBJ));
} /* End of 'DS6_RndGObjFree' function */


/* END OF 'RENDER.C' FILE */