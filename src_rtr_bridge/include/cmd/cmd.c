#include <stdio.h>
#include <cmd/cmd.h>
#include <string.h>

/********** Extern global ***********/
/***********************************/

void cmd_clean(CMD_ *CMD)
{
    CMD->cnt[0] = 0;
    CMD->cnt[1] = 0;
    for(int i=0;i<ARG1_MAX;i++)
    {
        CMD->arg_1[i] = NULL;        
    }
    for(int i=0;i<ARG2_MAX;i++)
    {
        CMD->arg_2[i] = NULL;
    }
}

void cmd_print(CMD_ *CMD)
{
    if( (CMD->cnt[0] > ARG1_MAX) && (CMD->cnt[1] > ARG2_MAX) )
    {
        printf("ERR : Count exceeds in structure CMD.\n");
    }
    else
    {
        printf("-----CMD->arg_1-------\n");
        for(int i=0;i<CMD->cnt[0];i++)
        {
            printf("%s\n",CMD->arg_1[i]);
        }

        printf("-----CMD->arg_2-------\n");
        for(int i=0;i<CMD->cnt[1];i++)
        {
            printf("%s-",CMD->arg_2[i]);
        }
        printf("\n");
    }
}

void cmd_init(CMD_2_ *GS__CMD_2)
{
    int i=0;
    sprintf(GS__CMD_2->CMD_FRMT[i].type,"SMS");
    GS__CMD_2->CMD_FRMT[i].arg2_max = 3;
    GS__CMD_2->CMD_FRMT[i].arg2_min = 2;

    sprintf(GS__CMD_2->CMD_FRMT[++i].type,"SIM");
    GS__CMD_2->CMD_FRMT[i].arg2_max = 1;
    GS__CMD_2->CMD_FRMT[i].arg2_min = 1;

    sprintf(GS__CMD_2->CMD_FRMT[++i].type,"RST");
    GS__CMD_2->CMD_FRMT[i].arg2_max = 1;
    GS__CMD_2->CMD_FRMT[i].arg2_min = 1;

    sprintf(GS__CMD_2->CMD_FRMT[++i].type,"STS");
    GS__CMD_2->CMD_FRMT[i].arg2_max = 1;
    GS__CMD_2->CMD_FRMT[i].arg2_min = 1;

    GS__CMD_2->cmd_cnt = i;
}