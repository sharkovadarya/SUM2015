/* FILE NAME: T04DETERM.C
 * PROGRAMMER: DS6
 * DATE: 04.06.2015
 * PURPOSE: to calculate determinator.
*/

#include <stdio.h>

#define MAX 30


double A[MAX][MAX], sum = 0;
int p[MAX];
int Parity = 0, N = 3;
   

void Swap( int *A, int *B )
{
  int tmp = *A;

  *A = *B;
  *B = tmp;
}

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
}

double Go( int Pos )
{
  int i, j; 
  double prod = 1;

  if (Pos == N)
  { 
   
    for (i = 0; i < N; i++)
    {
      p[i] = i;
      prod *= A[i][p[i]];
    }
    if (Parity == 0)
      sum += prod;
    else
      sum -= prod;
    return sum;   
  }  

  else
  {
    for (i = Pos; i < N; i++)
    {
      
      if (Pos != i)
        Parity = !Parity; 
      Swap(&p[Pos], &p[i]);
      Go(Pos + 1);  
      if (Pos != i)
        Parity = !Parity; 
      Swap(&p[Pos], &p[i]);          
    }  
  }   
}  

void main( void )
{
  Load("m1.txt");      
  Go(0);
  printf("The determinator of this matrix is %lf.\n", sum);

}