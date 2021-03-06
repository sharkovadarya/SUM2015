/* FILENAME: UNIT.C
 * PROGRAMMER: DS6
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>

#include "anim.h"

/* �������  ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ds6UNIT *Uni;
 *   - ��������� �� �������� ��������:
 *       ds6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID DS6_AnimUnitInit( ds6UNIT *Uni, ds6ANIM *Ani )
{
} /* End of 'DS6_AnimUnitInit' function */

/* �������  ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ds6UNIT *Uni;
 *   - ��������� �� �������� ��������:
 *       ds6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID DS6_AnimUnitClose( ds6UNIT *Uni, ds6ANIM *Ani )
{
} /* End of 'DS6_AnimUnitClose' function */

/* �������  ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ds6UNIT *Uni;
 *   - ��������� �� �������� ��������:
 *       ds6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID DS6_AnimUnitResponse( ds6UNIT *Uni, ds6ANIM *Ani )
{
} /* End of 'DS6_AnimUnitResponse' function */

/* �������  ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ds6UNIT *Uni;
 *   - ��������� �� �������� ��������:
 *       ds6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID DS6_AnimUnitRender( ds6UNIT *Uni, ds6ANIM *Ani )
{
} /* End of 'DS6_AnimUnitRender' function */

/* ������� �������� ������� ��������.
 * ���������:
 *   - ������ ��������� ������� ��������:
 *       INT Size;
 * ������������ ��������:
 *   (ds6UNIT *) ��������� �� ��������� ������ ��������.
 */
ds6UNIT * DS6_AnimUnitCreate( INT Size )
{
  ds6UNIT *Uni;

  if (Size < sizeof(ds6UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* ��������� ����  */
  Uni->Size = Size;
  Uni->Init = DS6_AnimUnitInit;
  Uni->Close = DS6_AnimUnitClose;
  Uni->Response = DS6_AnimUnitResponse;
  Uni->Render = DS6_AnimUnitRender;
  return Uni;
} /* End of 'DS6_AnimUnitCreate' function */

/* END OF 'UNIT.C' FILE */
