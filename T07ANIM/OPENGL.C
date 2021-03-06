#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

#pragma comment(lib, "glew32s")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")

ANIM:
  . . .
  HDC hDC;            /* �������� ���� ������ */
  HGLRC hGLRC;        /* �������� ���������� */
  . . .


_AnimInit:
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};

  . . .
  _Anim.hDC = GetDC(hWnd);
  . . .
  /*** ������������� OpenGL ***/
  /* ��������� ������ ����� */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(VG4_Anim.hDC, &pfd);
  DescribePixelFormat(VG4_Anim.hDC, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  SetPixelFormat(VG4_Anim.hDC, i, &pfd);

  /* ������� �������� ���������� */
  _Anim.hGLRC = wglCreateContext(_Anim.hDC);
  /* ������ �������� ��������� */
  wglMakeCurrent(_Anim.hDC, _Anim.hGLRC);

  /* �������������� ���������� */
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(VG4_Anim.hGLRC);
    ReleaseDC(_Anim.hWnd, _Anim.hDC);
    memset(&_Anim, 0, sizeof(vg4ANIM));
    return FALSE;
  }
  _RndProg = _ShaderLoad("TEST"); /* �������� ������� */
  
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.3, 0.5, 0.7, 1);
  . . .


_AnimClose:
  . . .
  _ShaderFree(VG4_RndProg);
  _RndProg = 0;
  /* ������ �������� ��������� */
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(_Anim.hGLRC);
  /* ������� ������� GDI */
  ReleaseDC(_Anim.hWnd, _Anim.hDC);
  . . .

_AnimResize:
  . . .
  glViewport(0, 0, W, H);
  . . .


_AnimRender:
  . . .
  /* ����������� ����� */

  /* ������� ���� */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* "����������" �� ���� �������� � ������������ ��������� */
  . . .
  glFinish();
  . . .

_AnimCopyFrame:
  /* ����� ������ ����� */
  SwapBuffers(VG4_Anim.hDC);


���������
/* ������ �� ��������� */
UINT _RndProg;

/* ��������� �������� ������ � ������� */
typedef struct tagVERTEX
{
  VEC P;   /* ������� */
  COLOR C; /* ���� */
} VERTEX;

/* ��� ������������� ������� - ����� ������ � ������������� */
typedef struct tagvg4GOBJ
{
  INT
    VA,          /* ������ ������ */
    VBuf, IBuf,  /* ������ ������ � �������� */
    NumOfV,      /* ���������� ������ */
    NumOfF;      /* ���������� ������ */
} vg4GOBJ;


_RndGObjLoad:
  vg4GOBJ *GObj; - ��������
  . . .
  /* ���������� � OpenGL */
  glGenVertexArrays(1, &GObj->VA);
  glGenBuffers(1, &GObj->VBuf);
  glGenBuffers(1, &GObj->IBuf);

  /* ������ �������� ������ ������ */
  glBindVertexArray(GObj->VA);
  /* ������ �������� ����� ������ */
  glBindBuffer(GL_ARRAY_BUFFER, GObj->VBuf);
  /* ������� ������ */
  glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX) * GObj->NumOfV, V, GL_STATIC_DRAW);
  /* ������ �������� ����� �������� */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GObj->IBuf);
  /* ������� ������ */
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT [3]) * GObj->NumOfF, Facets, GL_STATIC_DRAW);

  /* ������ ������� ������ */
  /*                    layout,
   *                       ���������� ���������,
   *                          ���,
   *                                    ���� �� �����������,
   *                                           ������ � ������ ������ �������� ������,
   *                                                           �������� � ������ �� ������ ������ */
  glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(VERTEX), (VOID *)0); /* ������� */
  glVertexAttribPointer(1, 4, GL_FLOAT, FALSE, sizeof(VERTEX), (VOID *)sizeof(VEC)); /* ���� */

  /* �������� ������ ��������� (layout) */
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  . . .
  glBindVertexArray(0);

_RndGObjDraw:
  vg4GOBJ *GObj; - ��������
  INT loc;
  . . .
  /* ������ ������������ */
  glBindVertexArray(GObj->VA);
  glUseProgram(_RndProg);
  /* ������ ���������� (��������� ��� �������) ������� */
  loc = glGetUniformLocation(_RndProg, "MatrWorld");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, _RndMatrWorld.A[0]);
  . . .
  /* ������ ���������� ����� */
  loc = glGetUniformLocation(_RndProg, "Time");
  if (loc != -1)
    glUniform1f(loc, _Anim.Time);
  . . .
  /* �������� ��������� ������������� �� �� �������� */
  glDrawElements(GL_TRIANGLES, GObj->NumOfF * 3, GL_UNSIGNED_INT, NULL);
  glUseProgram(0);
  glBindVertexArray(0);


