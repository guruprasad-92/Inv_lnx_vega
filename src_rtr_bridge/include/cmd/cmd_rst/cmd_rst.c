#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/reboot.h>

#include "cmd_rst/cmd_rst.h"
#include "basic/basic.h"
#include "basic/color.h"


uint8_t cmd_verify_rst(CMD_ *CMD, CMD_RST_ *CMD_RST)
{
    uint8_t ret = 0;
    int tmp = 0;
    if(CMD->cnt[1] != RST_ARG_MX)
    {
        CC_COLOR_RED;
        printf("ERR-RST : Minimum argument is 1\n\r");
        CC_COLOR_RST;
        ret = 0;
    }
    else
    {
        if(is_numeric(CMD->arg_2[0]) <= 0)
        {
            printf("ERR-RST : Argument must be numeric.\n\r");
        }
        else
        {
            tmp = atoi(CMD->arg_2[0]);
            if( (tmp==RST_MN) )//&& (tmp<=RST_MX) )
            {
                CMD_RST->cln_sts = 0;
                CMD_RST->rst = (uint8_t)tmp;
                ret = 1;
            }
            else
            {
                printf("ERR-RST : Argument must be 1 / 2\n\r");
                ret = 0;
            }
        }
    }
    return ret;
}

void cmd_rst_print(const CMD_RST_ *CMD_RST)
{
    //printf("DBG-RST : Printing RST\n\r");
    if(CMD_RST->cln_sts == 0)
    {
        printf("DBG-RST : RST = %d\n\r",CMD_RST->rst);
    }
}

void cmd_rst_clean(CMD_RST_ *CMD_RST)
{
    CMD_RST->cln_sts = 1;
    CMD_RST->rst = -1;
}

void OP_sys_rst(void)
{
    dbg_print(Bold_Yellow,"\nDBG-RST : --------- rebseting the system(within 2 Sec.) -------\n");
    sleep(2);
    sync();
    dbg_print(Bold_Yellow,"DBG-RST : reset failed due to ",reboot(RB_AUTOBOOT));
}

int cmd_op_rst(CMD_RST_ *CMD_RST)
{
    int ret = 0;
    if(CMD_RST->rst == 1)
    {
        printf("DBG-RST : Reseting the router\n\r");
        OP_sys_rst();
    }
    else if(CMD_RST->rst == 2)
    {
        printf("DBG-RST : Reseting the modem of the router.\n\r");
    }
    printf("DBG-RST : Reseting operation has not implemented yet.\n\r");
}