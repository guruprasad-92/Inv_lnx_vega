#include <stdio.h>
#include <cmd_sts/cmd_sts.h>
#include <basic/basic.h>


uint8_t cmd_verify_sts(CMD_ *CMD, CMD_STS_ *CMD_STS)
{
    uint8_t ret = 0;
    if(CMD->cnt[1] != C_STS_ARG)
    {
        printf("ERR-STS : Minimum argument is 1\n\r");
        ret = 0;
    }
    else
    {
        if(CMD->arg_2[0][0] == '?')
        {
            CMD_STS->cln_sts = 0;
            CMD_STS->sts = CMD->arg_2[0][0];
            ret = 1;
        }
        else
        {
            printf("ERR-STS : Invalid argument\n\r");
            ret = 0;
        }
    }
    return ret;
}

void cmd_sts_print(const CMD_STS_ *CMD_STS)
{
    printf("DBG-STS : clean status = %d\n\r",CMD_STS->cln_sts);
    printf("DBG-STS : STS = %c\n\r",CMD_STS->sts);
}

void cmd_sts_clean(CMD_STS_ *CMD_STS)
{
    CMD_STS->cln_sts = 1;
    CMD_STS->sts = -1;
}

int cmd_op_sts(CMD_STS_ *CMD_STS)
{
    int ret = 0;
    printf("DBG-STS : status operation has not implemeted yet\n\r");;
    return ret;
}