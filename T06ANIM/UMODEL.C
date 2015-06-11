/* FILENAME: UMODEL.C
 * PROGRAMMER: DS6
 * PURPOSE: Model unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include <string.h> 
#include "anim.h"
#include "vec.h"
#include "render.h"

/* MODEL image type */
typedef struct tagds6UNIT_MODEL
{
  DS6_UNIT_BASE_FIELDS;
  ds6GOBJ Model;
} ds6UNIT_MODEL;

/* Animation object initialization function.
 * ARGUMENTS:
 *   - self-pointer - animation object itself:
 *       ds6UNIT_MODEL *Uni;
 *   - animation context pointer:
 *       ds6ANIM *Ani;
 * RETURNS: none.
 */
static VOID DS6_AnimUnitInit( ds6UNIT_MODEL *Uni, ds6ANIM *Ani )
{  
  DS6_RndGObjLoad(&Uni->Model, "cow.object");
} /* End of 'DS6_AnimUnitInit' function */

/* Animation object deinitialization function.
 * ARGUMENTS:
 *   - указатель на "себя" - сам объект анимации:
 *       ds6UNIT_MODEL *Uni;
 *   - animation context pointer:
 *       ds6ANIM *Ani;
 * RETURNS: none.
 */
static VOID DS6_AnimUnitClose( ds6UNIT_MODEL *Uni, ds6ANIM *Ani )
{
} /* End of 'DS6_AnimUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - self-pointer - animation object itself:
 *       ds6UNIT_MODEL *Uni;
 *   - animation context pointer:
 *       ds6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID DS6_AnimUnitResponse( ds6UNIT_MODEL *Uni, ds6ANIM *Ani )
{
} /* End of 'DS6_AnimUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ds6UNIT_MODEL *Uni;
 *   - указатель на контекст анимации:
 *       ds6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID DS6_AnimUnitRender( ds6UNIT_MODEL *Uni, ds6ANIM *Ani )
{
  
  DS6_RndMatrWorld = MatrMulMatr(MatrMulMatr(
		MatrRotateY(30 * Ani->Time + 10 * Ani->JR), MatrScale(0.05, 0.05, Ani->JZ)),
		MatrTranslate(Ani->JX, -Ani->JY, 10 * Ani->JZ));

  DS6_RndMatrView = MatrView(VecSet(0 + DS6_GamePadShift, 0 + DS6_GamePadShift, 1 + DS6_GamePadShift), 
                             VecSet(0 + DS6_GamePadShift, 0 + DS6_GamePadShift, 0 + DS6_GamePadShift), 
                             VecSet(0 + DS6_GamePadShift, 1 + DS6_GamePadShift, 0 + DS6_GamePadShift));

  SelectObject(Ani->hDC, GetStockObject(WHITE_PEN));   
  DS6_RndGObjDraw(&Uni->Model); 
  
} /* End of 'DS6_AnimUnitRender' function */

/* Функция создания объекта анимации "мяч".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ds6UNIT *) указатель на созданный объект анимации.
 */
ds6UNIT * DS6_UnitModelCreate( VOID )
{
  ds6UNIT_MODEL *Uni;            

  if ((Uni = (VOID *)DS6_AnimUnitCreate(sizeof(ds6UNIT_MODEL))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)DS6_AnimUnitInit;
  Uni->Close = (VOID *)DS6_AnimUnitClose;
  Uni->Response = (VOID *)DS6_AnimUnitResponse;
  Uni->Render = (VOID *)DS6_AnimUnitRender;
  return (ds6UNIT *)Uni;
} /* End of 'DS6_UnitModelCreate' function */

/* END OF 'UMODEL.C' FILE */
