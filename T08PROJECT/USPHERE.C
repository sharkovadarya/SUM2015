/* FILENAME: USPHERE.C
* PROGRAMMER: DS6
* PURPOSE: Sphere view unit handle module.
* LAST UPDATE: 16.06.2015
*/

#include <stdlib.h>
#include "anim.h"
#include "render.h"
#include "noise.h"

#define Q 0.001

/* Sphere image type */
typedef struct tagds6UNIT_SPHERE
{
  DS6_UNIT_BASE_FIELDS;

  ds6PRIM Pr;
  INT TextId;  /* Id текстуры */
  VEC Velocity[L]; /* moving sphere velocity */
  INT Radius; /* Sphere radius */
  VEC Centre; /* Sphere centre */
  VEC Position[L]; /* Sphere centre position */
  //VEC Save[L]; /* Position before collision save */
} ds6UNIT_SPHERE;

/* Функция инициализации объекта анимации.
* АРГУМЕНТЫ:
*   - указатель на "себя" - сам объект анимации:
*       ds6UNIT_SPHERE *Uni;
*   - указатель на контекст анимации:
*       ds6ANIM *Ani;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
static VOID DS6_AnimUnitInit( ds6UNIT_SPHERE *Uni, ds6ANIM *Ani )
{
  ds6MATERIAL Mtl = DS6_MtlLib[0];
  Uni->Radius = 8;
  Uni->Centre = VecSet(-50, -25, -50);
  Uni->Position[1] = VecSet(0, 0, 0);
  Uni->Position[0] = VecSet(0, -70, 0);
  Uni->Velocity[1] = VecSet(0, -1.5, 0);
  Uni->Velocity[0] = VecSet(0, 2.5, 0);
  Uni->Position[2] = VecSet(0, 33, 0);
  Uni->Position[3] = VecSet(0, -127, 0);
  Uni->Velocity[2] = VecSet(0, 0, 0);
  Uni->Velocity[3] = VecSet(0, 0, 0);

  /* загружаем текстуру */ 
  DS6_PrimCreateSphere(&Uni->Pr, Uni->Centre, Uni->Radius, 30, 30);  
  Mtl.TexId = DS6_TextureLoad("glass.bmp");
  Uni->Pr.MtlNo = Uni->TextId = DS6_MtlAdd(&Mtl);
} /* End of 'DS6_AnimUnitInit' function */

/* Функция деинициализации объекта анимации.
* АРГУМЕНТЫ:
*   - указатель на "себя" - сам объект анимации:
*       ds6UNIT_SPHERE *Uni;
*   - указатель на контекст анимации:
*       ds6ANIM *Ani;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
static VOID DS6_AnimUnitClose( ds6UNIT_SPHERE *Uni, ds6ANIM *Ani )
{
  DS6_PrimFree(&Uni->Pr);
} /* End of 'DS6_AnimUnitClose' function */

static VOID DS6_AnimUnitResponse( ds6UNIT_SPHERE *Uni, ds6ANIM *Ani )
{ 
  INT i;

  for (i = 0; i < L; i++)
    Uni->Position[i] = VecAddVec(Uni->Position[i], VecMulNum(Uni->Velocity[i], 0.5));
  if (SphereCollisionDetection(Uni->Radius, Uni->Radius, Uni->Position[0], Uni->Position[1]) || 
      SphereCollisionDetection(Uni->Radius, Uni->Radius, Uni->Position[1], Uni->Position[2]) || 
      SphereCollisionDetection(Uni->Radius, Uni->Radius, Uni->Position[0], Uni->Position[3]))
    SphereCollisionResponse(Uni);
  //if (Ani->Time > 35)
    //DS6_AnimDoExit();
} /* End of 'DS6_AnimUnitResponse' function */

/* Функция построения объекта анимации.                                  
* АРГУМЕНТЫ:
*   - указатель на "себя" - сам объект анимации:
*       ds6UNIT_SPHERE *Uni;
*   - указатель на контекст анимации:
*       ds6ANIM *Ani;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
static VOID DS6_AnimUnitRender( ds6UNIT_SPHERE *Uni, ds6ANIM *Ani )
{
  INT i;

  DS6_RndMatrView = MatrView(VecSet(8, 8, 8),
                             VecSet(0, 0, 0),
                             VecSet(0, 1, 0)); 

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId);

  for (i = 0; i < L; i++)
  {
    DS6_RndMatrWorld = MatrTranslate(Uni->Position[i].X, Uni->Position[i].Y, Uni->Position[i].Z);
    DS6_PrimDraw(&Uni->Pr);
  }
} /* End of 'DS6_AnimUnitRender' function */

/* Функция создания объекта анимации "модель".
* АРГУМЕНТЫ: Нет.
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
*   (ds6UNIT *) указатель на созданный объект анимации.
*/
ds6UNIT * DS6_UnitSphereCreate( VOID )
{
  ds6UNIT_SPHERE *Uni;

  if ((Uni = (VOID *)DS6_AnimUnitCreate(sizeof(ds6UNIT_SPHERE))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)DS6_AnimUnitInit;
  Uni->Close = (VOID *)DS6_AnimUnitClose;
  Uni->Response = (VOID *)DS6_AnimUnitResponse;
  Uni->Render = (VOID *)DS6_AnimUnitRender;
  return (ds6UNIT *)Uni;
} /* End of 'DS6_UnitSphereCreate' function */

/* Sphere collision detection function.
 * ARGUMENTS:
 *   - Sphere radii:
 *        INT Radius1, INT Radius2;
 *   - Sphere centres:
 *        VEC Centre1, VEC Centre2;
 * RETURNS:
 *   (BOOL) TRUE if a collision happened, FALSE otherwise.
 */
BOOL SphereCollisionDetection( INT Radius1, INT Radius2, VEC Centre1, VEC Centre2 )
{
  VEC Delta = VecSubVec(Centre1, Centre2);
  FLT Sum = Radius1 + Radius2,
      Distance = VecLen(Delta);

  if (Distance <= Sum)
    return TRUE;
  return FALSE;
} /*End of 'SphereCollisionDetection' function.*/

/* Sphere collision response function.
 * ARGUMENTS:
 *  - Sphere unit:
 *      ds6UNIT_SPHERE *Uni;
 * RETURNS: None.
*/
INT SphereCollisionResponse( ds6UNIT_SPHERE *Uni )
{
  Uni->Velocity[0] = VecMulNum(Uni->Velocity[0], -1);
  Uni->Velocity[1] = VecMulNum(Uni->Velocity[1], -1);
  Uni->Position[0].Y = Uni->Position[0].Y - Uni->Radius * Q;
  Uni->Position[1].Y = Uni->Position[1].Y + Uni->Radius * Q;
  return 0;
} /*End of 'SphereCollisionResponse' function.*/

/* END OF 'USPHERE.C' FILE */
