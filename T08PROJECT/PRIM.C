/* FILENAME: PRIM.C
* PROGRAMMER: DS6
* PURPOSE: Primitive handle module.
* LAST UPDATE: 13.06.2015
*/

#include <stdlib.h>

#include "anim.h"
#include "render.h"

/* Матрица изменения примитива при создании */
MATR DS6_RndPrimMatrConvert = DS6_UNIT_MATR;

/* Цвет новых объектов по умолчанию */
COLOR DS6_RndPrimDefaultColor = {1, 1, 1, 1};

/* Функция создания примитива.
* АРГУМЕНТЫ:
*   - указатель на примитив:
*       ds6PRIM *Prim;
*   - тип примитива (DS6_PRIM_***):
*       ds6PRIM_TYPE Type;
*   - количество вершин и индексов:
*       INT NoofV, NoofI;
*   - массив вершин:
*       ds6VERTEX *Vertices;
*   - массив индексов:
*       INT *Indices;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*   (BOOL) TRUE при успехе, иначе FALSE.
*/
VOID DS6_PrimCreate(ds6PRIM *Prim, ds6PRIM_TYPE Type,
                    INT NoofV, INT NoofI, ds6VERTEX *Vertices, INT *Indices)
{
  Prim->Type = Type;
  Prim->NumOfI = NoofI;
  /* Создаем буфера OpenGL */
  glGenVertexArrays(1, &Prim->VA);
  glGenBuffers(1, &Prim->VBuf);
  glGenBuffers(1, &Prim->IBuf);

  /* делаем активным массив вершин */
  glBindVertexArray(Prim->VA);
  /* делаем активным буфер вершин */
  glBindBuffer(GL_ARRAY_BUFFER, Prim->VBuf);
  /* сливаем данные */
  glBufferData(GL_ARRAY_BUFFER, sizeof(ds6VERTEX) * NoofV, Vertices, GL_STATIC_DRAW);
  /* делаем активным буфер индексов */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Prim->IBuf);
  /* сливаем данные */
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NoofI, Indices, GL_STATIC_DRAW);

  /* задаем порядок данных */
  /*                    layout,
   *                       количество компонент,
   *                          тип,
   *                                    надо ли нормировать,
   *                                           размер в байтах одного элемента буфера,
   *                                                           смещение в байтах до начала данных */
  glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(ds6VERTEX), (VOID *)0); /* позиция */
  glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(ds6VERTEX), (VOID *)sizeof(VEC)); /* текстура */
  glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(ds6VERTEX), (VOID *)(sizeof(VEC) + sizeof(ds6UV))); /* нормаль */
  glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(ds6VERTEX), (VOID *)(sizeof(VEC) * 2 + sizeof(ds6UV))); /* цвет */

  /* включаем нужные аттрибуты (layout) */
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);

  /* отключаемся от массива вершин */
  glBindVertexArray(0);
} /* End of 'DS6_PrimCreate' function */

/* Функция удаления примитива.
* АРГУМЕНТЫ:
*   - указатель на примитив:
*       ds6PRIM *Prim;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
VOID DS6_PrimFree(ds6PRIM *Prim)
{
  /* делаем активным массив вершин */
  glBindVertexArray(Prim->VA);
  /* "отцепляем" буфера */
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Prim->VBuf);
  glDeleteBuffers(1, &Prim->IBuf);
  /* делаем неактивным массив вершин */
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Prim->VA);

  memset(Prim, 0, sizeof(ds6PRIM));
} /* End of 'DS6_PrimFree' function */


/* Background primitive drawing function.
* АРГУМЕНТЫ:
*   - указатель на примитив:
*       ds6PRIM *Background;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
VOID DS6_BackgroundDraw( ds6PRIM *Background )
{
  INT loc;
  MATR M;

  /* рисуем треугольники */
  glBindVertexArray(Background->VA);
  glUseProgram(DS6_RndProg);

  loc = glGetUniformLocation(DS6_RndProg, "MatrWorld");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, DS6_RndMatrWorld.A[0]);
  loc = glGetUniformLocation(DS6_RndProg, "MatrView");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, DS6_RndMatrView.A[0]);
  loc = glGetUniformLocation(DS6_RndProg, "MatrProj");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, DS6_RndMatrProj.A[0]);
  loc = glGetUniformLocation(DS6_RndProg, "MatrWVP");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, DS6_RndMatrWorldViewProj.A[0]);

  M = MatrTranspose(MatrInverse(MatrMulMatr(DS6_RndMatrWorld, DS6_RndMatrView)));
  loc = glGetUniformLocation(DS6_RndProg, "MatrWVInverse");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);

  M = MatrTranspose(MatrInverse(DS6_RndMatrWorld));
  loc = glGetUniformLocation(DS6_RndProg, "MatrWInverse");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);

  M = MatrMulMatr(DS6_RndMatrWorld, DS6_RndMatrView);
  loc = glGetUniformLocation(DS6_RndProg, "MatrWV");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);

  loc = glGetUniformLocation(DS6_RndProg, "Time");
  if (loc != -1)
    glUniform1f(loc, DS6_Anim.Time);

  glPrimitiveRestartIndex(0xFFFFFFFF);
  if (Background->Type == DS6_PRIM_GRID)
    glDrawElements(GL_TRIANGLE_STRIP, Background->NumOfI, GL_UNSIGNED_INT, NULL);
  else
    glDrawElements(GL_TRIANGLES, Background->NumOfI, GL_UNSIGNED_INT, NULL);

  glUseProgram(0);
  glBindVertexArray(0);    
} /* End of 'DS6_BackgroundDraw' function. */

/* Функция отрисовки примитива.
* АРГУМЕНТЫ:
*   - указатель на примитив:
*       ds6PRIM *Prim;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
VOID DS6_PrimDraw( ds6PRIM *Prim )
{
  INT loc;
  MATR M;

  DS6_RndMatrWorldViewProj = MatrMulMatr(MatrMulMatr(DS6_RndMatrWorld, DS6_RndMatrView), DS6_RndMatrProj);

  /* оставлено для отлдадки, если нет шейдеров */
  glLoadMatrixf(DS6_RndMatrWorldViewProj.A[0]);

  /* рисуем треугольники */
  glBindVertexArray(Prim->VA);
  glUseProgram(DS6_SphereShader);

  loc = glGetUniformLocation(DS6_SphereShader, "MatrWorld");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, DS6_RndMatrWorld.A[0]);
  loc = glGetUniformLocation(DS6_SphereShader, "MatrView");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, DS6_RndMatrView.A[0]);
  loc = glGetUniformLocation(DS6_SphereShader, "MatrProj");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, DS6_RndMatrProj.A[0]);
  loc = glGetUniformLocation(DS6_SphereShader, "MatrWVP");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, DS6_RndMatrWorldViewProj.A[0]);

  M = MatrTranspose(MatrInverse(MatrMulMatr(DS6_RndMatrWorld, DS6_RndMatrView)));
  loc = glGetUniformLocation(DS6_SphereShader, "MatrWVInverse");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);

  M = MatrTranspose(MatrInverse(DS6_RndMatrWorld));
  loc = glGetUniformLocation(DS6_SphereShader, "MatrWInverse");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);

  M = MatrMulMatr(DS6_RndMatrWorld, DS6_RndMatrView);
  loc = glGetUniformLocation(DS6_SphereShader, "MatrWV");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);

  loc = glGetUniformLocation(DS6_SphereShader, "Time");
  if (loc != -1)
    glUniform1f(loc, DS6_Anim.Time);

  /* Применение материала */
  loc = glGetUniformLocation(DS6_SphereShader, "Ka");
  if (loc != -1)
    glUniform3fv(loc, 1, &DS6_MtlLib[Prim->MtlNo].Ka.X);
  loc = glGetUniformLocation(DS6_SphereShader, "Kd");
  if (loc != -1)
    glUniform3fv(loc, 1, &DS6_MtlLib[Prim->MtlNo].Kd.X);
  loc = glGetUniformLocation(DS6_SphereShader, "Ks");
  if (loc != -1)
    glUniform3fv(loc, 1, &DS6_MtlLib[Prim->MtlNo].Ks.X);
  loc = glGetUniformLocation(DS6_SphereShader, "Kp");
  if (loc != -1)
    glUniform1f(loc, DS6_MtlLib[Prim->MtlNo].Kp);
  loc = glGetUniformLocation(DS6_SphereShader, "Kt");
  if (loc != -1)
    glUniform1f(loc, DS6_MtlLib[Prim->MtlNo].Kt);

  loc = glGetUniformLocation(DS6_SphereShader, "IsTextureUse");
  if (DS6_MtlLib[Prim->MtlNo].TexId == 0)
    glUniform1f(loc, 0);
  else
  {
    glUniform1f(loc, 1);
    glBindTexture(GL_TEXTURE_2D, DS6_MtlLib[Prim->MtlNo].TexId);
  }                   

  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(0xFFFFFFFF);
  if (Prim->Type == DS6_PRIM_GRID)
    glDrawElements(GL_TRIANGLE_STRIP, Prim->NumOfI, GL_UNSIGNED_INT, NULL);
  else
    glDrawElements(GL_TRIANGLES, Prim->NumOfI, GL_UNSIGNED_INT, NULL);

  glUseProgram(0);
  glBindVertexArray(0);

} /* End of 'DS6_PrimDraw' function */
/* Функция создания примитива плоскость.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       ds6PRIM *Prim;
 *   - угловая точка:
 *       VEC Loc;
 *   - касательные вектора-стороны:
 *       VEC Du, Dv;
 *   - разбиение:
 *       INT N, M;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе FALSE.
 */
BOOL DS6_PrimCreatePlane( ds6PRIM *Prim, VEC Loc, VEC Du, VEC Dv, INT N, INT M )
{
  INT i, j;
  VEC norm;
  INT *Ind, *iptr;
  ds6VERTEX *V, *ptr;

  memset(Prim, 0, sizeof(ds6PRIM));

  if ((V = malloc(sizeof(ds6VERTEX) * N * M +
                  sizeof(INT) * ((2 * M + 1) * (N - 1)))) == NULL)
    return FALSE;
  Ind = (INT *)(V + N * M);

  /* заполняем вершины */
  norm = VecNormalize(VecCrossVec(Du, Dv));
  for (ptr = V, i = 0; i < N; i++)
    for (j = 0; j < M; j++, ptr++)
    {
      ptr->P = VecAddVec(Loc,
                 VecAddVec(VecMulNum(Du, j / (M - 1.0)),
                           VecMulNum(Dv, i / (N - 1.0))));
      ptr->C = DS6_RndPrimDefaultColor;
      ptr->N = norm;
      ptr->T = UVSet(j / (M - 1.0), i / (N - 1.0));
    }

  /* заполняем индексы */
  for (iptr = Ind, i = 0; i < N - 1; i++)
  {
    for (j = 0; j < M; j++)
    {
      /* верхний */
      *iptr++ = i * M + j + M;
      /* нижний */
      *iptr++ = i * M + j;
    }
    /* сохраняем индекс разрыва примитива */
    *iptr++ = 0xFFFFFFFF;
  }

  DS6_PrimCreate(Prim, DS6_PRIM_GRID, M * N, (2 * M + 1) * (N - 1), V, Ind);

  free(V);

  return TRUE;
} /* End of 'DS6_PrimCreatePlane' function */



/* Функция создания примитива сфера.
* АРГУМЕНТЫ:
*   - указатель на примитив:
*       ds6PRIM *Prim;
*   - центр сферы:
*       VEC С;
*   - радиус сферы:
*       FLT R;
*   - разбиение:
*       INT N, M;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
*   (BOOL) TRUE при успехе, иначе FALSE.
*/
BOOL DS6_PrimCreateSphere( ds6PRIM *Prim, VEC C, FLT R, INT N, INT M )
{
  INT i, j;
  INT *Ind, *iptr;
  ds6VERTEX *V, *ptr;

  memset(Prim, 0, sizeof(ds6PRIM));

  if ((V = malloc(sizeof(ds6VERTEX) * N * M +
                  sizeof(INT) * ((2 * M + 1) * (N - 1)))) == NULL)
    return FALSE;
  Ind = (INT *)(V + N * M);

  /* заполняем вершины */
  for (ptr = V, i = 0; i < N; i++)
    for (j = 0; j < M; j++, ptr++)
    {
      DBL
        theta = PI * i / (N - 1.0), phi = 2 * PI * j / (M - 1.0),
        x = sin(theta) * sin(phi),
        y = cos(theta),
        z = sin(theta) * cos(phi);

      ptr->P = VecAddVec(C, VecSet(R * x, R * y, R * z));
      ptr->C = DS6_RndPrimDefaultColor;
      ptr->N = VecSet(x, y, z);
      ptr->T = UVSet(j / (M - 1.0), i / (N - 1.0));
    }

  /* заполняем индексы */
  for (iptr = Ind, i = 0; i < N - 1; i++)
  {
    for (j = 0; j < M; j++)
    {
      /* верхний */
      *iptr++ = i * M + j + M;
      /* нижний */
      *iptr++ = i * M + j;
    }
    /* сохраняем индекс разрыва примитива */
    *iptr++ = 0xFFFFFFFF;
  }

  DS6_PrimCreate(Prim, DS6_PRIM_GRID, M * N, (2 * M + 1) * (N - 1), V, Ind);
  free(V);    
  return TRUE;
} /* End of 'DS6_PrimCreateSphere' function */

/* END OF 'PRIM.C' FILE */
