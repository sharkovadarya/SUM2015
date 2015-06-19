/* FILE: RENDER.C
 * PROGRAMMER: DS6
 * LAST UPDATE: 10.06.2015
 * PURPOSE: Rendering system module. 
 */


#include <stdio.h>
#include <string.h>

#include "anim.h"
#include "render.h"

#pragma comment(lib, "glew32s")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")   

/* Матрицы */
MATR
  DS6_RndMatrWorld = DS6_UNIT_MATR,
  DS6_RndMatrView = DS6_UNIT_MATR,
  DS6_RndMatrProj = DS6_UNIT_MATR,
  DS6_RndMatrWorldViewProj = DS6_UNIT_MATR;

/* Параметры проецирования */
DBL
  DS6_RndWp = 3, DS6_RndHp = 3,     /* размеры обрасти проецирования */
  DS6_RndProjDist = 5;              /* расстояние до плоскости проекции */

/* Шейдер по умолчанию */
UINT DS6_RndProg;

/* Sphere shader */
UINT DS6_SphereShader;

typedef struct tagVERTEX
{
  VEC P;   /* позиция */
  COLOR C; /* Цвет */
} VERTEX;     

/* Функция загрузки геометрического объекта.
* АРГУМЕНТЫ:
*   - структура объекта для загрузки:
*       ds6PRIM *GObj;
*   - имя файла:
*       CHAR *FileName;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
*   (BOOL) TRUE при успехе, FALSE иначе.
*/
BOOL DS6_PrimLoad( ds6PRIM *GObj, CHAR *FileName )
{
  FILE *F;
  ds6VERTEX *V;
  INT (*Facets)[3];
  INT nv = 0, nf = 0;
  static CHAR Buf[10000];

  memset(GObj, 0, sizeof(ds6PRIM));
  /* Open file */
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Count vertices */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;
  }

  /* Allocate memory for data */
  if ((V = malloc(sizeof(ds6VERTEX) * nv + sizeof(INT [3]) * nf)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  Facets = (INT (*)[3])(V + nv);
  memset(V, 0, sizeof(ds6VERTEX) * nv + sizeof(INT [3]) * nf);

  /* Read vertices */
  rewind(F);
  nv = nf = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%f%f%f",
        &V[nv].P.X, &V[nv].P.Y, &V[nv].P.Z);
      V[nv].C = ColorSet(Rnd0(), Rnd0(), Rnd0(), 1);
      nv++;
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT a, b, c;

      sscanf(Buf + 2, "%i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &a, &b, &c) == 3 ||
              sscanf(Buf + 2, "%i//%*i %i//%*i %i//%*i", &a, &b, &c) == 3 ||
              sscanf(Buf + 2, "%i/%*i %i/%*i %i/%*i", &a, &b, &c) == 3 ||
              sscanf(Buf + 2, "%i %i %i", &a, &b, &c);

      Facets[nf][0] = a - 1;
      Facets[nf][1] = b - 1;
      Facets[nf][2] = c - 1;
      nf++;
    }
  }
  fclose(F);
  DS6_PrimCreate(GObj, DS6_PRIM_TRIMESH, nv, nf * 3, V, (INT *)Facets);

  /* освобождаем оперативную память */
  free(V);
  return TRUE;
} /* End of 'DS6_PrimLoad' function */

/* Функция загрузки текстуры.
* АРГУМЕНТЫ:
*   - имя файла:
*       CHAR *FileName;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
*   (INT ) идентификатор OpenGL для текстуры.
*/
INT DS6_TextureLoad( CHAR *FileName )
{
INT TexId = 0, i;
  HDC hMemDC, hMemDCDIB;
  BITMAP bm;
  HBITMAP hBm, hBmDIB;
  DWORD *Bits;
  BITMAPINFOHEADER bih = {0};
  /* склад уже загруженных текстур */
  static struct
  {
    CHAR Name[300]; /* имя файла */
    INT TexId;      /* идентификатор текстур */
  } StoreTex[300];
  static INT StoreTexSize = 0;

  /* ищем в уже загруженных */
  for (i = 0; i < StoreTexSize; i++)
    if (strcmp(FileName, StoreTex[i].Name) == 0)
      return StoreTex[i].TexId;

  /* загружаем изображение из файла */
  hBm = LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBm == NULL)
    return 0;

  /* Create compatible context and select image into one */
  hMemDC = CreateCompatibleDC(DS6_Anim.hDC);
  hMemDCDIB = CreateCompatibleDC(DS6_Anim.hDC);
  SelectObject(hMemDC, hBm);

  /* Obtain image size */
  GetObject(hBm, sizeof(BITMAP), &bm);

  /* строим DIB - device-independed-bitmap секция */
  bih.biSize = sizeof(BITMAPINFOHEADER);
  bih.biWidth = bm.bmWidth;
  bih.biHeight = -bm.bmHeight;
  bih.biBitCount = 32;
  bih.biCompression = BI_RGB;
  bih.biPlanes = 1;
  bih.biSizeImage = bm.bmWidth * bm.bmHeight * 4;

  hBmDIB = CreateDIBSection(NULL, (BITMAPINFO *)&bih, DIB_RGB_COLORS, &Bits, NULL, 0);
  SelectObject(hMemDCDIB, hBmDIB);

  /* отображаем в DIB картинку */
  BitBlt(hMemDCDIB, 0, 0, bm.bmWidth, bm.bmHeight, hMemDC, 0, 0, SRCCOPY);

  /* установка альфа канала */
  for (i = bm.bmWidth * bm.bmHeight - 1; i >= 0; i--)
    Bits[i] |= 0xFF000000;

  /* переносим в память OpenGL */
  glGenTextures(1, &TexId);
  glBindTexture(GL_TEXTURE_2D, TexId);
  gluBuild2DMipmaps(GL_TEXTURE_2D, 4, bm.bmWidth, bm.bmHeight,
    GL_BGRA_EXT, GL_UNSIGNED_BYTE, Bits);
  glBindTexture(GL_TEXTURE_2D, 0);

  DeleteDC(hMemDC);
  DeleteDC(hMemDCDIB);
  DeleteObject(hBm);
  DeleteObject(hBmDIB);

  /* сохраняем на складе */
  if (StoreTexSize < 300)
  {
    strncpy(StoreTex[StoreTexSize].Name, FileName, 300);
    StoreTex[StoreTexSize].TexId = TexId;
    StoreTexSize++;
  }
  return TexId;

} /* End of 'DS6_TextureLoadfunction */

/* END OF 'RENDER.C' FILE */