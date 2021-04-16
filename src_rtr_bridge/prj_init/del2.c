#include <stdio.h>
int main(void)
{
    int a;
    while(1)
    {
        printf("Enter a number : ");
        scanf("%d",&a);
        if(a==0)
        {
            break;
        }
        printf("Second idx = %d\n",(a & 2)>>1);
    }
    return 0;
}

/*

Pack type : Periodic {0/2}
if((typ==0) || (typ==2))

* Pack type : Bootup {0/1}
if((typ==0) || (typ==1))
{

*/