/* FILENAME: UBALL.C
 * PROGRAMMER: DS6
 * PURPOSE: Bounce ball unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>
#include <time.h>

#include "anim.h"
#include "vec.h"

/* ��� ������������� ���� */
typedef struct tagds6UNIT_BALL
{
  DS6_UNIT_BASE_FIELDS;

  VEC Pos;     /* ������� ���� */
  DWORD Color; /* ���� ���� */
  DBL
    Amplitude,  /* ��������� */
    PhaseShift, /* ����� �� ���� */
    ScaleShift; /* ������� ������� */
} ds6UNIT_BALL;

/* ��� ������������� ������� ���� */
typedef struct tagds6UNIT_BALL2
{
  DS6_UNIT_BASE_FIELDS;

  VEC Pos;     /* ������� ���� */
  DWORD Color; /* ���� ���� */
  DBL
    ScaleShift, /* ������� ������� */
    Radius;     /* Circle that the ball moves around radius */
} ds6UNIT_BALL2;

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ds6UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       ds6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID DS6_AnimUnitInit( ds6UNIT_BALL *Uni, ds6ANIM *Ani )
{
  Uni->Pos = VecSet(rand() % 1000, rand() % 700, 0);
  Uni->Color = RGB(rand() % 256, rand() % 256, rand() % 256);
  Uni->PhaseShift = rand() % 3000;
  Uni->ScaleShift = 5 + 0.30 * rand() / RAND_MAX;
  Uni->Amplitude = 30 + 59.0 * rand() / RAND_MAX;
} /* End of 'DS6_AnimUnitInit' function */

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ds6UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       ds6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID DS6_AnimUnitInit2( ds6UNIT_BALL2 *Uni, ds6ANIM *Ani )
{
  Uni->Pos = VecSet(rand() % 1000, rand() % 700, 0);
  Uni->Color = RGB(rand() % 256, rand() % 256, rand() % 256);  
  Uni->ScaleShift = 5 + 0.30 * rand() / RAND_MAX;  
  Uni->Radius = 228 +2.28 * rand() / 2.28;
} /* End of 'DS6_AnimUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ds6UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       ds6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID DS6_AnimUnitClose( ds6UNIT_BALL *Uni, ds6ANIM *Ani )
{
} /* End of 'DS6_AnimUnitClose' function */

/* ������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ds6UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       ds6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID DS6_AnimUnitResponse( ds6UNIT_BALL *Uni, ds6ANIM *Ani )
{
  if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
    DS6_AnimDoExit();
  if (GetAsyncKeyState('F') & 0x8000)
    DS6_AnimFlipFullScreen();
} /* End of 'DS6_AnimUnitResponse' function */
/* ������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ds6UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       ds6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID DS6_AnimUnitResponse2( ds6UNIT_BALL2 *Uni, ds6ANIM *Ani )
{
  if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
    DS6_AnimDoExit();
  if (GetAsyncKeyState('F') & 0x8000)
    DS6_AnimFlipFullScreen();
} /* End of 'DS6_AnimUnitResponse' function */

/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ds6UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       ds6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID DS6_AnimUnitRender( ds6UNIT_BALL *Uni, ds6ANIM *Ani )
{
  DBL shift = Uni->Amplitude * fabs(sin(Uni->ScaleShift * (DBL)clock() / CLOCKS_PER_SEC + Uni->PhaseShift));   

  SetDCBrushColor(Ani->hDC, Uni->Color);
  Ellipse(Ani->hDC, Uni->Pos.X - 5, Uni->Pos.Y - 5 - shift, Uni->Pos.X + 5, Uni->Pos.Y + 5 - shift);
} /* End of 'DS6_AnimUnitRender' function */

static VOID DS6_AnimUnitRender2( ds6UNIT_BALL2 *Uni, ds6ANIM *Ani, INT X1, INT Y1 )
{ 
  DBL co = X1 / Uni->Radius, si = Y1 / Uni->Radius;  
  DBL x = co * Uni->Radius, y = si * Uni->Radius;    

  SetDCBrushColor(Ani->hDC, Uni->Color);
  Ellipse(Ani->hDC, Uni->Pos.X - 5, Uni->Pos.Y  + x - 5, Uni->Pos.X + 5, y + Uni->Pos.Y + 5);

} /* End of 'DS6_AnimUnitRender' function */

/* ������� �������� ������� �������� "���".
 * ���������: ���.
 * ������������ ��������:
 *   (ds6UNIT *) ��������� �� ��������� ������ ��������.
 */
ds6UNIT * DS6_UnitBallCreate( VOID )
{
  ds6UNIT_BALL *Uni;

  if ((Uni = (VOID *)DS6_AnimUnitCreate(sizeof(ds6UNIT_BALL))) == NULL)
    return NULL;
  /* ��������� ���� */
  Uni->Init = (VOID *)DS6_AnimUnitInit;
  Uni->Close = (VOID *)DS6_AnimUnitClose;
  Uni->Response = (VOID *)DS6_AnimUnitResponse;
  Uni->Render = (VOID *)DS6_AnimUnitRender;
  return (ds6UNIT *)Uni;
} /* End of 'DS6_UnitBallCreate' function */ 

/* END OF 'UBALL.C' FILE */
