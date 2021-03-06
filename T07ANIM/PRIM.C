/* FILENAME: PRIM.C
* PROGRAMMER: DS6
* PURPOSE: Primitive handle module.
* LAST UPDATE: 13.06.2015
*/

#include "anim.h"
#include "render.h"

/* ������� ��������� ��������� ��� �������� */
MATR DS6_RndPrimMatrConvert = DS6_UNIT_MATR;

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
*   (BOOL) TRUE ��� ������, ����� FALSE.
*/
VOID DS6_PrimCreate(ds6PRIM *Prim, ds6PRIM_TYPE Type,
                    INT NoofV, INT NoofI, ds6VERTEX *Vertices, INT *Indices)
{
  Prim->Type = Type;
  Prim->NumOfI = NoofI;
  /* ������� ������ OpenGL */
  glGenVertexArrays(1, &Prim->VA);
  glGenBuffers(1, &Prim->VBuf);
  glGenBuffers(1, &Prim->IBuf);

  /* ������ �������� ������ ������ */
  glBindVertexArray(Prim->VA);
  /* ������ �������� ����� ������ */
  glBindBuffer(GL_ARRAY_BUFFER, Prim->VBuf);
  /* ������� ������ */
  glBufferData(GL_ARRAY_BUFFER, sizeof(ds6VERTEX) * NoofV, Vertices, GL_STATIC_DRAW);
  /* ������ �������� ����� �������� */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Prim->IBuf);
  /* ������� ������ */
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NoofI, Indices, GL_STATIC_DRAW);

  /* ������ ������� ������ */
  /*                    layout,
   *                       ���������� ���������,
   *                          ���,
   *                                    ���� �� �����������,
   *                                           ������ � ������ ������ �������� ������,
   *                                                           �������� � ������ �� ������ ������ */
  glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(ds6VERTEX), (VOID *)0); /* ������� */
  glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(ds6VERTEX), (VOID *)sizeof(VEC)); /* �������� */
  glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(ds6VERTEX), (VOID *)(sizeof(VEC) + sizeof(ds6UV))); /* ������� */
  glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(ds6VERTEX), (VOID *)(sizeof(VEC) * 2 + sizeof(ds6UV))); /* ���� */

  /* �������� ������ ��������� (layout) */
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);

  /* ����������� �� ������� ������ */
  glBindVertexArray(0);
} /* End of 'DS6_PrimCreate' function */

/* ������� �������� ���������.
* ���������:
*   - ��������� �� ��������:
*       ds6PRIM *Prim;
* ������������ ��������: ���.
*/
VOID DS6_PrimFree(ds6PRIM *Prim)
{
  /* ������ �������� ������ ������ */
  glBindVertexArray(Prim->VA);
  /* "���������" ������ */
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Prim->VBuf);
  glDeleteBuffers(1, &Prim->IBuf);
  /* ������ ���������� ������ ������ */
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Prim->VA);

  memset(Prim, 0, sizeof(ds6PRIM));
} /* End of 'DS6_PrimFree' function */

/* ������� ��������� ���������.
* ���������:
*   - ��������� �� ��������:
*       ds6PRIM *Prim;
* ������������ ��������: ���.
*/
VOID DS6_PrimDraw( ds6PRIM *Prim )
{
  INT loc;
  MATR M;

  DS6_RndMatrWorldViewProj = MatrMulMatr(MatrMulMatr(DS6_RndMatrWorld, DS6_RndMatrView), DS6_RndMatrProj);

  /* ��������� ��� ��������, ���� ��� �������� */
  glLoadMatrixf(DS6_RndMatrWorldViewProj.A[0]);

  /* ������ ������������ */
  glBindVertexArray(Prim->VA);
  glUseProgram(DS6_RndProg);

  loc = glGetUniformLocation(DS6_RndProg, "MatrWorld");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, DS6_RndMatrWorld.A[0]);
  loc = glGetUniformLocation(DS6_RndProg, "MatrView");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, DS6_RndMatrView.A[0]);
  loc = glGetUniformLocation(DS6_RndProg, "MatrProj");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, DS6_RndMatrProj.A[0]);
  loc = glGetUniformLocation(DS6_RndProg, "MatrWVP");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, DS6_RndMatrWorldViewProj.A[0]);

  M = MatrTranspose(MatrInverse(MatrMulMatr(DS6_RndMatrWorld, DS6_RndMatrView)));
  loc = glGetUniformLocation(DS6_RndProg, "MatrWVInverse");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);

  M = MatrTranspose(MatrInverse(DS6_RndMatrWorld));
  loc = glGetUniformLocation(DS6_RndProg, "MatrWInverse");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);

  M = MatrMulMatr(DS6_RndMatrWorld, DS6_RndMatrView);
  loc = glGetUniformLocation(DS6_RndProg, "MatrWV");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);

  loc = glGetUniformLocation(DS6_RndProg, "Time");
  if (loc != -1)
    glUniform1f(loc, DS6_Anim.Time);

 /* ���������� ��������� */
  loc = glGetUniformLocation(DS6_RndProg, "Ka");
  if (loc != -1)
    glUniform3fv(loc, 1, &DS6_MtlLib[Prim->MtlNo].Ka.X);
  loc = glGetUniformLocation(DS6_RndProg, "Kd");
  if (loc != -1)
    glUniform3fv(loc, 1, &DS6_MtlLib[Prim->MtlNo].Kd.X);
  loc = glGetUniformLocation(DS6_RndProg, "Ks");
  if (loc != -1)
    glUniform3fv(loc, 1, &DS6_MtlLib[Prim->MtlNo].Ks.X);
  loc = glGetUniformLocation(DS6_RndProg, "Kp");
  if (loc != -1)
    glUniform1f(loc, DS6_MtlLib[Prim->MtlNo].Kp);
  loc = glGetUniformLocation(DS6_RndProg, "Kt");
  if (loc != -1)
    glUniform1f(loc, DS6_MtlLib[Prim->MtlNo].Kt);

  loc = glGetUniformLocation(DS6_RndProg, "IsTextureUse");
  if (DS6_MtlLib[Prim->MtlNo].TexId == 0)
    glUniform1f(loc, 0);
  else
  {
    glUniform1f(loc, 1);
    glBindTexture(GL_TEXTURE_2D, DS6_MtlLib[Prim->MtlNo].TexId);
  }

  glPrimitiveRestartIndex(0xFFFFFFFF);
  if (Prim->Type == DS6_PRIM_GRID)
    glDrawElements(GL_TRIANGLE_STRIP, Prim->NumOfI, GL_UNSIGNED_INT, NULL);
  else
    glDrawElements(GL_TRIANGLES, Prim->NumOfI, GL_UNSIGNED_INT, NULL);

  glUseProgram(0);
  glBindVertexArray(0);

} /* End of 'DS6_PrimDraw' function */

/* END OF 'PRIM.C' FILE */
