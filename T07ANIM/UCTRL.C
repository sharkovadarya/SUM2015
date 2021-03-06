/* FILENAME: UCONTRL.C
 * PROGRAMMER: DS6
 * PURPOSE: Control unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include <stdio.h>  

#include "anim.h" 
/* �������� �� ��� Z */
INT DS6_GamePadShiftZ = 0;
INT DS6_GamePadShiftY = 0;
INT DS6_GamePadShiftX = 0;


/* ��� ��������� �������� */
typedef struct tagds6UNIT_CTRL
{
  DS6_UNIT_BASE_FIELDS; 
} ds6UNIT_CTRL;

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ds6UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       ds6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID DS6_AnimUnitInit( ds6UNIT_CTRL *Uni, ds6ANIM *Ani )
{
  
} /* End of 'DS6_AnimUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ds6UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       ds6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID DS6_AnimUnitClose( ds6UNIT_CTRL *Uni, ds6ANIM *Ani )
{
 
} /* End of 'DS6_AnimUnitClose' function */

/* ������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ds6UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       ds6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID DS6_AnimUnitResponse( ds6UNIT_CTRL *Uni, ds6ANIM *Ani )
{
  if (Ani->Keys[VK_ESCAPE])
    DS6_AnimDoExit();
  if (Ani->KeysClick['F'])
    DS6_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    DS6_AnimSetPause(!Ani->IsPause);  
  
  if (Ani->JButsClick[7])
    DS6_AnimFlipFullScreen();
  if (Ani->JButsClick[1])
    DS6_AnimSetPause(!Ani->IsPause);
  if (Ani->JButs[11])
    DS6_AnimDoExit();
  if (Ani->JButs[10])
    DS6_GamePadShiftZ++;
  if (Ani->JButs[2])
    DS6_GamePadShiftZ--; 
  if (Ani->JButs[3])
    DS6_GamePadShiftY++;
  if (Ani->JButs[5])
    DS6_GamePadShiftY--; 
  if (Ani->JButs[8])
    DS6_GamePadShiftX++;
  if (Ani->JButs[9])
    DS6_GamePadShiftX--;  
 
 
} /* End of 'DS6_AnimUnitResponse' function */

/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ds6UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       ds6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID DS6_AnimUnitRender( ds6UNIT_CTRL *Uni, ds6ANIM *Ani )
{
  static DBL count = 30;
  static CHAR Buf[1000];

  count += Ani->GlobalDeltaTime;
  if (count > 1)
  {
    count = 0;
    sprintf(Buf, "FPS: %.3f", Ani->FPS);
    SetWindowText(Ani->hWnd, Buf);
  }

} /* End of 'DS6_AnimUnitRender' function */

/* ������� �������� ������� �������� "����������".
 * ���������: ���.
 * ������������ ��������:
 *   (ds6UNIT *) ��������� �� ��������� ������ ��������.
 */
ds6UNIT * DS6_UnitControlCreate( VOID )
{
  ds6UNIT_CTRL *Uni;

  if ((Uni = (VOID *)DS6_AnimUnitCreate(sizeof(ds6UNIT_CTRL))) == NULL)
    return NULL;
  /* ��������� ���� */
  Uni->Init = (VOID *)DS6_AnimUnitInit;
  Uni->Close = (VOID *)DS6_AnimUnitClose;
  Uni->Response = (VOID *)DS6_AnimUnitResponse;
  Uni->Render = (VOID *)DS6_AnimUnitRender;
  return (ds6UNIT *)Uni;
} /* End of 'DS6_UnitBallCreate' function */

/* END OF 'UCONTRL.C' FILE */
