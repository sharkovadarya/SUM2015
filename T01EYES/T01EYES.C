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
VOID DrawEye( HDC hDC, INT w, INT h, INT Xc, INT Yc );

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
  static INT w, h, a = 200;
  HDC hDC;
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
    hDC = GetDC(hWnd); 
   /* DrawEye(hDC, w, h, w / 2, h / 2); */     
    ReleaseDC(hWnd, hDC);
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);  
    DrawEye(hDC, w, h, pt.x, pt.y);

   
                  
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
/*VOID DrawEye( HWND hWnd, HDC hDC, INT w, INT h, INT Xc, INT Yc )
{
  SelectObject(hDC, GetStockObject(DC_PEN));
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(255, 255, 255));
  Ellipse(hDC, w / 4 + (w / 8 - h / 8), h / 4, w - w / 2 - (w / 4 - h / 8), h - h / 2);
  Ellipse(hDC, w - w / 2 - (w / 4 - h / 8) + 2 * (w / 4 - h / 8), h / 4, w - 3 * w / 4 - (w / 4 - h / 8), h - h / 2);  SelectObject(hDC, GetStockObject(DC_PEN));
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(0, 0, 0));
  Ellipse(hDC, w / 4 + (w / 8 - h / 8), h / 4, w - w / 2 - (w / 4 - h / 8), h - h / 2);
  Ellipse(hDC, w - w / 2 - (w / 4 - h / 8) + 2 * (w / 4 - h / 8), h / 4, w - 3 * w / 4 - (w / 4 - h / 8), h - h / 2);  
}        */

/*VOID DrawEye( HWND hWnd, HDC hDC, INT w, INT h, INT Xc, INT Yc )
{
  static INT a = 200;                     
  Ellipse(hDC, w / 12, 5, (w - w / 6) / 2, (h - 10) / 2);
  Ellipse(hDC, w / 12 + w / 2, 5, (w - w / 6) / 2 + w / 2, (h - 10) / 2);    
  SelectObject (hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(0, 0, 0));
  Ellipse(hDC, w / 12 + a, 5 + a, (w - w / 6) / 2 - a, (h - 10) / 2 - a);
  Ellipse(hDC, w / 12 + w / 2 + a, 5 + a, (w - w / 6) / 2 + w / 2 - a, (h - 10) / 2 - a);   
  ReleaseDC(hWnd, hDC);   
} /* End of 'DrawEye' function */ 

VOID DrawEye( HDC hDC, INT W, INT H, INT Xc, INT Yc )
{
  FLOAT
    len = sqrt(sqr(Xc - W / 2) + sqr(Yc - H / 2)),
    co = (Xc - W / 2) / len, si = (Yc - H / 2) / len;
  INT l = 30, x = W / 2 + co * l, y = H / 2 + si * l;

  l = len;
  if (l > W / 2 - W / 8)
    l = W / 2 - W / 8;
  x = W / 2 + co * l;
  y = H / 2 + si * l;


  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(255, 255, 255));
  Ellipse(hDC, 0, 0, W, H);
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(2, 55, 5));
  Ellipse(hDC, x - W / 8, y - H / 8, x + W / 8, y + H / 8);

} /* End of 'DrawEye' function */




/* END OF 'T01FWIN.C' FILE */
