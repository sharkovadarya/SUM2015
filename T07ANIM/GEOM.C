/* FILENAME: GEOM.C
 * PROGRAMMER: DS6
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 13.06.2015
 */

#include <stdio.h>
#include <stdlib.h>

#include "render.h"

/* Функция добавления примитива к геометрическому объекту.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       ds6GEOM *G;
 *   - указатель на добавляемый примитив:
 *       ds6PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) номер добавленного примитива в массиве (-1 при ошибке).
 */
INT DS6_GeomAddPrim( ds6GEOM *G, ds6PRIM *Prim )
{
  ds6PRIM *new_bulk;

  /* Выделяем память под новый массив примитивов */
  if ((new_bulk = malloc(sizeof(ds6PRIM) * (G->NumOfPrimitives + 1))) == NULL)
    return -1;

  /* Копируем старые примитивы */
  if (G->Prims != NULL)
  {
    memcpy(new_bulk, G->Prims, sizeof(ds6PRIM) * G->NumOfPrimitives);
    free(G->Prims);
  }
  /* указываем на новый массив примитивлв */
  G->Prims = new_bulk;

  /* Сохраняем новый элемент */
  G->Prims[G->NumOfPrimitives] = *Prim;
  return G->NumOfPrimitives++;
} /* End of 'DS6_GeomAddPrim' function */

/* Функция освобождения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       ds6GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID DS6_GeomFree( ds6GEOM *G )
{
  INT i;

  if (G->Prims != NULL)
  {
    for (i = 0; i < G->NumOfPrimitives; i++)
      DS6_PrimFree(&G->Prims[i]);
    free(G->Prims);
  }
  memset(G, 0, sizeof(ds6GEOM));
} /* End of 'DS6_GeomFree' function */

/* Функция отображения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       ds6GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID DS6_GeomDraw( ds6GEOM *G )
{
  INT i, loc;

  for (i = 0; i < G->NumOfPrimitives; i++)
  {
    glUseProgram(DS6_RndProg);
    loc = glGetUniformLocation(DS6_RndProg, "PartNo");
    if (loc != -1)
      glUniform1f(loc, i);
    glUseProgram(0);
    DS6_PrimDraw(&G->Prims[i]);
  }
} /* End of 'DS6_GeomDraw' function */

/* Функция загрузки геометрического объекта из G3D файла.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       ds6GEOM *G;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе - FALSE.
 */
BOOL DS6_GeomLoad( ds6GEOM *G, CHAR *FileName )
{
  FILE *F;
  INT i, j, n;
  CHAR Sign[4];
  MATR M;
  static CHAR MtlName[300];

  memset(G, 0, sizeof(ds6GEOM));
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  M = MatrTranspose(MatrInverse(DS6_RndPrimMatrConvert));

  /* читаем сигнатуру */
  fread(Sign, 1, 4, F);
  if (*(DWORD *)Sign != *(DWORD *)"G3D")
  {
    fclose(F);
    return FALSE;
  }

  /* читаем количество примитивов в объекте */
  fread(&n, 4, 1, F);
  fread(MtlName, 1, 300, F);

  /* читаем примитивы */
  for (i = 0; i < n; i++)
  {
    INT nv, ni, *Ind;
    ds6VERTEX *Vert;
    ds6PRIM P;

    /* читаем количество вершин и индексов */
    fread(&nv, 4, 1, F);
    fread(&ni, 4, 1, F);
    /* читаем имя материала текущего примитива */
    fread(MtlName, 1, 300, F);

    /* выделяем память под вершины и индексы */
    if ((Vert = malloc(sizeof(ds6VERTEX) * nv + sizeof(INT) * ni)) == NULL)
      break;
    Ind = (INT *)(Vert + nv);

    /* читаем данные */
    fread(Vert, sizeof(ds6VERTEX), nv, F);
    /* конвертируем геометрию */
    for (j = 0; j < nv; j++)
    {
      Vert[j].P = PointTransform(Vert[j].P, DS6_RndPrimMatrConvert);
      Vert[j].N = VectorTransform(Vert[j].N, M);
    }
    fread(Ind, sizeof(INT), ni, F);

    /* заносим в примитив */
    DS6_PrimCreate(&P, DS6_PRIM_TRIMESH, nv, ni, Vert, Ind);

    free(Vert);

    /* добавляем примитив к объекту */
    DS6_GeomAddPrim(G, &P);
  }
  fclose(F);
  DS6_RndPrimMatrConvert = MatrIdentity();
  return TRUE;
} /* End of 'DS6_GeomDraw' function */

/* END OF 'GEOM.C' FILE */
