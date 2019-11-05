#include <stdio.h> 
int main()
{
  int i, j, x,n = 0;
  for (i = 3; i <= 20000; i++)
   {
       x = 0;
       for (j = 2; j < i; j++)
            if (i % j == 0)
                  x = 1;
         if (x == 0)
           {
              n++;
              printf("%d ", i);
              if (n%5==0)
                  printf("\n");

            }
    }return 0;
}
