/* T01FWIN.C
 * Пример базового макета программы под WinAPI.
 * Создание и поддержка простейшего окна.
 */

#include <windows.h>
#include <math.h>

/* Имя класса окна */
#define WND_CLASS_NAME "My window class"

/* Ссылка вперед */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );
VOID DrawEye( HWND hWnd, INT w, INT h );
VOID DrawPupil( HWND hWnd, INT w, INT h );

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
  wc.hbrBackground = CreateSolidBrush(RGB(0, 255, 255));
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
      WS_OVERLAPPEDWINDOW,          /* Стили окна - окно общего вида */
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
    /* Передача сообщений в функцию окна */
    DispatchMessage(&msg);

  return 30;
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
  INT s, d, x0,  y0;
  static INT w, h, a = 200;
  POINT pt;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 111, 50, NULL);
    return 0;
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);       
    return 0;
  case WM_TIMER:
    DrawEye(hWnd, w, h);
    DrawPupil(hWnd, w, h);
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);   

    x0 = (w / 12 + a + (w - w / 6) / 2 - a) / 2,  y0 = (5 + (h - 10) / 2) / 2;
    s = (pt.y - y0) / sqrt((pt.x - x0) * (pt.x - x0) + (pt.y - y0) * (pt.y - y0)) + y0;
    d = (s - y0) * (pt.x - x0) / (pt.y - y0);
    DrawPupil(hWnd, d, s); 
    

    return 0;
  case WM_DESTROY:
    KillTimer(hWnd, 111);
    PostMessage(hWnd, WM_QUIT, 0, 0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* Draw Eye function.
 * ARGUMENTS:
 *   - window descriptor:
 *       HWND hWnd;
 *   - window width:
 *       INT w;
 *   - window height:
 *       INT h;   
 * RETURNING:
 *   none.
 */
VOID DrawEye( HWND hWnd, INT w, INT h )
{
  HDC hDC;
  static INT a = 200;

  hDC = GetDC(hWnd);      
  Ellipse(hDC, w / 12, 5, (w - w / 6) / 2, (h - 10) / 2);
  Ellipse(hDC, w / 12 + w / 2, 5, (w - w / 6) / 2 + w / 2, (h - 10) / 2);     
  ReleaseDC(hWnd, hDC);   
} /* End of 'DrawEye' function */

/* Draw Eye Pupil function.
 * ARGUMENTS:
 *   - window descriptor:
 *       HWND hWnd;
 *   - window width:
 *       INT w;
 *   - window height:
 *       INT h;   
 * RETURNING:
 *   none.
 */
VOID DrawPupil( HWND hWnd, INT w, INT h )
{
  HDC hDC;
  static INT a = 200;

  hDC = GetDC(hWnd);
  SelectObject (hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(0, 0, 0));
  Ellipse(hDC, w / 12 + a, 5 + a, (w - w / 6) / 2 - a, (h - 10) / 2 - a);
  Ellipse(hDC, w / 12 + w / 2 + a, 5 + a, (w - w / 6) / 2 + w / 2 - a, (h - 10) / 2 - a);  
  ReleaseDC(hWnd, hDC);     
} /* End of 'DrawPupil' function */
/* END OF 'T01FWIN.C' FILE */
