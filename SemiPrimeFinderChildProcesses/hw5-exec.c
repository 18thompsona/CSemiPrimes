#include <stdio.h>
#include <stdlib.h>
#include "tlpi_hdr.h"

void FindSemiPrimes(int lowerLimit, int upperLimit);
int Compare (const void * a, const void * b);
void PrintArray(int *arr, int length);

int main(int argc, char *argv[])
{
    int lowerLimit, upperLimit;

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
    {
        usageErr("%s lowerLimit upperLimit (lowerLimit and upperLimit must be greater than 0)\n", argv[0]);
    }

    lowerLimit = strtol(argv[1], NULL, 10);
    upperLimit = strtol(argv[2], NULL, 10);
    
    if (lowerLimit <= 0 || upperLimit <= 0)
    {
        usageErr("Given limits are invalid\n");
    }

    FindSemiPrimes(lowerLimit, upperLimit);
    return 0;
}

void FindSemiPrimes(int lowerLimit, int upperLimit)
{
    int holder, arrayLength = 0, flag_var;
    int *primeproducts;

    primeproducts = (int*) malloc((upperLimit - lowerLimit)/2 * sizeof(int));

    if (primeproducts == NULL)
    {
        printf("Memory not allocated.\n");
        exit(0);
    }
    
    for(int i=2; i < upperLimit; ++i)
    {
        flag_var=0;

        //Finds a Prime
        for(int j = 2; j <= i/2; ++j)
        {
            if(i % j == 0)
            {
                flag_var=1;
                break;
            }
        }

        if(flag_var == 0)
        {
            for(int a = i; a < upperLimit; ++a)
            {
                flag_var=0;

                //Finds a Prime
                for(int b = 2; b <= a/2; ++b)
                {
                    if(a % b == 0)
                    {
                        flag_var = 1;
                        break;
                    }
                }

                if(flag_var == 0)
                {
                    holder = i * a;
                    if (holder >= lowerLimit && holder <= upperLimit)
                    {
                        primeproducts[arrayLength] = holder;
                        arrayLength++;
                    }
                }
            }
        }
    }
    qsort(primeproducts, arrayLength, sizeof(int), Compare);
    PrintArray(primeproducts, arrayLength);
    free(primeproducts);
}

void PrintArray(int *arr, int length)
{
    for (size_t i = 0; i < length; i++)
    {
        printf("%d ", arr[i]);
    }
}

int Compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}