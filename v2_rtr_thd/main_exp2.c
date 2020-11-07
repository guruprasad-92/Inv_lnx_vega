#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include <basic.h>

int main(void)
{
    char str[100] = {0};
    time_t tt;
    get_formated_time(str,1);
    printf("1= %s\n",str);
    get_formated_time(str,2);
    printf("1= %s\n",str);
    get_formated_time(str,3);
    printf("1= %s\n",str);
    time(&tt);
    printf("ctimr() = %s\n",ctime(&tt));
}