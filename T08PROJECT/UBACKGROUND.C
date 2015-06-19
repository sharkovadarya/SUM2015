/* FILENAME: UBACKGROUND.C
* PROGRAMMER: DS6
* PURPOSE: Background view unit handle module.
* LAST UPDATE: 16.06.2015
*/

#include "anim.h"

/* Background image type */
typedef struct tagds6UNIT_BACKGROUND
{
  DS6_UNIT_BASE_FIELDS;

  ds6PRIM Pr;   
  INT TextId;  /* Id текстуры */
} ds6UNIT_BACKGROUND;

/* Функция инициализации объекта анимации.
* АРГУМЕНТЫ:
*   - указатель на "себя" - сам объект анимации:
*       ds6UNIT_BACKGROUND *Uni;
*   - указатель на контекст анимации:
*       ds6ANIM *Ani;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
static VOID DS6_AnimUnitInit(ds6UNIT_BACKGROUND *Uni, ds6ANIM *Ani)
{
  ds6VERTEX V[] =
  {
    { { -1, -1, 0 }, { 0, 0 }, { 0, 0, 1 }, { 1, 1, 1, 1 } },
    { { 1, -1, 0 }, { 1, 0 }, { 0, 0, 1 }, { 1, 0, 1, 1 } },
    { { -1, 1, 0 }, { 0, 1 }, { 0, 0, 1 }, { 1, 1, 0, 1 } },
    { { 1, 1, 0 }, { 1, 1 }, { 0, 0, 1 }, { 1, 1, 0, 1 } },
  };
  INT I[] = { 0, 1, 2, 2, 1, 3 };
  BYTE txt[2][2][3] =
  {
    { { 255, 255, 255 }, { 0, 0, 0 } },
    { { 0, 0, 0 }, { 255, 255, 255 } }
  };
  ds6MATERIAL Mtl = DS6_MtlLib[0];

  /* загружаем текстуру */
  DS6_PrimCreate(&Uni->Pr, DS6_PRIM_TRIMESH, 4, 6, V, I);
  Mtl.TexId = DS6_TextureLoad("background.bmp");
  Uni->Pr.MtlNo = Uni->TextId = DS6_MtlAdd(&Mtl);

} /* End of 'DS6_AnimUnitInit' function */

/* Функция деинициализации объекта анимации.
* АРГУМЕНТЫ:
*   - указатель на "себя" - сам объект анимации:
*       ds6UNIT_BACKGROUND *Uni;
*   - указатель на контекст анимации:
*       ds6ANIM *Ani;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
static VOID DS6_AnimUnitClose(ds6UNIT_BACKGROUND *Uni, ds6ANIM *Ani)
{
  DS6_PrimFree(&Uni->Pr);
} /* End of 'DS6_AnimUnitClose' function */

/* Функция построения объекта анимации.                                  
* АРГУМЕНТЫ:
*   - указатель на "себя" - сам объект анимации:
*       ds6UNIT_BACKGROUND *Uni;
*   - указатель на контекст анимации:
*       ds6ANIM *Ani;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
static VOID DS6_AnimUnitRender(ds6UNIT_BACKGROUND *Uni, ds6ANIM *Ani)
{
  DS6_RndMatrView = MatrView(VecSet(8, 8, 8),
                             VecSet(0, 0, 0),
                             VecSet(0, 1, 0)); 

  DS6_RndMatrWorld = MatrMulMatr(MatrIdentity(), MatrScale(1, 1, 1));

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId);
  DS6_BackgroundDraw(&Uni->Pr);  
} /* End of 'DS6_AnimUnitRender' function */

/* Функция создания объекта анимации "модель".
* АРГУМЕНТЫ: Нет.
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
*   (ds6UNIT *) указатель на созданный объект анимации.
*/
ds6UNIT * DS6_UnitBackgroundCreate( VOID )
{
  ds6UNIT_BACKGROUND *Uni;

  if ((Uni = (VOID *)DS6_AnimUnitCreate(sizeof(ds6UNIT_BACKGROUND))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)DS6_AnimUnitInit;
  Uni->Close = (VOID *)DS6_AnimUnitClose;
  Uni->Render = (VOID *)DS6_AnimUnitRender;
  return (ds6UNIT *)Uni;
} /* End of 'DS6_UnitBackgroundCreate' function */

/* END OF 'UBACKGROUND.C' FILE */
