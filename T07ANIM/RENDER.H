/* FILE: RENDER.H
 * PROGRAMMER: DS6
 * DATE: 09.06.2015
 * PURPOSE: include stuff that renders stuff.
*/

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>


/* Тип представления объекта - набор вершин и треугольников */
typedef struct tagds6GOBJ
{
  INT
      VA,          /* массив вершин */
      VBuf, IBuf,  /* буфера вершин и индексов */
      NumOfV,      /* количество вершин */
      NumOfF;      /* количество граней */
} ds6GOBJ;

/* Матрицы */
extern MATR
  DS6_RndMatrWorld,
  DS6_RndMatrView,
  DS6_RndMatrProj;

/* Параметры проецирования */
extern DBL
  DS6_RndWp, DS6_RndHp,    /* размеры обрасти проецирования */
  DS6_RndProjDist;         /* расстояние до плоскости проекции */

/* Шейдер по умолчанию */
extern UINT DS6_RndProg;

/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       ds6GOBJ *GObj;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, FALSE иначе.
 */
BOOL DS6_RndGObjLoad( ds6GOBJ *GObj, CHAR *FileName );

/* Функция освобождения памяти из-под геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       ds6GOBJ *GObj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID DS6_RndGObjFree( ds6GOBJ *GObj );

/* Функция отрисовки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       ds6GOBJ *GObj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID DS6_RndGObjDraw( ds6GOBJ *GObj );

UINT DS6_ShaderLoad(CHAR *FileNamePrefix);

/* Функция уничтодения шейдеров для программы.
* АРНУМЕНТЫ:
*   - номер программы:
*       UINT PrgId;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
VOID DS6_ShaderFree(UINT PrgId);

#endif /* __RENDER_H_ */    

/* END OF 'RENDER.H' FILE */