/* FILENAME: ANIM.C
 * PROGRAMMER: DS6
 * PURPOSE: Animation system module.
 * LAST UPDATE: 10.06.2015
 */
#include "anim.h"
#include <stdio.h>  
#include <mmsystem.h>
#pragma comment(lib, "winmm")

/* Получение значения оси джойстика */
#define DS6_GET_AXIS_VALUE(Axis) \
  (2.0 * (ji.dw ## Axis ## pos - jc.w ## Axis ## min) / (jc.w ## Axis ## max - jc.w ## Axis ## min) - 1.0)

/* Сохраненные координаты мыши */
static INT
  DS6_MouseOldX, DS6_MouseOldY;

/* Системный контекст анимации */
ds6ANIM DS6_Anim;

static INT64
  TimeFreq,  /* единиц измерения в секунду */
  TimeStart, /* время начала анимации */
  TimeOld,   /* время прошлого кадра */
  TimePause, /* время простоя в паузе */
  TimeFPS;   /* время для замера FPS */
static INT
  FrameCounter; /* счетчик кадров */


/* Функция установки паузы анимации.
 * АРГУМЕНТЫ:
 *   - флаг паузы:
 *       BOOL NewPauseFlag;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID DS6_AnimSetPause( BOOL NewPauseFlag )
{
  DS6_Anim.IsPause = NewPauseFlag;
} /* End of 'AG6_AnimSetPause' function */



/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID DS6_AnimInit( HWND hWnd )
{
  HDC hDC = GetDC(hWnd);
  LARGE_INTEGER li;

  /* Инициализация таймера */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;
  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  DS6_Anim.IsPause = FALSE;
  FrameCounter = 0;

  memset(&DS6_Anim, 0, sizeof(ds6ANIM));
  DS6_Anim.hWnd = hWnd;
  /* Инициализируем буфер кадра */
  DS6_Anim.hDC = CreateCompatibleDC(hDC);
  DS6_Anim.hBmFrame = CreateCompatibleBitmap(hDC, 30, 30);
  SelectObject(DS6_Anim.hDC, DS6_Anim.hBmFrame);
  DS6_Anim.W = 30;
  DS6_Anim.H = 30;
  DS6_Anim.NumOfUnits = 0;
  ReleaseDC(hWnd, hDC);

  /* Инициализация таймера */
  QueryPerformanceFrequency(&li);
  QueryPerformanceCounter(&li);
} /* End of 'DS6_AnimInit' function */

/* Функция деинициализации анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID DS6_AnimClose( VOID )
{
  INT i;

  /* Освобождение памяти из-под объектов анимации */
  for (i = 0; i < DS6_Anim.NumOfUnits; i++)
  {
    DS6_Anim.Units[i]->Close(DS6_Anim.Units[i], &DS6_Anim);
    free(DS6_Anim.Units[i]);
    DS6_Anim.Units[i] = NULL;
  }

  /* Удаляем объекты GDI */
  DeleteObject(DS6_Anim.hBmFrame);
  DeleteDC(DS6_Anim.hDC);
} /* End of 'DS6_AnimClose' function */

/* Функция обработки изменения размера области вывода.
 * АРГУМЕНТЫ:
 *   - новый размер области вывода:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID DS6_AnimResize( INT W, INT H )
{
  HDC hDC = GetDC(DS6_Anim.hWnd);

  /* Перевыделение памяти у буфера кадра */
  DeleteObject(DS6_Anim.hBmFrame);
  DS6_Anim.hBmFrame = CreateCompatibleBitmap(hDC, W, H);
  SelectObject(DS6_Anim.hDC, DS6_Anim.hBmFrame);

  /* Сохранение размера */
  DS6_Anim.W = W;
  DS6_Anim.H = H;
  DS6_Anim.NumOfUnits = 0;

  ReleaseDC(DS6_Anim.hWnd, hDC);
} /* End of 'DS6_AnimResize' function */

/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
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

  /* вычисляем FPS */
  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    static CHAR Buf[100];

    DS6_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  /* время "прошлого" кадра */
  TimeOld = li.QuadPart;

  FrameCounter++;


  /* очистка фона */
  SelectObject(DS6_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(DS6_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(DS6_Anim.hDC, RGB(0, 0, 0));
  Rectangle(DS6_Anim.hDC, 0, 0, DS6_Anim.W + 1, DS6_Anim.H + 1);

  /* keyboard and mouse and gamepad control */
  GetKeyboardState(DS6_Anim.Keys);
  for (i = 0; i < 256; i++)
    DS6_Anim.Keys[i] >>= 7;
  for (i = 0; i < 256; i++)
    DS6_Anim.KeysClick[i] = DS6_Anim.Keys[i] && !DS6_Anim.KeysOld[i];
  memcpy(DS6_Anim.KeysOld, DS6_Anim.Keys, sizeof(DS6_Anim.KeysOld));

  /* Мышь */
  /* колесо */
  DS6_Anim.MsWheel = DS6_MouseWheel;
  DS6_MouseWheel = 0;
  /* абсолютная позиция */
  GetCursorPos(&pt);
  ScreenToClient(DS6_Anim.hWnd, &pt);
  DS6_Anim.MsX = pt.x;
  DS6_Anim.MsY = pt.y;
  /* относительное перемещение */
  DS6_Anim.MsDeltaX = pt.x - DS6_MouseOldX;
  DS6_Anim.MsDeltaY = pt.y - DS6_MouseOldY;
  DS6_MouseOldX = pt.x;
  DS6_MouseOldY = pt.y;

  /* Джойстик */
  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS jc;

    /* получение общей информации о джостике */
    if (joyGetDevCaps(JOYSTICKID2, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      /* получение текущего состояния */
      ji.dwSize = sizeof(JOYCAPS);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID2, &ji) == JOYERR_NOERROR)
      {
        /* Кнопки */
        memcpy(DS6_Anim.JButsOld, DS6_Anim.JButs, sizeof(DS6_Anim.JButs));
        for (i = 0; i < 32; i++)
          DS6_Anim.JButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          DS6_Anim.JButsClick[i] = DS6_Anim.JButs[i] && !DS6_Anim.JButsOld[i];

        /* Оси */
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

  
  /* опрос на изменение состояний объектов */
  for (i = 0; i < DS6_Anim.NumOfUnits; i++)
    DS6_Anim.Units[i]->Response(DS6_Anim.Units[i], &DS6_Anim);

  /* рисование объектов */
  SelectObject(DS6_Anim.hDC, GetStockObject(WHITE_PEN));   
  for (i = 0; i < DS6_Anim.NumOfUnits; i++)
  {     
    DS6_Anim.Units[i]->Render(DS6_Anim.Units[i], &DS6_Anim);
  }
} /* End of 'DS6_AnimRender' function */

/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID DS6_AnimCopyFrame( VOID )
{
  HDC hDC;

  /* вывод кадра */
  hDC = GetDC(DS6_Anim.hWnd);
  BitBlt(hDC, 0, 0, DS6_Anim.W, DS6_Anim.H, DS6_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(DS6_Anim.hWnd, hDC);
} /* End of 'DS6_AnimCopyFrame' function */

/* Функция добавления в систему объекта анимации.
 * АРГУМЕНТЫ:
 *   - добавляемый объект анимации:
 *       ds6UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID DS6_AnimAddUnit( ds6UNIT *Unit )
{
  if (DS6_Anim.NumOfUnits < DS6_MAX_UNITS)
  {
    DS6_Anim.Units[DS6_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &DS6_Anim);
  }
} /* End of 'DS6_AnimAddUnit' function */


/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */

VOID DS6_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* текущий режим */
  static RECT SaveRC;               /* сохраненный размер */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* сохраняем старый размер окна */
    GetWindowRect(DS6_Anim.hWnd, &SaveRC);

    /* определяем в каком мониторе находится окно */
    hmon = MonitorFromWindow(DS6_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* получаем информацию для монитора */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* переходим в полный экран */
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
    /* восстанавливаем размер окна */
    SetWindowPos(DS6_Anim.hWnd, HWND_TOP,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'DS6_AnimFlipFullScreen' function */

/* Функция выхода из анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID DS6_AnimDoExit( VOID )
{
  PostMessage(DS6_Anim.hWnd, WM_CLOSE, 0, 0);
} /* End of 'DS6_AnimDoExit' function */

/* END OF 'ANIM.C' FILE */
