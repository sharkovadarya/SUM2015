/* FILENAME: UCOW.C
 * PROGRAMMER: DS6
 * PURPOSE: Cow unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>
#include <time.h>

#include "anim.h"
#include "vec.h"

/* Cow image type */
typedef struct tagds6UNIT_COW
{
  DS6_UNIT_BASE_FIELDS;

  VEC Pos;     /* Cow position */
  DWORD Color; /* Cow color */
  DBL
    Amplitude,  /* Amplitude */
    PhaseShift, /* Phase angle */
    ScaleShift; /* Time scale */
} ds6UNIT_COW;

/* Animation object initialization function.
 * ARGUMENTS:
 *   - self-pointer - animation object itself:
 *       ds6UNIT_COW *Uni;
 *   - animation context pointer:
 *       ds6ANIM *Ani;
 * RETURNS: none.
 */
static VOID DS6_AnimUnitInit( ds6UNIT_COW *Uni, ds6ANIM *Ani )
{
  Uni->Pos = VecSet(rand() % 1000, rand() % 700, 0);
  Uni->Color = RGB(rand() % 256, rand() % 256, rand() % 256);
  Uni->PhaseShift = rand() % 3000;
  Uni->ScaleShift = 5 + 0.30 * rand() / RAND_MAX;
  Uni->Amplitude = 30 + 59.0 * rand() / RAND_MAX;
} /* End of 'DS6_AnimUnitInit' function */

/* Animation object deinitialization function.
 * ARGUMENTS:
 *   - указатель на "себя" - сам объект анимации:
 *       ds6UNIT_COW *Uni;
 *   - animation context pointer:
 *       ds6ANIM *Ani;
 * RETURNS: none.
 */
static VOID DS6_AnimUnitClose( ds6UNIT_COW *Uni, ds6ANIM *Ani )
{
} /* End of 'DS6_AnimUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - self-pointer - animation object itself:
 *       ds6UNIT_COW *Uni;
 *   - animation context pointer:
 *       ds6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID DS6_AnimUnitResponse( ds6UNIT_COW *Uni, ds6ANIM *Ani )
{
  if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
    DS6_AnimDoExit();
  if (GetAsyncKeyState('F') & 0x8000)
    DS6_AnimFlipFullScreen();
} /* End of 'DS6_AnimUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ds6UNIT_COW *Uni;
 *   - указатель на контекст анимации:
 *       ds6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID DS6_AnimUnitRender( ds6UNIT_COW *Uni, ds6ANIM *Ani )
{
  DBL shift = Uni->Amplitude * fabs(sin(Uni->ScaleShift * (DBL)clock() / CLOCKS_PER_SEC + Uni->PhaseShift));

  SetDCBrushColor(Ani->hDC, Uni->Color);
  Ellipse(Ani->hDC, Uni->Pos.X - 5, Uni->Pos.Y - 5 - shift, Uni->Pos.X + 5, Uni->Pos.Y + 5 - shift);
} /* End of 'DS6_AnimUnitRender' function */

/* Функция создания объекта анимации "мяч".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ds6UNIT *) указатель на созданный объект анимации.
 */
ds6UNIT * DS6_UnitBallCreate( VOID )
{
  ds6UNIT_COW *Uni;

  if ((Uni = (VOID *)DS6_AnimUnitCreate(sizeof(ds6UNIT_COW))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)DS6_AnimUnitInit;
  Uni->Close = (VOID *)DS6_AnimUnitClose;
  Uni->Response = (VOID *)DS6_AnimUnitResponse;
  Uni->Render = (VOID *)DS6_AnimUnitRender;
  return (ds6UNIT *)Uni;
} /* End of 'DS6_UnitBallCreate' function */

/* END OF 'UCOW.C' FILE */
