#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

#pragma comment(lib, "glew32s")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")

ANIM:
  . . .
  HDC hDC;            /* Контекст окна вывода */
  HGLRC hGLRC;        /* Контекст рендеринга */
  . . .


_AnimInit:
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};

  . . .
  _Anim.hDC = GetDC(hWnd);
  . . .
  /*** Инициализация OpenGL ***/
  /* описываем формат точки */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(VG4_Anim.hDC, &pfd);
  DescribePixelFormat(VG4_Anim.hDC, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  SetPixelFormat(VG4_Anim.hDC, i, &pfd);

  /* создаем контекст построения */
  _Anim.hGLRC = wglCreateContext(_Anim.hDC);
  /* делаем текущими контексты */
  wglMakeCurrent(_Anim.hDC, _Anim.hGLRC);

  /* инициализируем расширения */
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(VG4_Anim.hGLRC);
    ReleaseDC(_Anim.hWnd, _Anim.hDC);
    memset(&_Anim, 0, sizeof(vg4ANIM));
    return FALSE;
  }
  _RndProg = _ShaderLoad("TEST"); /* загрузка шейдера */
  
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.3, 0.5, 0.7, 1);
  . . .


_AnimClose:
  . . .
  _ShaderFree(VG4_RndProg);
  _RndProg = 0;
  /* делаем текущими контексты */
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(_Anim.hGLRC);
  /* Удаляем объекты GDI */
  ReleaseDC(_Anim.hWnd, _Anim.hDC);
  . . .

_AnimResize:
  . . .
  glViewport(0, 0, W, H);
  . . .


_AnimRender:
  . . .
  /* перерисовка кадра */

  /* очистка фона */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* "проббегаем" по всем объектам и отрисовываем геометрию */
  . . .
  glFinish();
  . . .

_AnimCopyFrame:
  /* вывод буфера кадра */
  SwapBuffers(VG4_Anim.hDC);


РИСОВАНИЕ
/* Шейдер по умолчанию */
UINT _RndProg;

/* Структура хранения данных о вершине */
typedef struct tagVERTEX
{
  VEC P;   /* позиция */
  COLOR C; /* Цвет */
} VERTEX;

/* Тип представления объекта - набор вершин и треугольников */
typedef struct tagvg4GOBJ
{
  INT
    VA,          /* массив вершин */
    VBuf, IBuf,  /* буфера вершин и индексов */
    NumOfV,      /* количество вершин */
    NumOfF;      /* количество граней */
} vg4GOBJ;


_RndGObjLoad:
  vg4GOBJ *GObj; - параметр
  . . .
  /* отправляем в OpenGL */
  glGenVertexArrays(1, &GObj->VA);
  glGenBuffers(1, &GObj->VBuf);
  glGenBuffers(1, &GObj->IBuf);

  /* делаем активным массив вершин */
  glBindVertexArray(GObj->VA);
  /* делаем активным буфер вершин */
  glBindBuffer(GL_ARRAY_BUFFER, GObj->VBuf);
  /* сливаем данные */
  glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX) * GObj->NumOfV, V, GL_STATIC_DRAW);
  /* делаем активным буфер индексов */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GObj->IBuf);
  /* сливаем данные */
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT [3]) * GObj->NumOfF, Facets, GL_STATIC_DRAW);

  /* задаем порядок данных */
  /*                    layout,
   *                       количество компонент,
   *                          тип,
   *                                    надо ли нормировать,
   *                                           размер в байтах одного элемента буфера,
   *                                                           смещение в байтах до начала данных */
  glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(VERTEX), (VOID *)0); /* позиция */
  glVertexAttribPointer(1, 4, GL_FLOAT, FALSE, sizeof(VERTEX), (VOID *)sizeof(VEC)); /* цвет */

  /* включаем нужные аттрибуты (layout) */
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  . . .
  glBindVertexArray(0);

_RndGObjDraw:
  vg4GOBJ *GObj; - параметр
  INT loc;
  . . .
  /* рисуем треугольники */
  glBindVertexArray(GObj->VA);
  glUseProgram(_RndProg);
  /* задаем униформную (константа для шейдера) матрицу */
  loc = glGetUniformLocation(_RndProg, "MatrWorld");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, _RndMatrWorld.A[0]);
  . . .
  /* задаем униформное число */
  loc = glGetUniformLocation(_RndProg, "Time");
  if (loc != -1)
    glUniform1f(loc, _Anim.Time);
  . . .
  /* вызываем отрисовку треугольников по их индексам */
  glDrawElements(GL_TRIANGLES, GObj->NumOfF * 3, GL_UNSIGNED_INT, NULL);
  glUseProgram(0);
  glBindVertexArray(0);


