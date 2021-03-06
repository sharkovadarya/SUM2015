/* FILENAME: ANIM.C
 * PROGRAMMER: DS6
 * PURPOSE: Animation system module.
 * LAST UPDATE: 10.06.2015
 */
#include "anim.h" 
#include <stdio.h>  
#include <mmsystem.h>
#pragma comment(lib, "winmm")

/* ��������� �������� ��� ��������� */
#define DS6_GET_AXIS_VALUE(Axis) \
  (2.0 * (ji.dw ## Axis ## pos - jc.w ## Axis ## min) / (jc.w ## Axis ## max - jc.w ## Axis ## min) - 1.0)


/* ����������� ���������� ���� */
static INT
  DS6_MouseOldX, DS6_MouseOldY;

/* ��������� �������� �������� */
ds6ANIM DS6_Anim;

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
  POINT pt;

 

  memset(&DS6_Anim, 0, sizeof(ds6ANIM));
  DS6_Anim.hWnd = hWnd;
  /* �������������� ����� ����� */
  DS6_Anim.hDC = CreateCompatibleDC(hDC);
  DS6_Anim.hBmFrame = CreateCompatibleBitmap(hDC, 30, 30);
  SelectObject(DS6_Anim.hDC, DS6_Anim.hBmFrame);
  DS6_Anim.W = 30;
  DS6_Anim.H = 30;             
  DS6_Anim.NumOfUnits = 0;
  ReleaseDC(hWnd, hDC);

   /* ������������� ������� */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;
  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  DS6_Anim.IsPause = FALSE;
  FrameCounter = 0;

   /* ������������� ����� */
  GetCursorPos(&pt);
  ScreenToClient(DS6_Anim.hWnd, &pt);
  DS6_MouseOldX = pt.x;
  DS6_MouseOldY = pt.y;
  GetKeyboardState(DS6_Anim.KeysOld);  

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

  /* projection parametres correction */
  if (W > H)
    DS6_RndWp = (DBL)W / H * 3, DS6_RndHp = 3;
  else
    DS6_RndHp = (DBL)H / W * 3, DS6_RndWp = 3;

   DS6_RndMatrProj = MatrFrustum( - DS6_RndWp / 2, DS6_RndWp / 2, - DS6_RndHp / 2, DS6_RndHp / 2, DS6_RndProjDist, 800);

} /* End of 'DS6_AnimResize' function */

/* ������� ���������� ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID DS6_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;
  POINT pt;

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

    DS6_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  /* ����� "��������" ����� */
  TimeOld = li.QuadPart; 

  /* ������� ���� */
  SelectObject(DS6_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(DS6_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(DS6_Anim.hDC, RGB(0, 0, 0));
  Rectangle(DS6_Anim.hDC, 0, 0, DS6_Anim.W + 1, DS6_Anim.H + 1);

  /* input update */
  /* keyboard */
  GetKeyboardState(DS6_Anim.Keys);
  for (i = 0; i < 256; i++)
    DS6_Anim.Keys[i] >>= 7;
  for (i = 0; i < 256; i++)
    DS6_Anim.KeysClick[i] = DS6_Anim.Keys[i] && !DS6_Anim.KeysOld[i];
  memcpy(DS6_Anim.KeysOld, DS6_Anim.Keys, sizeof(DS6_Anim.KeysOld));

  /* ���� */
  /* ������ */
  DS6_Anim.MsWheel = DS6_MouseWheel;
  DS6_MouseWheel = 0;
  /* ���������� ������� */
  GetCursorPos(&pt);
  ScreenToClient(DS6_Anim.hWnd, &pt);
  DS6_Anim.MsX = pt.x;
  DS6_Anim.MsY = pt.y;
  /* ������������� ����������� */
  DS6_Anim.MsDeltaX = pt.x - DS6_MouseOldX;
  DS6_Anim.MsDeltaY = pt.y - DS6_MouseOldY;
  DS6_MouseOldX = pt.x;
  DS6_MouseOldY = pt.y;

  /* �������� */
  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS jc;

    /* ��������� ����� ���������� � �������� */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      /* ��������� �������� ��������� */
      ji.dwSize = sizeof(JOYCAPS);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* ������ */
        memcpy(DS6_Anim.JButsOld, DS6_Anim.JButs, sizeof(DS6_Anim.JButs));
        for (i = 0; i < 32; i++)
          DS6_Anim.JButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          DS6_Anim.JButsClick[i] = DS6_Anim.JButs[i] && !DS6_Anim.JButsOld[i];

        /* ��� */
        DS6_Anim.JX = DS6_GET_AXIS_VALUE(X);
        DS6_Anim.JY = DS6_GET_AXIS_VALUE(Y);
        if (jc.wCaps & JOYCAPS_HASZ)
          DS6_Anim.JZ = DS6_GET_AXIS_VALUE(Z);
        if (jc.wCaps & JOYCAPS_HASU)
          DS6_Anim.JU = DS6_GET_AXIS_VALUE(U);
        if (jc.wCaps & JOYCAPS_HASV)
          DS6_Anim.JV = DS6_GET_AXIS_VALUE(V);
        if (jc.wCaps & JOYCAPS_HASR)
          DS6_Anim.JR = DS6_GET_AXIS_VALUE(R);

        if (jc.wCaps & JOYCAPS_HASPOV)
        {
          if (ji.dwPOV == 0xFFFF)
            DS6_Anim.JPOV = 0;
          else
            DS6_Anim.JPOV = ji.dwPOV / 4500 + 1;
        }
      }
    }
  }

  
  /* ����� �� ��������� ��������� �������� */
  for (i = 0; i < DS6_Anim.NumOfUnits; i++)
    DS6_Anim.Units[i]->Response(DS6_Anim.Units[i], &DS6_Anim);

  /* ��������� �������� */
  
  for (i = 0; i < DS6_Anim.NumOfUnits; i++)
  {     
    SelectObject(DS6_Anim.hDC, GetStockObject(WHITE_PEN));   

    DS6_RndMatrWorld = MatrIdentity();
    DS6_Anim.Units[i]->Render(DS6_Anim.Units[i], &DS6_Anim);
  }                
  FrameCounter++;
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

VOID DS6_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* ������� ����� */
  static RECT SaveRC;               /* ����������� ������ */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* ��������� ������ ������ ���� */
    GetWindowRect(DS6_Anim.hWnd, &SaveRC);

    /* ���������� � ����� �������� ��������� ���� */
    hmon = MonitorFromWindow(DS6_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* �������� ���������� ��� �������� */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* ��������� � ������ ����� */
    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(DS6_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(DS6_Anim.hWnd, HWND_TOPMOST,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    /* ��������������� ������ ���� */
    SetWindowPos(DS6_Anim.hWnd, HWND_TOP,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'DS6_AnimFlipFullScreen' function */

/* ������� ������ �� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID DS6_AnimDoExit( VOID )
{
  PostMessage(DS6_Anim.hWnd, WM_CLOSE, 0, 0);
} /* End of 'DS6_AnimDoExit' function */

/* END OF 'ANIM.C' FILE */
