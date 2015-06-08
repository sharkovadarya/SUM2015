/* FILENAME: ANIM.C
 * PROGRAMMER: DS6
 * PURPOSE: Animation system module.
 * LAST UPDATE: 08.06.2015
 */

#include "anim.h"

/* Системный контекст анимации */
static ds6ANIM DS6_Anim;

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

  ReleaseDC(DS6_Anim.hWnd, hDC);
} /* End of 'DS6_AnimResize' function */

/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID DS6_AnimRender( VOID )
{
  INT i;

  /* опрос на изменение состояний объектов */
  for (i = 0; i < DS6_Anim.NumOfUnits; i++)
    DS6_Anim.Units[i]->Response(DS6_Anim.Units[i], &DS6_Anim);

  /* очистка фона */
  SelectObject(DS6_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(DS6_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(DS6_Anim.hDC, RGB(0, 0, 0));
  Rectangle(DS6_Anim.hDC, 0, 0, DS6_Anim.W + 1, DS6_Anim.H + 1);

  /* рисование объектов */
  for (i = 0; i < DS6_Anim.NumOfUnits; i++)
  {
    SelectObject(DS6_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(DS6_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(DS6_Anim.hDC, RGB(255, 255, 255));
    SetDCPenColor(DS6_Anim.hDC, RGB(0, 0, 0));

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
