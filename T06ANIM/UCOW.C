/* FILENAME: UCOW.C
 * PROGRAMMER: DS6
 * PURPOSE: Cow unit handle module.
 * LAST UPDATE: 09.06.2015
 */

#include <string.h> 
#include <time.h>
#include "anim.h"
#include "vec.h"
#include "render.h"

/* BALL image type */
typedef struct tagds6UNIT_BALL
{
  DS6_UNIT_BASE_FIELDS;
    
  DWORD Color; /* Cow color */
  VEC Pos;     /* Cow position */ 
  DBL
    Amplitude,  /* Амплитуда */
    PhaseShift, /* Сдвиг по фазе */
    ScaleShift;  
} ds6UNIT_BALL;

/* Animation object initialization function.
 * ARGUMENTS:
 *   - self-pointer - animation object itself:
 *       ds6UNIT_BALL *Uni;
 *   - animation context pointer:
 *       ds6ANIM *Ani;
 * RETURNS: none.
 */
static VOID DS6_AnimUnitInit( ds6UNIT_BALL *Uni, ds6ANIM *Ani )
{   
  Uni->Color = RGB(rand() % 256, rand() % 256, rand() % 256);  
  Uni->Pos = VecSet(Ani->W + rand() / RAND_MAX, Ani->H + rand() / RAND_MAX, 0); 
  Uni->PhaseShift = rand() % 3000;
  Uni->ScaleShift = 5 + 0.30 * rand() / RAND_MAX;
  Uni->Amplitude = 30 + 59.0 * rand() / RAND_MAX;
  DS6_RndGObjLoad("cow.object");
} /* End of 'DS6_AnimUnitInit' function */

/* Animation object deinitialization function.
 * ARGUMENTS:
 *   - указатель на "себя" - сам объект анимации:
 *       ds6UNIT_BALL *Uni;
 *   - animation context pointer:
 *       ds6ANIM *Ani;
 * RETURNS: none.
 */
static VOID DS6_AnimUnitClose( ds6UNIT_BALL *Uni, ds6ANIM *Ani )
{
} /* End of 'DS6_AnimUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - self-pointer - animation object itself:
 *       ds6UNIT_BALL *Uni;
 *   - animation context pointer:
 *       ds6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID DS6_AnimUnitResponse( ds6UNIT_BALL *Uni, ds6ANIM *Ani )
{
} /* End of 'DS6_AnimUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ds6UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       ds6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID DS6_AnimUnitRender( ds6UNIT_BALL *Uni, ds6ANIM *Ani )
{
  DBL shift = Uni->Amplitude * fabs(sin(Uni->ScaleShift * (DBL)clock() / CLOCKS_PER_SEC + Uni->PhaseShift));
  
  SetDCBrushColor(Ani->hDC, Uni->Color);  
  DS6_RndGObjDraw(Ani->hDC, Uni->Pos.X + shift, Uni->Pos.Y + shift);  
  
} /* End of 'DS6_AnimUnitRender' function */

/* Функция создания объекта анимации "мяч".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ds6UNIT *) указатель на созданный объект анимации.
 */
ds6UNIT * DS6_UnitBallCreate( VOID )
{
  ds6UNIT_BALL *Uni;            

  if ((Uni = (VOID *)DS6_AnimUnitCreate(sizeof(ds6UNIT_BALL))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)DS6_AnimUnitInit;
  Uni->Close = (VOID *)DS6_AnimUnitClose;
  Uni->Response = (VOID *)DS6_AnimUnitResponse;
  Uni->Render = (VOID *)DS6_AnimUnitRender;
  return (ds6UNIT *)Uni;
} /* End of 'DS6_UnitBallCreate' function */

/* END OF 'UBALL.C' FILE */
