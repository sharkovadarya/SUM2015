/* FILENAME: ANIM.H
 * PROGRAMMER: DS6
 * PURPOSE: Animation system declaration module.
 * LAST UPDATE: 08.06.2015
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "render.h"

/* ������������ ���������� ��������� �������� */
#define DS6_MAX_UNITS 3000

#define L 4

/* ���������� ���� �������� �������� "������" */
typedef struct tagds6UNIT ds6UNIT;

/* ��������� �������� ��������� �������� */
typedef struct
{
  HWND hWnd;          /* ���� ������ */
  INT W, H;           /* ������ ���� ������ */
  HDC hDC;            /* �������� ������ */
  HGLRC hGLRC;        /* �������� ���������� */

  /* ������ ��������� �������� � �� ���������� */
  ds6UNIT *Units[DS6_MAX_UNITS];
  INT NumOfUnits;

  BOOL IsPause;       /* �������� �� ����� */
  DBL GlobalTime, 
      GlobalDeltaTime, 
      DeltaTime, 
      Time, 
      FPS;
  /* ���������� ����� */
  BYTE
    Keys[256],       /* ��������� ������ ���������� � ���� */
    KeysOld[256],    /* ��������� ������ �� ���������� ����� */
    KeysClick[256];  /* ����� ������������ ������� */
  INT
    MsDeltaX, MsDeltaY, /* ������������� ���������� ������� ���� */
    MsX, MsY,        /* ���������� ���������� ������� ���� */
    MsWheel;         /* ��������� ������ ���� */
  BYTE
    JButs[32],       /* ��������� ������ ��������� */
    JButsOld[32],    /* ��������� ������ ��������� �� ���������� ����� */
    JButsClick[32],  /* ����� ������������ ������� ������ ��������� */
    JPOV;            /* ������������� POV - 0..8 */
  DBL
    JX, JY, JZ, JR, JU, JV; /* ��� ��������� (-1.0 .. 1.0*/


} ds6ANIM;

extern ds6ANIM DS6_Anim;
extern INT DS6_GamePadShiftZ;
extern INT DS6_GamePadShiftY;
extern INT DS6_GamePadShiftX;

/* ������� ���� ������� ��������:
 * - ������ ��������� ��� ������������
 *     INT Size;
 * - ������������� ��������:
 *     INT Id;
 * - ��������� �� ������� �������������
 *     VOID (*Init)( ds6UNIT *Unit, ds6ANIM *Ani );
 * - ��������� �� ������� ���������������
 *     VOID (*Close)( ds6UNIT *Unit, ds6ANIM *Ani );
 * - ��������� �� ������� ���������� ����������� ����������
 *     VOID (*Response)( ds6UNIT *Unit, ds6ANIM *Ani );
 * - ��������� �� ������� ����������
 *     VOID (*Render)( ds6UNIT *Unit, ds6ANIM *Ani );
 */
#define DS6_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( ds6UNIT *Unit, ds6ANIM *Ani );       \
  VOID (*Close)( ds6UNIT *Unit, ds6ANIM *Ani );      \
  VOID (*Response)( ds6UNIT *Unit, ds6ANIM *Ani );   \
  VOID (*Render)( ds6UNIT *Unit, ds6ANIM *Ani )

/* ������� ��� ������� �������� */
struct tagds6UNIT
{
  DS6_UNIT_BASE_FIELDS;
};

/* ������� ������������� ��������.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 * ������������ ��������: ���.
 */
BOOL DS6_AnimInit( HWND hWnd );

/* ������� ��������������� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID DS6_AnimClose( VOID );

/* ������� ��������� ��������� ������� ������� ������.
 * ���������:
 *   - ����� ������ ������� ������:
 *       INT W, H;
 * ������������ ��������: ���.
 */
VOID DS6_AnimResize( INT W, INT H );

/* ������� ���������� ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID DS6_AnimRender( VOID );

/* ������� ������ ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID DS6_AnimCopyFrame( VOID );

/* ������� ���������� � ������� ������� ��������.
 * ���������:
 *   - ����������� ������ ��������:
 *       ds6UNIT *Unit;
 * ������������ ��������: ���.
 */
VOID DS6_AnimAddUnit( ds6UNIT *Unit );

/* ������� ������������ �/�� �������������� ������
 * � ������ ���������� ���������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID DS6_AnimFlipFullScreen( VOID );

/* ������� �������� ������� ��������.
 * ���������:
 *   - ������ ��������� ������� ��������:
 *       INT Size;
 * ������������ ��������:
 *   (ds6UNIT *) ��������� �� ��������� ������ ��������.
 */
ds6UNIT * DS6_AnimUnitCreate( INT Size );

/* ������� ������ �� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID DS6_AnimDoExit( VOID );

/* Set pause function.
 * ARGUMENTS: 
 *   - pause flag:
 *       BOOL NewPAuseFlag;
 * RETURNS: none.
 */  
VOID DS6_AnimSetPause( BOOL NewPauseFlag );

extern INT DS6_MouseWheel;


#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */