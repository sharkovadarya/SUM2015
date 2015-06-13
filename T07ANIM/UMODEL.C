/* FILENAME: UMODEL.C
 * PROGRAMMER: DS6
 * PURPOSE: Model unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include "anim.h"


/* MODEL image type */
typedef struct tagds6UNIT_MODEL
{
  DS6_UNIT_BASE_FIELDS;

  ds6GOBJ Model;
} ds6UNIT_MODEL;

/* Animation object initialization function.
 * ARGUMENTS:
 *   - self-pointer - animation object itself:
 *       ds6UNIT_MODEL *Uni;
 *   - animation context pointer:
 *       ds6ANIM *Ani;
 * RETURNS: none.
 */
static VOID DS6_AnimUnitInit( ds6UNIT_MODEL *Uni, ds6ANIM *Ani )
{  
  DS6_RndGObjLoad(&Uni->Model, "cube.object");
} /* End of 'DS6_AnimUnitInit' function */

/* Animation object deinitialization function.
 * ARGUMENTS:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ds6UNIT_MODEL *Uni;
 *   - animation context pointer:
 *       ds6ANIM *Ani;
 * RETURNS: none.
 */
static VOID DS6_AnimUnitClose( ds6UNIT_MODEL *Uni, ds6ANIM *Ani )
{
	DS6_RndGObjFree(&Uni->Model);
} /* End of 'DS6_AnimUnitClose' function */

static VOID DS6_AnimUnitResponse(ds6UNIT_MODEL *Uni, ds6ANIM *Ani)
{}
/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       ds6UNIT_MODEL *Uni;
 *   - ��������� �� �������� ��������:
 *       ds6ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID DS6_AnimUnitRender( ds6UNIT_MODEL *Uni, ds6ANIM *Ani )
{  
  INT i, j;

  DS6_RndMatrView = MatrView(VecSet(8, 8, 8),
                             VecSet(0, 0, 0),
		             VecSet(0, 1, 0));

  if (Ani->KeysClick['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (Ani->KeysClick['Q'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);

  for (i = -2; i < 2; i++)
    for (j = -2; j < 2; j++)
    {
      DS6_RndMatrWorld =
                         MatrMulMatr(MatrMulMatr(MatrMulMatr(
			             MatrTranslate(Ani->JX * 228, Ani->JY * 228, 0),
				     MatrScale(0.1, 0.1, 0.1)),
				     MatrRotateY(30 * Ani->Time + Ani->JR * 180)),
				     MatrTranslate(j * 2.5, 0, i * 2.5 + 228 * Ani->JZ));
      glColor3d(i & 1, j & 1, 1 - ((i & 1) + (j & 1)) / 2);
      DS6_RndGObjDraw(&Uni->Model);
    }
  
} /* End of 'DS6_AnimUnitRender' function */

/* ������� �������� ������� �������� "���".
 * ���������: ���.
 * ������������ ��������:
 *   (ds6UNIT *) ��������� �� ��������� ������ ��������.
 */
ds6UNIT * DS6_UnitModelCreate( VOID )
{
  ds6UNIT_MODEL *Uni;            

  if ((Uni = (VOID *)DS6_AnimUnitCreate(sizeof(ds6UNIT_MODEL))) == NULL)
    return NULL;
  /* ��������� ���� */
  Uni->Init = (VOID *)DS6_AnimUnitInit;
  Uni->Close = (VOID *)DS6_AnimUnitClose;
	Uni->Response = (VOID *)DS6_AnimUnitResponse;
  Uni->Render = (VOID *)DS6_AnimUnitRender;
  return (ds6UNIT *)Uni;
} /* End of 'DS6_UnitModelCreate' function */

/* END OF 'UMODEL.C' FILE */