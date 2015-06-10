/* FILE: RENDER.H
 * PROGRAMMER: DS6
 * DATE: 09.06.2015
 * PURPOSE: include stuff that renders stuff.
*/

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

/* Тип представления объекта - набор вершин и треугольников */
typedef struct tagds6GOBJ
{
  VEC *V;      /* массив вершин объекта */
  INT NumOfV;  /* количество вершин */
  INT (*F)[3]; /* массив граней объекта */
  INT NumOfF;  /* количество граней */
} ds6GOBJ;

/* Матрицы */
extern MATR
  DS6_RndMatrWorld,
  DS6_RndMatrView;

/* Параметры проецирования */
extern DBL
  DS6_RndWp, DS6_RndHp,    /* размеры обрасти проецирования */
  DS6_RndProjDist;         /* расстояние до плоскости проекции */

/* Функция преобразования из мировой системы координат в кадр.
 * АРГУМЕНТЫ:
 *   - исходная точка:
 *       VEC P;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (POINT) координаты в кадре.
 */
POINT DS6_RndWorldToScreen( VEC P );

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

#endif /* __RENDER_H_ */    

/* END OF 'RENDER.H' FILE */