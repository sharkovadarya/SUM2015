/* FILE NAME: T02CLOCK.C
 * PROGRAMMER: DS6
 * DATE: 02.06.2015
 * PURPOSE: clock visualisation.
 */

#pragma warning(disable: 4244)

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <windows.h>

/* Имя класса окна */
#define WND_CLASS_NAME "My window class"

#define PI 3.14159265358979323846

/* Ссылка вперед */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );
VOID DrawArrow( HDC hDC, INT X1, INT Y1, INT Len, DOUBLE Angle );
VOID DrawHand( HDC hDC, INT X1, INT Y1, INT Len, INT w, DOUBLE Angle );

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
  wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 0));
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
  SYSTEMTIME st;
  CHAR Buf[100];
  HFONT hFnt, hOldFnt; 
 /* HPEN hPen; */
 /* HBRUSH hBrush, hOldBr;*/
  static INT xx[100], yy[100];
  static BITMAP bm;
  static HBITMAP hBm, hBmLogo;
  static HDC hMemDC, hMemDCLogo;
  static INT w, h;
  

  switch (Msg)
  {
  case WM_CREATE:
    cs = (CREATESTRUCT *)lParam;
    SetTimer(hWnd, 111, 50, NULL);     

    hBmLogo = LoadImage(NULL, "clock-face-14.bmp", IMAGE_BITMAP, w, h, LR_LOADFROMFILE);  
    GetObject(hBmLogo, sizeof(bm), &bm);   

    /* создаем контекст в памяти */
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hMemDCLogo = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    SelectObject(hMemDCLogo, hBmLogo);     
    return 0;

  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);

    /* создаем картинку размером с окно */
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
    Rectangle(hMemDC, 0, 0, w, h);

    StretchBlt(hMemDC, w / 2 - bm.bmWidth / 2, h / 2 - bm.bmHeight / 2, bm.bmWidth, bm.bmHeight,
      hMemDCLogo, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);    

    GetLocalTime(&st);
    
    SelectObject(hMemDC, GetStockObject(BLACK_PEN));
    SelectObject(hMemDC, GetStockObject(BLACK_BRUSH)); 
    DrawHand(hMemDC, w / 2, h / 2, 280, 30, st.wSecond * 6 * PI / 180 + PI);
    DrawHand(hMemDC, w / 2, h / 2, 240, 30, (180 + st.wMinute * 6) * PI / 180);
    DrawHand(hMemDC, w / 2, h / 2, 150, 30, (int)st.wHour % 12 * PI / 180);
    //DrawHand(hMemDC, w / 2, h / 2, 300, 30, (180 + (st.wHour % 12) * 30)) * PI / 180);
   
    SelectObject(hMemDC, GetStockObject(NULL_PEN));
    SelectObject(hMemDC, GetStockObject(NULL_BRUSH));

    hFnt = CreateFont(30, 0, 0, 0, FW_BOLD, TRUE, FALSE,
      FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS,
      CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
      VARIABLE_PITCH | FF_ROMAN, "");     
   
    hOldFnt = SelectObject(hMemDC, hFnt);
    GetLocalTime(&st);
    SetTextColor(hMemDC, RGB(2, 2, 8));
    SetBkColor(hMemDC, RGB(12, 24, 200));
    SetBkMode(hMemDC, TRANSPARENT);
    TextOut(hMemDC, w / 2 - 95, 3 * h / 4 - 50, Buf,
      sprintf(Buf, "%02d:%02d:%02d (%02d.%02d.%d)",
        st.wHour, st.wMinute, st.wSecond,
        st.wDay, st.wMonth, st.wYear)); 

    DeleteObject(hFnt);  

    InvalidateRect(hWnd, NULL, TRUE);
    return 0;      

  case WM_ERASEBKGND:
    BitBlt((HDC)wParam, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    return 0;

  case WM_DESTROY:
    DeleteDC(hMemDC);
    DeleteObject(hBm);
    DeleteDC(hMemDCLogo);
    DeleteObject(hBmLogo);
    KillTimer(hWnd, 111);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* Draw clock's arrow function.
 * ARGUMENTS:
 *   - device context:
 *       HDC hDC;
 *   - center coordinates:
 *       INT X1, INT Y1;
 *   - arrow length:
 *       INT Len;
 *   - angle:
 *       DOUBLE Angle;
 * RETURNED VALUES:
 *   none.
 */
VOID DrawArrow( HDC hDC, INT X1, INT Y1, INT Len, DOUBLE Angle )
{
  DOUBLE si = sin(Angle * PI / 180), co = cos(Angle * PI / 180);

  MoveToEx(hDC, X1, Y1, NULL);
  LineTo(hDC, X1 + Len * si, Y1 - Len * co);
} /* End of 'DrawArrow' function */

/* Draw clock's hand function.
 * ARGUMENTS:
 *   - device context:
 *       HDC hDC;
 *   - center coordinates:
 *       INT X1, INT Y1;
 *   - arrow length:
 *       INT Len;
     - arrow width:
         INT w;
 *   - angle:
 *       DOUBLE Angle;
 * RETURNED VALUES:
 *   none.
 */
VOID DrawHand( HDC hDC, INT X1, INT Y1, INT Len, INT w, DOUBLE Angle )
{
  DOUBLE si = sin(Angle), co = cos(Angle);
  INT i;
  POINT pnts[] = 
  {
    {0, -w}, {-w, 0}, {0, Len}, {w, 0}
  }, pntdraw[sizeof pnts / sizeof pnts[0]];

  for (i = 0; i < sizeof pnts / sizeof pnts[0]; i++)
  {
    pntdraw[i].x = X1 + pnts[i].x * co - pnts[i].y * si;
    pntdraw[i].y = Y1 + pnts[i].x * si + pnts[i].y * co;
  }
  Polygon(hDC, pntdraw, 4);
} /* End of 'DrawArrow' function */

/* END OF 'T02DBLB.C' FILE */
