#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termio.h>
#include <string.h>

#include "operation/mdm_operation.h"
#include "operation/mdm_payload.h"
#include "basic/basic.h"

int main(void)
{
    int a = 0;
    printf("Enter sim number to select : ");
    scanf("%d",&a);
    uint32_t tm_tkn = 0;
    if(a==1 || a==2)
    {
        int ret = mdm_selSim(a,&tm_tkn);
        if(ret == a)
        {
            dbg_print(Bold_Green,"Selected sim = %d\n",a);
            dbg_print(Bold_Green,"Time taken to switch : %dSec\n",tm_tkn);
        }
        else
        {
            dbg_print(Bold_Red,"Sim selection failed. Time spent : %d\n",tm_tkn);
        }
        
    }


}