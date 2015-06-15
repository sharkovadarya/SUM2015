/* FILENAME: RENDER.H
* PROGRAMMER: DS6
* PURPOSE: Rendering system declaration module.
* LAST UPDATE: 13.06.2015
*/

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

/* Матрицы */
extern MATR
  DS6_RndMatrWorld,
  DS6_RndMatrView,
  DS6_RndMatrProj,
  DS6_RndMatrWorldViewProj;

/* Матрица изменения примитива при создании */
extern MATR DS6_RndPrimMatrConvert;

/* Параметры проецирования */
extern DBL
  DS6_RndWp, DS6_RndHp,    /* размеры обрасти проецирования */
  DS6_RndProjDist;         /* расстояние до плоскости проекции */

/* Шейдер по умолчанию */
extern UINT DS6_RndProg;

/* Функция загрузки шейдеров для одной программы.
* АРНУМЕНТЫ:
*   - префикс имени файла:
*       CHAR *FileNamePrefix;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
*   (UINT) номер загруженной программы.
*/
UINT DS6_ShaderLoad( CHAR *FileNamePrefix );

/* Функция уничтодения шейдеров для программы.
* АРНУМЕНТЫ:
*   - номер программы:
*       UINT PrgId;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
VOID DS6_ShaderFree( UINT PrgId );

/***
 * Работа с материалами
 ***/

/* Тип задания материала поверхности */
typedef struct tagds6MATERIAL
{
  /* Имя материала */
  CHAR Name[300];

  /* Коэффициенты отражения */
  VEC Ka, Kd, Ks;
  FLT Kp; /* Степень "бликовости" */

  /* Коэффициент прозрачности */
  FLT Kt;

  /* Текстура поверхности */
  INT TexId;
} ds6MATERIAL;

/* Глобальная таблица материалов */
#define DS6_MATERIAL_MAX 300
extern INT DS6_MtlLibSize;
extern ds6MATERIAL DS6_MtlLib[DS6_MATERIAL_MAX];

/* Функция добавления материала в библиотеку.
 * АРГУМЕНТЫ:
 *   - структура с памаметрами материала:
 *       ds6MATERIAL *Mtl;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) порядковый номер добавленного материала (0 при ошибке).
 */
INT DS6_MtlAdd( ds6MATERIAL *Mtl );

/* Функция поиска материала по имени.
 * АРГУМЕНТЫ:
 *   - имя материала:
 *       CHAR *Name;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) порядковый номер найденного материала (0 при ошибке).
 */
INT DS6_MtlFind( CHAR *Name );

/* Функция загрузки материала из файла (*.MTL).
 * АРГУМЕНТЫ:
 *   - имя файла материала:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) количество загруженных материалов.
 */
INT DS6_MtlLoad( CHAR *FileName );


/***
* Работа с примитивами
***/

/* Виды примитивов */
typedef enum tagds6PRIM_TYPE
{
  DS6_PRIM_TRIMESH,  /* примитив - набор треугольников */
  DS6_PRIM_GRID      /* регулярная сетка треугольников (через triangle strip) */
} ds6PRIM_TYPE;

/* Тип хранения примитива */
typedef struct tagds6PRIM
{
  ds6PRIM_TYPE Type; /* Тип примитива (DS6_PRIM_***) */
  INT
      VA,          /* массив вершин */
      VBuf, IBuf,  /* буфера вершин и индексов */
      NumOfI,      /* количество индексов для вывода примитива */
      MtlNo;       /* номер материала из библиотеки */
} ds6PRIM;

/* Тип хранения текстурных координат */
typedef struct tagds6UV
{
  FLT U, V; /* текстурные координаты */
} ds6UV;

/* Функция задания текстурных координат.
* АРГУМЕНТЫ:
*   - новые значени координат:
*       FLT U, V;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
*   (ds6UV) построенные координаты.
*/
__inline ds6UV UVSet(FLT U, FLT V)
{
  ds6UV UV = {U, V};

  return UV;
} /* End of 'UVSet' function */

/* Тип хранения одной вершины паримитива */
typedef struct tagds6VERTEX
{
  VEC P;    /* позиция вершины */
  ds6UV T;  /* текстурные координаты вершины */
  VEC N;    /* нормаль в вершине */
  COLOR C;  /* цвет вершины */
} ds6VERTEX;

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
*/
VOID DS6_PrimCreate( ds6PRIM *Prim, ds6PRIM_TYPE Type, 
                     INT NoofV, INT NoofI, ds6VERTEX *Vertices, INT *Indices );

/* Функция удаления примитива.
* АРГУМЕНТЫ:
*   - указатель на примитив:
*       ds6PRIM *Prim;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
VOID DS6_PrimFree( ds6PRIM *Prim );

/* Функция отрисовки примитива.
* АРГУМЕНТЫ:
*   - указатель на примитив:
*       ds6PRIM *Prim;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
VOID DS6_PrimDraw( ds6PRIM *Prim );

/* Функция создания примитива плоскость.
* АРГУМЕНТЫ:
*   - указатель на примитив:
*       ds6PRIM *Prim;
*   - касательные вектора-стороны:
*       VEC Du, Dv;
*   - разбиение:
*       INT N, M;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
*   (BOOL) TRUE при успехе, иначе FALSE.
*/
BOOL DS6_PrimCreatePlane( ds6PRIM *Prim, VEC Du, VEC Dv, INT N, INT M );

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
BOOL DS6_PrimCreateSphere( ds6PRIM *Prim, VEC C, FLT R, INT N, INT M );

/* Функция загрузки геометрического объекта.
* АРГУМЕНТЫ:
*   - структура объекта для загрузки:
*       ds6PRIM *GObj;
*   - имя файла:
*       CHAR *FileName;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
*   (BOOL) TRUE при успехе, FALSE иначе.
*/
BOOL DS6_PrimLoad( ds6PRIM *GObj, CHAR *FileName );

/***
* Работа с геометрическими объектами
***/

/* Геометрический объект - набор примитивов */
typedef struct tagds6GEOM
{
  INT NumOfPrimitives; /* количество примитивов */
  ds6PRIM *Prims;      /* массив примитивов */
} ds6GEOM;

/* Функция добавления примитива к геометрическому объекту.
* АРГУМЕНТЫ:
*   - указатель на геометрический объект:
*       ds6GEOM *G;
*   - указатель на добавляемый примитив:
*       ds6PRIM *Prim;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
*   (INT) номер добавленного примитива в массиве (-1 при ошибке).
*/
INT DS6_GeomAddPrim( ds6GEOM *G, ds6PRIM *Prim );

/* Функция освобождения геометрического объекта.
* АРГУМЕНТЫ:
*   - указатель на геометрический объект:
*       ds6GEOM *G;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
VOID DS6_GeomFree( ds6GEOM *G );

/* Функция отображения геометрического объекта.
* АРГУМЕНТЫ:
*   - указатель на геометрический объект:
*       ds6GEOM *G;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
VOID DS6_GeomDraw( ds6GEOM *G );

/* Функция загрузки геометрического объекта из G3D файла.
* АРГУМЕНТЫ:
*   - указатель на геометрический объект:
*       ds6GEOM *G;
*   - имя файла:
*       CHAR *FileName;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
*   (BOOL) TRUE при успехе, иначе - FALSE.
*/
BOOL DS6_GeomLoad( ds6GEOM *G, CHAR *FileName );

/* Функция загрузки текстуры.
* АРГУМЕНТЫ:
*   - имя файла:
*       CHAR *FileName;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
*   (INT ) идентификатор OpenGL для текстуры.
*/
INT DS6_TextureLoad( CHAR *FileName );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
