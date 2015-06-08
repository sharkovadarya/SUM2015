/* FILE NAME: T03PERM.C
 * PROGRAMMER: DS6
 * DATE: 03.06.2015
 * PURPOSE: calculate the number of "changes".
*/

#include <stdio.h>

#define N 5

int p[N];
int Parity = 0;

void Write( int x );


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

/* Permutation function.
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
    for (i = 0; i < N; i++)
      Write(p[i]);  
    for (i = 0; i < N; i++)
      printf("%i", p[i]);
    printf(" - %s\n", Parity ? "odd" : "even");
    return;   
  }  

  else
  {
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
  }  
}


/* Write results to a file function
 * ARGUMENTS:
     - thing that is to be written:
         int x;
 * RETURNS:
     - none. 
*/
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
} /* End of 'Write' function. */ 

/* Fill the array function.
 * ARGUMENTS: 
     - none;
 * RETURNS:
     - none;
*/
void Fill( void )
{
  int i;

  for (i = 0; i < N; i++)
    p[i] = i + 1;
} /* End of 'Fill' function. */

/* Main function.
 * ARGUMENTS: 
     - none;
 * RETURNS:
     - none;
*/     
void main( void )
{
  Fill();  
  Go(0);
} /* End of 'main' function. */