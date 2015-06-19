/* FILENAME: UMODEL.C
* PROGRAMMER: DS6
* PURPOSE: Model view unit handle module.
* LAST UPDATE: 10.06.2015
*/

#include "anim.h"

/* Тип представления мяча */
typedef struct tagds6UNIT_MODEL
{
  DS6_UNIT_BASE_FIELDS;

  ds6GEOM Model; /* Модель для отображения */
  ds6GEOM Geom; /* Модель для отображения */
  ds6PRIM Pr;
  INT TextId;  /* Id текстуры */
} ds6UNIT_MODEL;

/* Функция инициализации объекта анимации.
* АРГУМЕНТЫ:
*   - указатель на "себя" - сам объект анимации:
*       ds6UNIT_MODEL *Uni;
*   - указатель на контекст анимации:
*       ds6ANIM *Ani;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
static VOID DS6_AnimUnitInit(ds6UNIT_MODEL *Uni, ds6ANIM *Ani)
{
  ds6VERTEX V[] =
  {
    { { 0, 0, 0 }, { 0, 0 }, { 0, 0, 1 }, { 1, 1, 1, 1 } },
    { { 1, 0, 0 }, { 5, 0 }, { 0, 0, 1 }, { 1, 0, 1, 1 } },
    { { 0, 1, 0 }, { 0, 5 }, { 0, 0, 1 }, { 1, 1, 0, 1 } },
    { { 1, 1, 0 }, { 5, 5 }, { 0, 0, 1 }, { 1, 1, 0, 1 } },
  };
  INT I[] = { 0, 1, 2, 2, 1, 3 };
  BYTE txt[2][2][3] =
  {
    { { 255, 255, 255 }, { 0, 0, 0 } },
    { { 0, 0, 0 }, { 255, 255, 255 } }
  };

  /* загружаем текстуру */
  Uni->TextId = DS6_TextureLoad("ElphSkin.BMP");

  
  DS6_GeomLoad(&Uni->Model, "elephant.g3d");
  DS6_RndPrimMatrConvert = MatrRotateX(-90);
  //DS6_GeomLoad(&Uni->Geom, "X6.G3D");
} /* End of 'DS6_AnimUnitInit' function */

/* Функция деинициализации объекта анимации.
* АРГУМЕНТЫ:
*   - указатель на "себя" - сам объект анимации:
*       ds6UNIT_MODEL *Uni;
*   - указатель на контекст анимации:
*       ds6ANIM *Ani;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
static VOID DS6_AnimUnitClose(ds6UNIT_MODEL *Uni, ds6ANIM *Ani)
{
  DS6_GeomFree(&Uni->Model);
  DS6_GeomFree(&Uni->Geom);
  DS6_PrimFree(&Uni->Pr);
} /* End of 'DS6_AnimUnitClose' function */

/* Функция построения объекта анимации.
* АРГУМЕНТЫ:
*   - указатель на "себя" - сам объект анимации:
*       ds6UNIT_MODEL *Uni;
*   - указатель на контекст анимации:
*       ds6ANIM *Ani;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
static VOID DS6_AnimUnitRender(ds6UNIT_MODEL *Uni, ds6ANIM *Ani)
{
  INT i, j;

  DS6_RndMatrView = MatrView(VecSet(8 + DS6_GamePadShiftZ, 8 + DS6_GamePadShiftZ, 8 + DS6_GamePadShiftZ),
                             VecSet(DS6_GamePadShiftY, DS6_GamePadShiftY, DS6_GamePadShiftY),
                             VecSet(DS6_GamePadShiftX, 1 + DS6_GamePadShiftX, DS6_GamePadShiftX));

  if (Ani->KeysClick['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (Ani->KeysClick['Q'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);

  for (i = -1; i < 1; i++)
    for (j = -1; j < 1; j++)
    {
      DS6_RndMatrWorld =
        MatrMulMatr(MatrMulMatr(MatrMulMatr(
        MatrTranslate(Ani->JX * 1488, Ani->JY * 1488, 0),
        MatrScale(0.001, 0.001, 0.001)),
        MatrRotateY(30 * Ani->Time + Ani->JR * 228)),
        MatrTranslate(j * 22.8, 0, i * 22.8 + 228 * Ani->JZ));
      glColor3d(i & 1, j & 1, 1 - ((i & 1) + (j & 1)) / 2);
      DS6_GeomDraw(&Uni->Model);
    }
  DS6_RndMatrWorld = MatrRotateY(30 * Ani->Time);
  //MatrMulMatr(MatrRotateZ(30 * sin(Ani->Time * 3.0)), MatrRotateY(30 * Ani->Time));
  DS6_GeomDraw(&Uni->Geom);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId);
  DS6_PrimDraw(&Uni->Pr);
} /* End of 'DS6_AnimUnitRender' function */

/* Функция создания объекта анимации "модель".
* АРГУМЕНТЫ: Нет.
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
*   (ds6UNIT *) указатель на созданный объект анимации.
*/
ds6UNIT * DS6_UnitModelCreate(VOID)
{
  ds6UNIT_MODEL *Uni;

  if ((Uni = (VOID *)DS6_AnimUnitCreate(sizeof(ds6UNIT_MODEL))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)DS6_AnimUnitInit;
  Uni->Close = (VOID *)DS6_AnimUnitClose;
  Uni->Render = (VOID *)DS6_AnimUnitRender;
  return (ds6UNIT *)Uni;
} /* End of 'DS6_UnitModelCreate' function */

/* END OF 'UMODEL.C' FILE */
