/* FILENAME: UNITS.H
 * PROGRAMMER: DS6
 * PURPOSE: Animation units samples declaration module.
 * LAST UPDATE: 08.06.2015
 */

#ifndef __UNITS_H_
#define __UNITS_H_

#include "def.h"


/* Функция создания объекта анимации "мяч".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ds6UNIT *) указатель на созданный объект анимации.
 */
ds6UNIT * DS6_UnitBallCreate( VOID );
ds6UNIT * DS6_UnitTriangleCreate( VOID );

#endif /* __UNITS_H_ */

/* END OF 'UNITS.H' FILE */
