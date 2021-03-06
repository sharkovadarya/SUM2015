/* FILENAME: RENDER.H
* PROGRAMMER: DS6
* PURPOSE: Rendering system declaration module.
* LAST UPDATE: 13.06.2015
*/

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

/* ������� */
extern MATR
  DS6_RndMatrWorld,
  DS6_RndMatrView,
  DS6_RndMatrProj,
  DS6_RndMatrWorldViewProj;

/* ������� ��������� ��������� ��� �������� */
extern MATR DS6_RndPrimMatrConvert;

/* ��������� ������������� */
extern DBL
  DS6_RndWp, DS6_RndHp,    /* ������� ������� ������������� */
  DS6_RndProjDist;         /* ���������� �� ��������� �������� */

/* ������ �� ��������� */
extern UINT DS6_RndProg;

/* ������� �������� �������� ��� ����� ���������.
* ���������:
*   - ������� ����� �����:
*       CHAR *FileNamePrefix;
* ������������ ��������:
*   (UINT) ����� ����������� ���������.
*/
UINT DS6_ShaderLoad( CHAR *FileNamePrefix );

/* ������� ����������� �������� ��� ���������.
* ���������:
*   - ����� ���������:
*       UINT PrgId;
* ������������ ��������: ���.
*/
VOID DS6_ShaderFree( UINT PrgId );

/***
 * ������ � �����������
 ***/

/* ��� ������� ��������� ����������� */
typedef struct tagds6MATERIAL
{
  /* ��� ��������� */
  CHAR Name[300];

  /* ������������ ��������� */
  VEC Ka, Kd, Ks;
  FLT Kp; /* ������� "����������" */

  /* ����������� ������������ */
  FLT Kt;

  /* �������� ����������� */
  INT TexId;
} ds6MATERIAL;

/* ���������� ������� ���������� */
#define DS6_MATERIAL_MAX 300
extern INT DS6_MtlLibSize;
extern ds6MATERIAL DS6_MtlLib[DS6_MATERIAL_MAX];

/* ������� ���������� ��������� � ����������.
 * ���������:
 *   - ��������� � ����������� ���������:
 *       ds6MATERIAL *Mtl;
 * ������������ ��������:
 *   (INT) ���������� ����� ������������ ��������� (0 ��� ������).
 */
INT DS6_MtlAdd( ds6MATERIAL *Mtl );

/* ������� ������ ��������� �� �����.
 * ���������:
 *   - ��� ���������:
 *       CHAR *Name;
 * ������������ ��������:
 *   (INT) ���������� ����� ���������� ��������� (0 ��� ������).
 */
INT DS6_MtlFind( CHAR *Name );

/* ������� �������� ��������� �� ����� (*.MTL).
 * ���������:
 *   - ��� ����� ���������:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (INT) ���������� ����������� ����������.
 */
INT DS6_MtlLoad( CHAR *FileName );


/***
* ������ � �����������
***/

/* ���� ���������� */
typedef enum tagds6PRIM_TYPE
{
  DS6_PRIM_TRIMESH,  /* �������� - ����� ������������� */
  DS6_PRIM_GRID      /* ���������� ����� ������������� (����� triangle strip) */
} ds6PRIM_TYPE;

/* ��� �������� ��������� */
typedef struct tagds6PRIM
{
  ds6PRIM_TYPE Type; /* ��� ��������� (DS6_PRIM_***) */
  INT
      VA,          /* ������ ������ */
      VBuf, IBuf,  /* ������ ������ � �������� */
      NumOfI,      /* ���������� �������� ��� ������ ��������� */
      MtlNo;       /* ����� ��������� �� ���������� */
} ds6PRIM;

/* ��� �������� ���������� ��������� */
typedef struct tagds6UV
{
  FLT U, V; /* ���������� ���������� */
} ds6UV;

/* ������� ������� ���������� ���������.
* ���������:
*   - ����� ������� ���������:
*       FLT U, V;
* ������������ ��������:
*   (ds6UV) ����������� ����������.
*/
__inline ds6UV UVSet(FLT U, FLT V)
{
  ds6UV UV = {U, V};

  return UV;
} /* End of 'UVSet' function */

/* ��� �������� ����� ������� ���������� */
typedef struct tagds6VERTEX
{
  VEC P;    /* ������� ������� */
  ds6UV T;  /* ���������� ���������� ������� */
  VEC N;    /* ������� � ������� */
  COLOR C;  /* ���� ������� */
} ds6VERTEX;

/* ������� �������� ���������.
* ���������:
*   - ��������� �� ��������:
*       ds6PRIM *Prim;
*   - ��� ��������� (DS6_PRIM_***):
*       ds6PRIM_TYPE Type;
*   - ���������� ������ � ��������:
*       INT NoofV, NoofI;
*   - ������ ������:
*       ds6VERTEX *Vertices;
*   - ������ ��������:
*       INT *Indices;
* ������������ ��������: ���.
*/
VOID DS6_PrimCreate( ds6PRIM *Prim, ds6PRIM_TYPE Type, 
                     INT NoofV, INT NoofI, ds6VERTEX *Vertices, INT *Indices );

/* ������� �������� ���������.
* ���������:
*   - ��������� �� ��������:
*       ds6PRIM *Prim;
* ������������ ��������: ���.
*/
VOID DS6_PrimFree( ds6PRIM *Prim );

/* ������� ��������� ���������.
* ���������:
*   - ��������� �� ��������:
*       ds6PRIM *Prim;
* ������������ ��������: ���.
*/
VOID DS6_PrimDraw( ds6PRIM *Prim );

/* ������� �������� ��������� ���������.
* ���������:
*   - ��������� �� ��������:
*       ds6PRIM *Prim;
*   - ����������� �������-�������:
*       VEC Du, Dv;
*   - ���������:
*       INT N, M;
* ������������ ��������:
*   (BOOL) TRUE ��� ������, ����� FALSE.
*/
BOOL DS6_PrimCreatePlane( ds6PRIM *Prim, VEC Du, VEC Dv, INT N, INT M );

/* ������� �������� ��������� �����.
* ���������:
*   - ��������� �� ��������:
*       ds6PRIM *Prim;
*   - ����� �����:
*       VEC �;
*   - ������ �����:
*       FLT R;
*   - ���������:
*       INT N, M;
* ������������ ��������:
*   (BOOL) TRUE ��� ������, ����� FALSE.
*/
BOOL DS6_PrimCreateSphere( ds6PRIM *Prim, VEC C, FLT R, INT N, INT M );

/* ������� �������� ��������������� �������.
* ���������:
*   - ��������� ������� ��� ��������:
*       ds6PRIM *GObj;
*   - ��� �����:
*       CHAR *FileName;
* ������������ ��������:
*   (BOOL) TRUE ��� ������, FALSE �����.
*/
BOOL DS6_PrimLoad( ds6PRIM *GObj, CHAR *FileName );

/***
* ������ � ��������������� ���������
***/

/* �������������� ������ - ����� ���������� */
typedef struct tagds6GEOM
{
  INT NumOfPrimitives; /* ���������� ���������� */
  ds6PRIM *Prims;      /* ������ ���������� */
} ds6GEOM;

/* ������� ���������� ��������� � ��������������� �������.
* ���������:
*   - ��������� �� �������������� ������:
*       ds6GEOM *G;
*   - ��������� �� ����������� ��������:
*       ds6PRIM *Prim;
* ������������ ��������:
*   (INT) ����� ������������ ��������� � ������� (-1 ��� ������).
*/
INT DS6_GeomAddPrim( ds6GEOM *G, ds6PRIM *Prim );

/* ������� ������������ ��������������� �������.
* ���������:
*   - ��������� �� �������������� ������:
*       ds6GEOM *G;
* ������������ ��������: ���.
*/
VOID DS6_GeomFree( ds6GEOM *G );

/* ������� ����������� ��������������� �������.
* ���������:
*   - ��������� �� �������������� ������:
*       ds6GEOM *G;
* ������������ ��������: ���.
*/
VOID DS6_GeomDraw( ds6GEOM *G );

/* ������� �������� ��������������� ������� �� G3D �����.
* ���������:
*   - ��������� �� �������������� ������:
*       ds6GEOM *G;
*   - ��� �����:
*       CHAR *FileName;
* ������������ ��������:
*   (BOOL) TRUE ��� ������, ����� - FALSE.
*/
BOOL DS6_GeomLoad( ds6GEOM *G, CHAR *FileName );

/* ������� �������� ��������.
* ���������:
*   - ��� �����:
*       CHAR *FileName;
* ������������ ��������:
*   (INT ) ������������� OpenGL ��� ��������.
*/
INT DS6_TextureLoad( CHAR *FileName );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
