/****************************
 * 
 *  Author  : Guruprasad
 *  Date    :
 * 
 * *************************/

#include <cmd_sim/cmd_sim.h>
#include <basic/basic.h>
#include <stdio.h>
#include <stdlib.h>


uint8_t cmd_verify_sim(CMD_ *CMD, CMD_SIM_ *CMD_SIM)
{
    uint8_t ret = 0;
    int tmp = 0;
    
    if(CMD->cnt[1] != SIM_ARG_MX)
    {
        printf("ERR-SIM : Minimum argument is 1\n\r");
        ret = 0;
    }
    else
    {
        //Success
        //rintf("is_numeric = %d\n",is_numeric(CMD->arg_2[0]));
        if(is_numeric(CMD->arg_2[0]) <= 0)
        {
            printf("ERR-SIM : Argument must be numeric\n\r");
            ret = 0;
        }
        else
        {
            tmp = atoi(CMD->arg_2[0]);
            if( (tmp==1) || (tmp==2) )
            {
                CMD_SIM->sim = (uint8_t)tmp;
                CMD_SIM->cln_sts = 0;
                ret = 1;
            }
            else
            {
                //error
                printf("ERR-SIM : Argument must be 1 / 2\n\r");
                ret = 0;
            }        
        }
    }
    return ret;
}

void cmd_sim_print(const CMD_SIM_ *CMD_SIM)
{
    printf("DBG-SIM : clean status = %d\n",CMD_SIM->cln_sts);
    if(!CMD_SIM->cln_sts)
    {
        printf("DBG-SIM : SIM = %d\n\r",CMD_SIM->sim);
    }
}

void cmd_sim_clean(CMD_SIM_ *CMD_SIM)
{
    CMD_SIM->cln_sts = 1;
    CMD_SIM->sim = -1;
}

int cmd_op_sim(CMD_SIM_ *CMD_SIM)
{
    int ret = 0;
    printf("DBG-SIM-OP : Selecting SIM-%d\n\r",CMD_SIM->sim);
    printf("DBG-SIM-OP : Not implimented yet.\n\r");
    if(CMD_SIM->sim == 0)
    {
        
    }
    else if(CMD_SIM->sim == 1)
    {
        /* code */
    }
}
    
        