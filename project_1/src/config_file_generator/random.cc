/* rand example: guess the number */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct location {
int x;
int y;
}carLocation[15];
int main ()
{
  int x, y;
int i, j;
  /* initialize random seed: */
  srand ( time(NULL) );

  /* generate secret number: */
for (i = 0; i < 7; i++ ){
  x = rand() % 360;
  y = (rand() % 4) *120;
  carLocation[i].x = x;
  carLocation[i].y = y;
}

for (i = 7; i < 15; i++ ){
  y = rand() % 360;
  x = (rand() % 4) *120;
  carLocation[i].x = x;
  carLocation[i].y = y;
}
for (j = 0; j<15;j++){
printf("the %d car location: %d, %d\n",j+1, carLocation[j].x, carLocation[j].y );
}

}
