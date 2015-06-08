/* FILENAME: ANIM.C
 * PROGRAMMER: DS6
 * PURPOSE: Animation system module.
 * LAST UPDATE: 08.06.2015
 */

#include <stdio.h>
#include "anim.h"


/* ��������� �������� �������� */
static ds6ANIM DS6_Anim;
static INT64
  TimeFreq,  /* ������ ��������� � ������� */
  TimeStart, /* ����� ������ �������� */
  TimeOld,   /* ����� �������� ����� */
  TimePause, /* ����� ������� � ����� */
  TimeFPS;   /* ����� ��� ������ FPS */
static INT
  FrameCounter; /* ������� ������ */


/* ������� ��������� ����� ��������.
 * ���������:
 *   - ���� �����:
 *       BOOL NewPauseFlag;
 * ������������ ��������: ���.
 */
VOID DS6_AnimSetPause( BOOL NewPauseFlag )
{
  DS6_Anim.IsPause = NewPauseFlag;
} /* End of 'AG6_AnimSetPause' function */



/* ������� ������������� ��������.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 * ������������ ��������: ���.
 */
VOID DS6_AnimInit( HWND hWnd )
{
  HDC hDC = GetDC(hWnd);
  LARGE_INTEGER li;

  /* ������������� ������� */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;
  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  DS6_Anim.IsPause = FALSE;
  FrameCounter = 0;

  DS6_Anim.hWnd = hWnd;
  /* �������������� ����� ����� */
  DS6_Anim.hDC = CreateCompatibleDC(hDC);
  DS6_Anim.hBmFrame = CreateCompatibleBitmap(hDC, 300, 300);
  SelectObject(DS6_Anim.hDC, DS6_Anim.hBmFrame);
  DS6_Anim.W = 1000;
  DS6_Anim.H = 1000;
  DS6_Anim.NumOfUnits = 0;
  ReleaseDC(hWnd, hDC);

  /* ������������� ������� */
  QueryPerformanceFrequency(&li);
  QueryPerformanceCounter(&li);
} /* End of 'DS6_AnimInit' function */

/* ������� ��������������� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID DS6_AnimClose( VOID )
{
  INT i;

  /* ������������ ������ ��-��� �������� �������� */
  for (i = 0; i < DS6_Anim.NumOfUnits; i++)
  {
    DS6_Anim.Units[i]->Close(DS6_Anim.Units[i], &DS6_Anim);
    free(DS6_Anim.Units[i]);
    DS6_Anim.Units[i] = NULL;
  }

  /* ������� ������� GDI */
  DeleteObject(DS6_Anim.hBmFrame);
  DeleteDC(DS6_Anim.hDC);
} /* End of 'DS6_AnimClose' function */

/* ������� ��������� ��������� ������� ������� ������.
 * ���������:
 *   - ����� ������ ������� ������:
 *       INT W, H;
 * ������������ ��������: ���.
 */
VOID DS6_AnimResize( INT W, INT H )
{
  HDC hDC = GetDC(DS6_Anim.hWnd);

  /* ������������� ������ � ������ ����� */
  DeleteObject(DS6_Anim.hBmFrame);
  DS6_Anim.hBmFrame = CreateCompatibleBitmap(hDC, W, H);
  SelectObject(DS6_Anim.hDC, DS6_Anim.hBmFrame);

  /* ���������� ������� */
  DS6_Anim.W = W;
  DS6_Anim.H = H;

  ReleaseDC(DS6_Anim.hWnd, hDC);
} /* End of 'DS6_AnimResize' function */

/* ������� ���������� ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID DS6_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;

  QueryPerformanceCounter(&li);
  DS6_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  DS6_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  if (!DS6_Anim.IsPause)
    DS6_Anim.DeltaTime = DS6_Anim.GlobalDeltaTime;
  else
  {
    TimePause += li.QuadPart - TimeOld;
    DS6_Anim.DeltaTime = 0;
  }

  DS6_Anim.Time = (DBL)(li.QuadPart - TimePause - TimeStart) / TimeFreq;

  /* ��������� FPS */
  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    static CHAR Buf[100];

    sprintf(Buf, "FPS: %.5f", DS6_Anim.FPS);
    SetWindowText(DS6_Anim.hWnd, Buf);

    DS6_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  /* ����� "��������" ����� */
  TimeOld = li.QuadPart;

  FrameCounter++;

  /* ����� �� ��������� ��������� �������� */
  for (i = 0; i < DS6_Anim.NumOfUnits; i++)
    DS6_Anim.Units[i]->Response(DS6_Anim.Units[i], &DS6_Anim);

  /* ������� ���� */
  SelectObject(DS6_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(DS6_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(DS6_Anim.hDC, RGB(0, 0, 0));
  Rectangle(DS6_Anim.hDC, 0, 0, DS6_Anim.W + 1, DS6_Anim.H + 1);

  /* ��������� �������� */
  for (i = 0; i < DS6_Anim.NumOfUnits; i++)
  {
    SelectObject(DS6_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(DS6_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(DS6_Anim.hDC, RGB(255, 255, 255));
    SetDCPenColor(DS6_Anim.hDC, RGB(0, 0, 0));

    DS6_Anim.Units[i]->Render(DS6_Anim.Units[i], &DS6_Anim);
  }
} /* End of 'DS6_AnimRender' function */

/* ������� ������ ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID DS6_AnimCopyFrame( VOID )
{
  HDC hDC;

  /* ����� ����� */
  hDC = GetDC(DS6_Anim.hWnd);
  BitBlt(hDC, 0, 0, DS6_Anim.W, DS6_Anim.H, DS6_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(DS6_Anim.hWnd, hDC);
} /* End of 'DS6_AnimCopyFrame' function */

/* ������� ���������� � ������� ������� ��������.
 * ���������:
 *   - ����������� ������ ��������:
 *       ds6UNIT *Unit;
 * ������������ ��������: ���.
 */
VOID DS6_AnimAddUnit( ds6UNIT *Unit )
{
  if (DS6_Anim.NumOfUnits < DS6_MAX_UNITS)
  {
    DS6_Anim.Units[DS6_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &DS6_Anim);
  }
} /* End of 'DS6_AnimAddUnit' function */


/* ������� ������������ �/�� �������������� ������
 * � ������ ���������� ���������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID DS6_AnimFlipFullScreen( HWND hWnd ) 
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRC;

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    GetWindowRect(hWnd, &SaveRC);
    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);
    
    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);

    SetWindowPos(hWnd, HWND_TOP, rc.left, rc.top
      , rc.right - rc.left, rc.bottom - rc.top + 201, SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    SetWindowPos(hWnd, HWND_TOPMOST, SaveRC.left, SaveRC.top, SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top, SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }


}   /* End of 'DS6_AnimFlipFullScreen' function */

/* ������� ������ �� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID DS6_AnimDoExit( VOID )
{
  PostMessage(DS6_Anim.hWnd, WM_CLOSE, 0, 0);
} /* End of 'DS6_AnimDoExit' function */

/* END OF 'ANIM.C' FILE */