#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>
#include "tlpi_hdr.h"

void FindSemiPrime(int lowerLimit, int upperLimit);
void PrintArray(int *arr, int length);
int compare (const void * a, const void * b);

int main(int argc, char *argv[])
{
    int lowerLimit, upperLimit, children, rangeAmount, remain, jobDone = 0;
    int oddNumFlag = 0;
    char passLower[50], passUpper[50];

    if (argc != 4 || strcmp(argv[1], "--help") == 0)
    {
        usageErr("%s Processes lowerLimit upperLimit (Processes, lowerLimit, upperLimit must be greater than 0)\n", argv[0]);
    }

    children = strtol(argv[1], NULL, 10);
    lowerLimit = strtol(argv[2], NULL, 10);
    upperLimit = strtol(argv[3], NULL, 10);

    if (children < 1)
    {
        usageErr("Must have at least one process\n");
    }
    if (lowerLimit <= 0 || upperLimit <= 0)
    {
        usageErr("Given limits are invalid\n");
    }

    rangeAmount = (upperLimit-lowerLimit)/children;
    remain = (upperLimit-lowerLimit)%children;

    if (remain)
    {
        if (children > (upperLimit-lowerLimit+1))
        {
            children = upperLimit-lowerLimit+1;
        }
            oddNumFlag = 1;
    }

    upperLimit = lowerLimit + rangeAmount;
    sprintf(passLower, "%i", lowerLimit);
    sprintf(passUpper, "%i", upperLimit);

    for (int i = 0; i < children; i++)
    {
        if (jobDone == 0)
        {
            //Creates children to do task, does create a race condition but this does not matter.
            switch (fork())
            {
            case -1:
                errExit("Child process could not be created");
                break;
            case 0:
                execl("hw5-exec", "hw5-exec", passLower, passUpper, NULL);
                printf("fail\n");
                errExit("execv");
                break;
            
            default:
                //Used When division of labor is not equal
                if (oddNumFlag == 1)
                {
                    remain--;
                    lowerLimit = upperLimit+1;
                    upperLimit = upperLimit + rangeAmount + 1;
                    sprintf(passLower, "%i", lowerLimit);
                    sprintf(passUpper, "%i", upperLimit);
                    if (remain == 0)
                    {
                        oddNumFlag = 0;
                    }
                }
                //Divides the labor when equal
                else
                {
                    lowerLimit = upperLimit+1;
                    upperLimit = upperLimit + rangeAmount;
                    sprintf(passLower, "%i", lowerLimit);
                    sprintf(passUpper, "%i", upperLimit);
                }
                break;
            }
        }
    }

    //Stops the child processes after they are done.
    for (int p = 0; p < children; p++)
    {
        wait(NULL);
    }

    printf("\n");

    return 0;
}

void FindSemiPrime(int lowerLimit, int upperLimit)
{
    int flag_var, holder, arrayLength = 0;
    int *primeproducts;

    primeproducts = (int*) malloc((upperLimit - lowerLimit)/2 * sizeof(int));
    if (primeproducts == NULL)
    {
        errExit("Memory could not be allocated");
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
                        // printf("%d ", holder);
                        // printf("Hello \n");
                    }
                }
            }
        }
    }

    qsort(primeproducts, arrayLength, sizeof(int), compare);
    PrintArray(primeproducts, arrayLength);
    free(primeproducts);

    return;
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