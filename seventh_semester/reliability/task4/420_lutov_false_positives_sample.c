#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

int min(int p1, int p2)
{
    return(p1 < p2 ? p1 : p2);
}

void mem_copy(void *PMemDest, const void *PMemSrc, unsigned int count)
{
    memcpy(PMemDest,PMemSrc,count );
}


int test(int x)
{
        int shift;

    int arr1[] = { 16, 32, 64, 128, 256, 512, 1024 };

    int index = rand()%7;

    int *p_arr2;

    p_arr2 = (int *)malloc(sizeof(int) * arr1[index]);

    shift = arr1[index] - 7;

    memcpy(p_arr2 - shift, arr1, 7);
    printf("func %d \n",x);

    return(1);
}


void mem_manage_func(int DesVar)
{
    int Size1 = 100;
    int Size2 = rand() % 50;

    float *PArr1 = (float *)malloc(sizeof(float) * Size1);
    int *PArr2 = (int *)malloc(sizeof(int) * Size2);


    if (DesVar > 15)
    {
        mem_copy(PArr2, PArr1, min(Size1, Size2));
    }
    else    if (DesVar > 8)
            {
                mem_copy(PArr2, PArr1, Size1);
            }
            else
            {
                mem_copy(PArr2, PArr1, Size2);
            }
}


void print_string(char *str, int StrLen)
{
        char FrmStr[24];

        sprintf(FrmStr, "String for print: <%%%ds>", StrLen);

        printf((const char *)FrmStr, str);
}

long double count_func(float arg1, unsigned int arg2, double arg3, int arg4)
{
    long double res;
    int *i;
    double *arr;

    arr = (double *)malloc(sizeof(double) * 3);


    for (*i = 0; *i < (arg4 * arg4) % 1000; *i++)
    {
        if (*i % 10 < 5)
        {
            res *= (arg1 / arg3) * arr[*i];
        }
        else
        {
            res /= (long double)acos((double)(log10(abs(arg3)) * exp(abs(arg1))));

            res += *(arr + *i);
        }

        free(arr);
    }

    test(1342);

    return(res);
}



void recurs_func(unsigned int RecArg, char *ExternStr)
{
    char *LocalStr;
    int in_var;
    char str[10];
    FILE *fin;
    int *p_arr;
    int buf[10];
    int i;


    LocalStr = (char *)malloc(sizeof(char) * 20);

    for (i = 0; i < 20; i++)
    {
        LocalStr[i] = '0';
    }

    scanf("%10s", &LocalStr[strlen(LocalStr) + 1]);

    mem_copy( LocalStr, ExternStr, rand()%25 );

    if ( RecArg%10 < 1 )
    {
        print_string( LocalStr, 15 );
        print_string( ExternStr, 17 );
        count_func( (float) RecArg, RecArg, (double) RecArg, (int) RecArg );
    }
    else
    {
        fin = fopen("input.file", "r");
        scanf(" %d %s", &in_var, str);

        p_arr += in_var;

        printf("%d", p_arr[0]);


        for ( i = 0; i < in_var; i++ )
        {
            printf( "%d", buf [i] );
        }

        recurs_func((unsigned int) rand(), LocalStr);
    }
}



void manage_func(int IterNum,
                 void (*iter_func)(unsigned int RecArg, char *ExternStr),
                 long double (*math_func)(float arg1,
                 unsigned int arg2,
                 double arg3,
                 int arg4))
{
    int i;
    long double res;

    for (i = 0; i < IterNum; i++)
    {
        res = (*math_func)(i/IterNum, abs(i), res, IterNum - i);
    }

    if (res > 32767)
    {
        (*iter_func)(5, "Start string");

        printf( "Some string: %d", res );

        mem_manage_func( 10 );
    }
    else
    {
        printf( "Other result: %s", res );

        mem_manage_func(5);
    }


    return;
}



int main(void)
{

    void (*iter_func)(unsigned int RecArg, char *ExternStr);
    long double (*math_func)(float arg1,
                             unsigned int arg2,
                             double arg3,
                             int arg4);


    iter_func=recurs_func;
    math_func=count_func;

    manage_func( 1000, iter_func, math_func );


    return 0;
}
