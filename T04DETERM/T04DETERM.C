/* FILE NAME: T04DETERM.C
 * PROGRAMMER: DS6
 * DATE: 04.06.2015
 * PURPOSE: to calculate matrix determinator.
*/

#include <stdio.h>

#define MAX 30


double A[MAX][MAX], sum = 0;
int p[MAX];
int Parity = 0, N = 3;
   

/* Swap values function.
 * ARGUMENTS:
     - values that are to be swapped:
         int *A, int *B;
 * RETURNS:
     - none.
*/
void Swap( int *A, int *B )
{
  int tmp = *A;

  *A = *B;
  *B = tmp;
} /* End of 'Swap' function. */

/* Load matrix from file function.
 * ARGUMENTS:
     - file name:
         char *FileName;
 * RETURNS:
     - none;
 */

void Load( char *FileName )
{
  int i, j;
  FILE *F; 

  if ((F = fopen(FileName, "r")) != NULL)
  {
    fscanf(F, "%d", &N);
    for (i = 0; i < N; i++)
      for (j = 0; j < N; j++)
        fscanf(F, "%lf", &A[i][j]);
    fclose(F);
  }
} /* End of 'Load' function. */

/* Matrix determinator calculation function.
 * ARGUMENTS:
     - position:
         int Pos;
 * RETURNS:
     - none.
*/

void Go( int Pos )
{
  int i, x, save;

  if (Pos == N)
  {
    double prod = 1;

    for (i = 0; i < N; i++)
    {
      prod *= A[i][p[i]];

      if (Parity == 0)
        sum += prod;
      else
        sum -= prod;
    }
    return;
  }

  save = Parity;
  Go(Pos + 1);
  for (i = Pos + 1; i < N; i++)
  {
    Parity = !Parity;
    Swap(&p[Pos], &p[i]);
    Go(Pos + 1);
  }

  Parity = save;
  x = p[Pos];

  for (i = Pos + 1; i < N; i++)
    p[i - 1] = p[i];
  p[N - 1] = x;
} /* End of 'Go' function */

/* Main function;
 * ARGUMENTS:
     - none;
 * RETURNS:
     - none.
*/

void main( void )
{
  int i;
  
  for (i = 0; i < N; i++)
    p[i] = i + 1;

  Load("m.txt");      
  Go(0);
  printf("The determinator of this matrix is %lf.\n", sum);
} /* End of 'main' function */