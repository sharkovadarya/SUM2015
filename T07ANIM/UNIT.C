/* FILENAME: UNIT.C
 * PROGRAMMER: DS6
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>

#include "anim.h"

/* Функция  инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ds6UNIT *Uni;
 *   - указатель на контекст анимации:
 *       ds6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID DS6_AnimUnitInit( ds6UNIT *Uni, ds6ANIM *Ani )
{
} /* End of 'DS6_AnimUnitInit' function */

/* Функция  деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ds6UNIT *Uni;
 *   - указатель на контекст анимации:
 *       ds6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID DS6_AnimUnitClose( ds6UNIT *Uni, ds6ANIM *Ani )
{
} /* End of 'DS6_AnimUnitClose' function */

/* Функция  обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ds6UNIT *Uni;
 *   - указатель на контекст анимации:
 *       ds6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID DS6_AnimUnitResponse( ds6UNIT *Uni, ds6ANIM *Ani )
{
} /* End of 'DS6_AnimUnitResponse' function */

/* Функция  построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ds6UNIT *Uni;
 *   - указатель на контекст анимации:
 *       ds6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID DS6_AnimUnitRender( ds6UNIT *Uni, ds6ANIM *Ani )
{
} /* End of 'DS6_AnimUnitRender' function */

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ:
 *   - размер структуры объекта анимации:
 *       INT Size;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ds6UNIT *) указатель на созданный объект анимации.
 */
ds6UNIT * DS6_AnimUnitCreate( INT Size )
{
  ds6UNIT *Uni;

  if (Size < sizeof(ds6UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* заполняем поля  */
  Uni->Size = Size;
  Uni->Init = DS6_AnimUnitInit;
  Uni->Close = DS6_AnimUnitClose;
  Uni->Response = DS6_AnimUnitResponse;
  Uni->Render = DS6_AnimUnitRender;
  return Uni;
} /* End of 'DS6_AnimUnitCreate' function */

/* END OF 'UNIT.C' FILE */
