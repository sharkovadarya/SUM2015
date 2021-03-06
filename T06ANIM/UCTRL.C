/* FILENAME: UCONTRL.C
 * PROGRAMMER: DS6
 * PURPOSE: Control unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include <stdio.h>  

#include "anim.h" 

/* ��� ��������� �������� */
typedef struct tagds6UNIT_CTRL
{
  DS6_UNIT_BASE_FIELDS;
  HFONT hFnt; /* ����� ��� ������ FPS */
} ds6UNIT_CTRL;

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ds6UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       ds6ANIM *Ani;
 * ������������ ��������: ���.
 */

INT DS6_GamePadShift = 0;
static VOID DS6_AnimUnitInit( ds6UNIT_CTRL *Uni, ds6ANIM *Ani )
{
  Uni->hFnt = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE,
    FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
    VARIABLE_PITCH, "Bookman Old Style");
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
  DeleteObject(Uni->hFnt);
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
  if (Ani->JButsClick[3])
    DS6_GamePadShift++;
  if (Ani->JButsClick[2])
    DS6_GamePadShift--;  
  
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
  HFONT hFntOld = SelectObject(Ani->hDC, Uni->hFnt);
  RECT rc;
  static CHAR Buf[1000];

  SetTextColor(Ani->hDC, RGB(22, 228, 8));
  SetBkMode(Ani->hDC, TRANSPARENT);

  rc.left = 0;       
  rc.top = 0;
  rc.right = Ani->W;
  rc.bottom = Ani->H;
  DrawText(Ani->hDC, Buf,
    sprintf(Buf,
      "FPS: %.3f\n"
      "J: %i %i %i, POV:%i\n"
      "%.5f %.5f %.5f %.5f %.5f %.5f",
      Ani->FPS,
      Ani->JButs[0], Ani->JButs[1], Ani->JButs[2], Ani->JPOV,
      Ani->JX, Ani->JY, Ani->JZ, Ani->JR, Ani->JU, Ani->JV),
    &rc,
    DT_TOP | DT_LEFT);

  SelectObject(Ani->hDC, hFntOld);
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
