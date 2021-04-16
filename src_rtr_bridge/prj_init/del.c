#include<stdio.h>
#include<stdint.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/reboot.h>
typedef struct ST_
{
    int a;
    int b;
    float f;
}ST;
ST st, *st1;


int str2numstr(char *str, char *numstr[], int len)
{
    int sz = strlen(str);
    int c = 0, j =0;
    char *st, *end;
    for(int i=0;i<sz;i++)
    {
        if( (str[i] >= '0') && (str[i] <= '9') )
        {
            if( j==0 )
            {
                if( (c+1) < len )
                {
                    numstr[c++] = &str[i];
                    printf("start num = %c idx = %d\n",str[i],i);
                }
            }
            if(i==0)
            {
                j = -1;
            }
            else
            {
                j = i;
            }
        }
        else if( (i-j) == 1)
        {
            j = 0;
            str[i] = 0;
        }
    }
    return c;
}




int main(void)
{
    time_t tm1, tm2;
    int flg=0;
    while(1)
    {
        time(&tm1);
        if(flg==0)
        {
            tm2 = tm1;
            flg = 1;
        }
        if(tm1-tm2 >= 3)
        {
            printf("%s\n",ctime(&tm1));
            flg = 0;
        }
    }
    /*
    char *numstr[30]={0};
    char str[200]={"12-adfweaf-345-skjvb-3265-jdsf3498bg-875e4-nvskdjb-635834-kasdjbv-387456-,mxnbv-387456834-aksjdb-34875638-skdb345skjh-jfdbh123--234"};
    int ret = str2numstr(str,numstr,15);
    for(int i=0;i<ret;i++)    
    {
        printf("%d. %s\n",i,numstr[i]);
    }
    */

}