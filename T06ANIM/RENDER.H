/* FILE: RENDER.H
 * PROGRAMMER: DS6
 * DATE: 09.06.2015
 * PURPOSE: include stuff that renders stuff.
*/

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

/* ��� ������������� ������� - ����� ������ � ������������� */
typedef struct tagds6GOBJ
{
  VEC *V;      /* ������ ������ ������� */
  INT NumOfV;  /* ���������� ������ */
  INT (*F)[3]; /* ������ ������ ������� */
  INT NumOfF;  /* ���������� ������ */
} ds6GOBJ;

/* ������� */
extern MATR
  DS6_RndMatrWorld,
  DS6_RndMatrProj,
  DS6_RndMatrView;

/* ��������� ������������� */
extern DBL
  DS6_RndWp, DS6_RndHp,    /* ������� ������� ������������� */
  DS6_RndProjDist;         /* ���������� �� ��������� �������� */

/* ������� �������������� �� ������� ������� ��������� � ����.
 * ���������:
 *   - �������� �����:
 *       VEC P;
 * ������������ ��������:
 *   (POINT) ���������� � �����.
 */
POINT DS6_RndWorldToScreen( VEC P );

/* ������� �������� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       ds6GOBJ *GObj;
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, FALSE �����.
 */
BOOL DS6_RndGObjLoad( ds6GOBJ *GObj, CHAR *FileName );

/* ������� ������������ ������ ��-��� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       ds6GOBJ *GObj;
 * ������������ ��������: ���.
 */
VOID DS6_RndGObjFree( ds6GOBJ *GObj );

/* ������� ��������� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       ds6GOBJ *GObj;
 * ������������ ��������: ���.
 */
VOID DS6_RndGObjDraw( ds6GOBJ *GObj );

#endif /* __RENDER_H_ */    

/* END OF 'RENDER.H' FILE */