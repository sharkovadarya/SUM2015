/* FILE: T05GLOBE.C
 * PROGRAMMER: DS6
 * DATE: 04.06.2015
 * PURPOSE: output an image of a 3d sphere.
*/


#include <windows.h>
#include "def.h"
#include "globe.h"

/* Имя класса окна */
#define WND_CLASS_NAME "My window class"

#define PI 3.14159265358979323846

/* Ссылка вперед */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );

VOID FlipFullScreen( HWND hWnd ) 
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


} 


/* Главная функция программы.
 *   - дескриптор экземпляра приложения:
 *       HINSTANCE hInstance;
 *   - дескриптор предыдущего экземпляра приложения
 *     (не используется и должно быть NULL):
 *       HINSTANCE hPrevInstance;
 *   - командная строка:
 *       CHAR *CmdLine;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) код возврата в операционную систему.
 *   0 - при успехе.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  /* HINSTANCE hIns = LoadLibrary("shell32"); */

  /* Регистрация класса окна */
  wc.style = CS_VREDRAW | CS_HREDRAW; /* Стиль окна: полностью перерисовывать
                                       * при изменении вертикального или
                                       * горизонтального размеров
                                       * еще можно CS_DBLCLKS для добавления
                                       * отработки двойного нажатия */
  wc.cbClsExtra = 0; /* Дополнительное количество байт для класса */
  wc.cbWndExtra = 0; /* Дополнительное количество байт для окна */
  wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
  wc.hCursor = LoadCursor(NULL, IDC_HAND); /* Загрузка курсора (системного) */
  wc.hIcon = LoadIcon(NULL, IDI_ASTERISK); /* Загрузка пиктограммы (системной) */
  wc.hInstance = hInstance; /* Дескриптор приложения, регистрирующего класс */
  wc.lpszMenuName = NULL; /* Имя ресурса меню */
  wc.lpfnWndProc = MyWindowFunc; /* Указатель на функцию обработки */
  wc.lpszClassName = WND_CLASS_NAME;

  /* Регистрация класса в системе */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  /* Создание окна */
  hWnd =
    CreateWindow(WND_CLASS_NAME,    /* Имя класса окна */
      "Title",                      /* Заголовок окна */
      WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,          /* Стили окна - окно общего вида */
      CW_USEDEFAULT, CW_USEDEFAULT, /* Позиция окна (x, y) - по умолчанию */
      CW_USEDEFAULT, CW_USEDEFAULT, /* Размеры окна (w, h) - по умолчанию */
      NULL,                         /* Дескриптор родительского окна */
      NULL,                         /* Дескриптор загруженного меню */
      hInstance,                    /* Дескриптор приложения */
      NULL);                        /* Указатель на дополнительные параметры */

  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

  /* Запуск цикла сообщений окна */
  while (GetMessage(&msg, NULL, 0, 0))
  {
    /* Обработка сообщений от клавиатуры */
    TranslateMessage(&msg);
    /* Передача сообщений в функцию окна */
    DispatchMessage(&msg);
  }

  return msg.wParam;
} /* End of 'WinMain' function */

/* Функция обработки сообщения окна.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 *   - номер сообщения (см. WM_***):
 *       UINT Msg;
 *   - параметр сообшения ('word parameter'):
 *       WPARAM wParam;
 *   - параметр сообшения ('long parameter'):
 *       LPARAM lParam;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (LRESULT) - в зависимости от сообщения.
 */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  CREATESTRUCT *cs; 
  static BITMAP bm;
  static HBITMAP hBm, hBmLogo;
  static HDC hMemDC; 
  static INT w, h;
  

  switch (Msg)
  {
  case WM_CREATE:
    cs = (CREATESTRUCT *)lParam;
    SetTimer(hWnd, 111, 50, NULL);     
    /* создаем контекст в памяти */
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    return 0;

  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);

    if (hBm != NULL)
      DeleteObject(hBm);

    hDC = GetDC(hWnd);                                                                            
    hBm = CreateCompatibleBitmap(hDC, w, h); 
    
    ReleaseDC(hWnd, hDC);

    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 111, 0);      
    return 0;

  case WM_TIMER:      
    /* Clear Background */
    SelectObject(hMemDC, GetStockObject(NULL_PEN));
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hMemDC, RGB(255, 255, 255));   
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);  

    
    DrawGlobe(hMemDC, w, h);

    InvalidateRect(hWnd, NULL, TRUE);
    return 0;  

  case WM_KEYDOWN:
    if (wParam = 'F')
      FlipFullScreen(hWnd);


   case WM_ERASEBKGND:
    BitBlt((HDC)wParam, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    return 0;

  case WM_DESTROY:
    DeleteDC(hMemDC);
    DeleteObject(hBm);
    KillTimer(hWnd, 111);
    PostQuitMessage(0);
    return 0;

  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */


/* END OF 'T05GLOBE.C' FILE */
