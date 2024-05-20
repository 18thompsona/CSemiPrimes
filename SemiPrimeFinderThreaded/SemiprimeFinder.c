#include <stdio.h>
#include <stdlib.h>
#include "tlpi_hdr.h"

void *FindSemiPrime(void *arg);
void PrintArray(int *arr, int length);
int compare (const void * a, const void * b);

struct ranges
{
    int upper, lower;
};



int *primeproducts;
int arrayLength = 0;




int main(int argc, char *argv[])
{
    int lowerLimit, upperLimit, threads, rangeAmount, remain;
    int oddNumFlag = 0;

    
    

    if (argc != 4 || strcmp(argv[1], "--help") == 0)
    {
        usageErr("%s Threads lowerLimit upperLimit (Threads, lowerLimit, upperLimit must be greater than 0)\n", argv[0]);
    }

    threads = strtol(argv[1], NULL, 10);
    lowerLimit = strtol(argv[2], NULL, 10);
    upperLimit = strtol(argv[3], NULL, 10);

    if (threads < 1)
    {
        usageErr("Must have at least one process\n");
    }
    if (lowerLimit <= 0 || upperLimit <= 0)
    {
        usageErr("Given limits are invalid\n");
    }

    primeproducts = (int*) malloc((upperLimit - lowerLimit)/2 * sizeof(int));
    if (primeproducts == NULL)
    {
        errExit("Memory could not be allocated");
    }

    pthread_t threadcount[threads];
    struct ranges upperlower[threads];



    rangeAmount = (upperLimit-lowerLimit)/threads;
    remain = (upperLimit-lowerLimit)%threads;

    if (remain)
    {
        if (threads > (upperLimit-lowerLimit+1))
        {
            threads = upperLimit-lowerLimit+1;
        }
            oddNumFlag = 1;
    }

    upperLimit = lowerLimit + rangeAmount;

    for (int i = 0; i < threads; i++)
    {
        int err;
        upperlower[i].lower=lowerLimit;
        upperlower[i].upper=upperLimit;

        //Creates threads to do task, does create a race condition but this does not matter.
        err = pthread_create(&threadcount[i], NULL, FindSemiPrime, &upperlower[i]);
        if (err != 0)
        {
            errExit("pthread");
        }
        
    
        //Used When division of labor is not equal
        if (oddNumFlag == 1)
        {
            remain--;
            lowerLimit = upperLimit+1;
            upperLimit = upperLimit + rangeAmount + 1;
            if (remain == 0)
            {
                oddNumFlag = 0;
            }
        }
        else
        {
            lowerLimit = upperLimit+1;
            upperLimit = upperLimit + rangeAmount;
        }
    }

    for (int i = 0; i < threads; i++)
    {
        pthread_join(threadcount[i], NULL);
    }

    qsort(primeproducts, arrayLength, sizeof(int), compare);
    PrintArray(primeproducts, arrayLength);
    free(primeproducts);

    printf("\n");

    return 0;
}

void *FindSemiPrime(void *arg)
{
    int flag_var, holder;
    struct ranges *temp = (struct ranges *)arg;

    for(int i=2; i < temp->upper; ++i)
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
            for(int a = i; a < temp->upper; ++a)
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
                    if (holder >= temp->lower && holder <= temp->upper)
                    {
                        primeproducts[arrayLength] = holder;
                        arrayLength++;
                    }
                }
            }
        }
    }
    return NULL;
}

void PrintArray(int *arr, int length)
{
    for (size_t i = 0; i < length; i++)
    {
        printf("%d ", arr[i]);
    }
}

int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}