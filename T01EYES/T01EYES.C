/* T01FWIN.C
 * ������ �������� ������ ��������� ��� WinAPI.
 * �������� � ��������� ����������� ����.
 */

#include <windows.h>
#include <math.h>

/* ��� ������ ���� */
#define WND_CLASS_NAME "My window class"
#define sqr(x) ((x) * (x))

/* ������ ������ */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );
VOID DrawEye( HDC hDC, INT w, INT h );   
VOID DrawPupil( HDC hDC, INT w, INT h, INT X1, INT Y1 );

/* ������� ������� ���������.
 *   - ���������� ���������� ����������:
 *       HINSTANCE hInstance;
 *   - ���������� ����������� ���������� ����������
 *     (�� ������������ � ������ ���� NULL):
 *       HINSTANCE hPrevInstance;
 *   - ��������� ������:
 *       CHAR *CmdLine;
 * ������������ ��������:
 *   (INT) ��� �������� � ������������ �������.
 *   0 - ��� ������.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  /* HINSTANCE hIns = LoadLibrary("shell32"); */

  /* ����������� ������ ���� */
  wc.style = CS_VREDRAW | CS_HREDRAW; /* ����� ����: ��������� ��������������
                                       * ��� ��������� ������������� ���
                                       * ��������������� ��������
                                       * ��� ����� CS_DBLCLKS ��� ����������
                                       * ��������� �������� ������� */
  wc.cbClsExtra = 0; /* �������������� ���������� ���� ��� ������ */
  wc.cbWndExtra = 0; /* �������������� ���������� ���� ��� ���� */
  wc.hbrBackground = CreateSolidBrush(RGB(0, 255, 255));
  wc.hCursor = LoadCursor(NULL, IDC_HAND); /* �������� ������� (����������) */
  wc.hIcon = LoadIcon(NULL, IDI_ASTERISK); /* �������� ����������� (���������) */
  wc.hInstance = hInstance; /* ���������� ����������, ��������������� ����� */
  wc.lpszMenuName = NULL; /* ��� ������� ���� */
  wc.lpfnWndProc = MyWindowFunc; /* ��������� �� ������� ��������� */
  wc.lpszClassName = WND_CLASS_NAME;

  /* ����������� ������ � ������� */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  /* �������� ���� */
  hWnd =
    CreateWindow(WND_CLASS_NAME,    /* ��� ������ ���� */
      "Title",                      /* ��������� ���� */
      WS_OVERLAPPEDWINDOW,          /* ����� ���� - ���� ������ ���� */
      CW_USEDEFAULT, CW_USEDEFAULT, /* ������� ���� (x, y) - �� ��������� */
      CW_USEDEFAULT, CW_USEDEFAULT, /* ������� ���� (w, h) - �� ��������� */
      NULL,                         /* ���������� ������������� ���� */
      NULL,                         /* ���������� ������������ ���� */
      hInstance,                    /* ���������� ���������� */
      NULL);                        /* ��������� �� �������������� ��������� */

  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

  /* ������ ����� ��������� ���� */
  while (GetMessage(&msg, NULL, 0, 0))
    /* �������� ��������� � ������� ���� */
    DispatchMessage(&msg);

  return 30;
} /* End of 'WinMain' function */

/* ������� ��������� ��������� ����.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 *   - ����� ��������� (��. WM_***):
 *       UINT Msg;
 *   - �������� ��������� ('word parameter'):
 *       WPARAM wParam;
 *   - �������� ��������� ('long parameter'):
 *       LPARAM lParam;
 * ������������ ��������:
 *   (LRESULT) - � ����������� �� ���������.
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
