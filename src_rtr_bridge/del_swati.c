#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


/**********************************************
 *          Supporting functions
 * *******************************************/
int arr_max(int *arr, uint32_t sz)
{
    int ret = *arr;

    for(int i=1;i<sz;i++)
    {
        (ret > arr[i]) ?  : (ret=arr[i]);
    }
    return ret;
}

/*
[ 325 123 459 ]
*/
void print_sumOFprv2(int *arr, uint32_t sz)
{
    int *p2=NULL,*p1=NULL;
    p1 = arr;
    p2 = p1+1;
    arr +=2;
    for(int i=0;i<sz;i++)
    {
        if(arr[i] == (p1[i] + p2[i]) )
        {
            printf("%d + %d = %d\n",p1[i],p2[i],arr[i]);
        }
    }
}

void print_adjacent_num(int *arr, uint32_t sz)
{
    int *p = arr;
    arr += 1;
    int tmp = 0;
    for(uint32_t i=0;i<sz; i++)
    {
        tmp = arr[i] - p[i];
        if( (tmp==1) || (tmp == -1) )
        {
            printf("Adjacent numbers are : %d,%d\n",arr[i],p[i]);
        }
    }
}

/***************************************
 *              Q. 4
 *  Print the adjacent pair of the array
 *  whose difference is maximum.
 * *************************************/
void q4(int *arr, uint32_t sz)
{
    int *p = arr;
    int sz2, i2;
    int tmp1, tmp2;
    tmp1 = arr[0] - arr[1];
    if(tmp1 < 0)
    {
        tmp1 = 0 - tmp1;
    }
    arr += 2;
    i2 = 3;
    for(int i=0;i<sz-2;i++)
    {
        tmp2 = arr[i] - 
    }
}

int  main(void)
{
    int a[] = {1,2,3,4,5,6,5,4,3,45,56,76,123,0,9,8};
    printf("%d\n",arr_max(a,sizeof(a)/sizeof(int)));
    uint32_t ar = -1;
    printf("%x\n",ar);
}