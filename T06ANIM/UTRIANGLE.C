/* FILENAME: UTRIANGLE.C
 * PROGRAMMER: DS6
 * PURPOSE: Triangle unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>
#include <time.h>

#include "anim.h"
#include "vec.h"

/* Тип представления мяча */
typedef struct tagds6UNIT_TRIANGLE
{
  DS6_UNIT_BASE_FIELDS;

  VEC Pos;     /* Позиция мяча */
  DWORD Color; /* Цвет мяча */
  DBL
    Amplitude,  /* Амплитуда */
    PhaseShift, /* Сдвиг по фазе */
    ScaleShift; /* Масштаб времени */
} ds6UNIT_TRIANGLE;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ds6UNIT_TRIANGLE *Uni;
 *   - указатель на контекст анимации:
 *       ds6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID DS6_AnimUnitInit( ds6UNIT_TRIANGLE *Uni, ds6ANIM *Ani )
{
  Uni->Pos = VecSet(rand() % 1000, rand() % 228, 228);
  Uni->Color = RGB(rand() % 228, rand() % 228, rand() % 228);
  Uni->PhaseShift = rand() % 22;
  Uni->ScaleShift = 22.8 + 0.228 * rand() / RAND_MAX;
  Uni->Amplitude = 8 + 22.8 * rand() / RAND_MAX;
} /* End of 'DS6_AnimUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ds6UNIT_TRIANGLE *Uni;
 *   - указатель на контекст анимации:
 *       ds6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID DS6_AnimUnitClose( ds6UNIT_TRIANGLE *Uni, ds6ANIM *Ani )
{
} /* End of 'DS6_AnimUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ds6UNIT_TRIANGLE *Uni;
 *   - указатель на контекст анимации:
 *       ds6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID DS6_AnimUnitResponse( ds6UNIT_TRIANGLE *Uni, ds6ANIM *Ani )
{
  if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
    DS6_AnimDoExit();
  if (GetAsyncKeyState('F') & 0x8000)
    DS6_AnimFlipFullScreen();
} /* End of 'DS6_AnimUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       ds6UNIT_TRIANGLE *Uni;
 *   - указатель на контекст анимации:
 *       ds6ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID DS6_AnimUnitRender( ds6UNIT_TRIANGLE *Uni, ds6ANIM *Ani )
{
  DBL shift = Uni->Amplitude * fabs(sin(Uni->ScaleShift * (DBL)clock() / CLOCKS_PER_SEC + Uni->PhaseShift));
  INT i;
  POINT pnts[] = 
  {
    {rand() % 228, rand() % 228}, {rand() % 228, rand() % 228}, {rand() % 228, rand() % 228}, {rand() % 228, rand() % 228}
  }, pntdraw[sizeof pnts / sizeof pnts[0]];

  for (i = 0; i < sizeof pnts / sizeof pnts[0]; i++)
  {
    pntdraw[i].x = rand() % 22800 * rand() % 2280 + pnts[i].x * rand() % 220;
    pntdraw[i].y = rand() % 2280 - pnts[i].y * rand() % 80;
  } 
  SetDCBrushColor(Ani->hDC, Uni->Color);
  Polygon(Ani->hDC, pntdraw, 3);  
 
} /* End of 'DS6_AnimUnitRender' function */

/* Функция создания объекта анимации "мяч".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (ds6UNIT *) указатель на созданный объект анимации.
 */
ds6UNIT * DS6_UnitTriangleCreate( VOID )
{
  ds6UNIT_TRIANGLE *Uni;

  if ((Uni = (VOID *)DS6_AnimUnitCreate(sizeof(ds6UNIT_TRIANGLE))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)DS6_AnimUnitInit;
  Uni->Close = (VOID *)DS6_AnimUnitClose;
  Uni->Response = (VOID *)DS6_AnimUnitResponse;
  Uni->Render = (VOID *)DS6_AnimUnitRender;
  return (ds6UNIT *)Uni;
} /* End of 'DS6_UnitTRIANGLECreate' function */

/* END OF 'UTRIANGLE.C' FILE */
