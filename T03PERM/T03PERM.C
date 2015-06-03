/* FILE NAME: T03PERM.C
 * PROGRAMMER: DS6
 * DATE: 03.06.2015
 * PURPOSE: ...
*/

#include <stdio.h>

#define N 5

int p[N];

void Write( int x );

void Swap( int *A, int *B )
{
  int tmp = *A;

  *A = *B;
  *B = tmp;
}

void Go( int Pos )
{
  int i;

  if (Pos == N)
  { 
    for (i = 0; i < N; i++)
      Write(p[i]);
    return;   
  }
  else
  {
    for (i = Pos; i < N; i++)
    {
      Swap(&p[Pos], &p[i]);
      Go(Pos + 1);        
      Swap(&p[Pos], &p[i]);
      /*if (Pos != i)
        Parity = !Parity;      */
    }  
  }
  
}

void Write( int x )
{
  FILE *F;
  static int k = 0;

  if ((F = fopen("a.log", "a")) != NULL)
  {
    k++;
    fprintf(F, "%d ", x);

    if (k % 5 == 0)
      fprintf(F, "\n");
      
    fclose(F);  
  }

}

void Fill( void )
{
  int i;

  for (i = 0; i < N; i++)
    p[i] = i + 1;
}

void main( void )
{
/*  bool Parity FALSE; */

  Fill();  
  Go(0);
}