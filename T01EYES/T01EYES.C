/* T01FWIN.C
 * Пример базового макета программы под WinAPI.
 * Создание и поддержка простейшего окна.
 */

#include <windows.h>
#include <math.h>

/* Имя класса окна */
#define WND_CLASS_NAME "My window class"
#define sqr(x) ((x) * (x))

/* Ссылка вперед */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );
VOID DrawEye( HDC hDC, INT w, INT h );   
VOID DrawPupil( HDC hDC, INT w, INT h, INT X1, INT Y1 );

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
  HDC hDC;
  POINT pt;
  static INT w, h, a = 200;
  

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
 
    hDC = GetDC(hWnd);
    SelectObject(hDC, GetStockObject(BLACK_PEN));
    SelectObject(hDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hDC, RGB(0, 255, 255));
    Rectangle(hDC, 0, 0, w + 1, h + 1);

    
    DrawEye(hDC, w, h);    
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);   
    DrawPupil(hDC, w, h, pt.x, pt.y); 
    ReleaseDC(hWnd, hDC);
                  
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


VOID DrawEye( HDC hDC, INT w, INT h )
{
  SelectObject(hDC, GetStockObject(WHITE_BRUSH));
  Ellipse(hDC, w / 2 - 150, h / 2 + 50, w / 2 - 50, h / 2 - 50);
  Ellipse(hDC, w / 2 + 50, h / 2 + 50, w / 2 + 150, h / 2 - 50);  
  SelectObject(hDC, GetStockObject(NULL_BRUSH));

} /* End of 'DrawEye' function */

/* Draw Eye Pupil function.
 * ARGUMENTS:
 *   - window descriptor:
 *       HWND hWnd;
 *   - window width:    
 *       INT w;
 *   - window height:
 *       INT h; 
 *   - center coordinates:
         INT X1, Y1;
 * RETURNING:
 *   none.
 */
VOID DrawPupil( HDC hDC, INT w, INT h, INT X1, INT Y1 )
{
  DOUBLE len = sqrt(sqr(X1 - w / 2 + 100) + sqr(Y1 - h / 2)), co = (X1 - w / 2 + 100) / len, si = (Y1 - h / 2) / len;
  INT l = 25, x = w / 2 - 100 + co * l, y = h / 2 + si * l;

  l = len;
  if (l > 25)
    l = 25;
  x = w / 2 - 100 + co * l;
  y = h / 2 + si * l;

  SelectObject(hDC, GetStockObject(BLACK_BRUSH));
  Ellipse(hDC, x - 10, y + 10, x + 10, y - 10);

  len = sqrt(sqr(X1 - w / 2 - 100) + sqr(Y1 - h / 2)), co = (X1 - w / 2 - 100) / len, si = (Y1 - h / 2) / len;
  l = len;
  if (l > 20)
    l = 20;
  x = w / 2 + 100 + co * l;
  y = h / 2 + si * l;
  Ellipse(hDC, x - 10, y + 10, x + 10, y - 10);
  SelectObject(hDC, GetStockObject(NULL_BRUSH)); 
} /* End of 'DrawEye' function */


/* END OF 'T01FWIN.C' FILE */
