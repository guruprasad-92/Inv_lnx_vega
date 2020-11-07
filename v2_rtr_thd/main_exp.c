/***************************
 * 
 *  Author  : Guruprasad
 *  Date    : 31/10/2020
 * 
 * ************************/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <netinet/in.h>

#include "rtr_dtStruct.h"
#include "vega_thrds.h"
#include "vega_sock.h"
#include "basic.h"

static sTH_ARG_PRD_ sTH_ARG_PRD = {0};
static sTH_ARG_CMD_ sTH_ARG_CMD = {0};

int main(int c, char *mn_arg[])
{
    sRtr_thd_ spRtr_thd[3];
    spRtr_thd[0].func_thd = &th_rtr_prd;
    spRtr_thd[1].func_thd = &thd_rtr_cmd;
    spRtr_thd[2].func_thd = &thd_rtr_cmd_test;
    
    const uint32_t thd_cnt = 3;
    int ret, rt_thd, i;

    void *sts = NULL;
    void *th_args[3] = {0};
    sem_t th_sem[2] = {0};
    pthread_t th_id[3] = {0};

    if(c < 2)
    {
        dbg_print(Bold_Red,"ERR : Not enough arguments.");
        dbg_print(Bold_Red,"Exiting... .. .\n");
        return -1;
    }
    else if(c > 2)
    {
        dbg_print(Bold_Red,"ERR : Too much arguments.");
        dbg_print(Bold_Red,"Exiting... .. .\n");
        return -1;
    }
    
    if(validate_ip(mn_arg[1]) != 0)
    {
        dbg_print(Bold_Red,"ERR : Invalid IP.");
        dbg_print(Bold_Red,"Exiting... .. .\n");
        return -1;
    }


    sprintf(sTH_ARG_PRD.cTh_ip,"%s",mn_arg[1]);
    sprintf(sTH_ARG_CMD.cTh_ip,"%s",mn_arg[1]);
    sTH_ARG_PRD.cTh_port = 41;
    sTH_ARG_CMD.cTh_port = 40;

    for(i=0; i<thd_cnt-1; i++)
    {
        ret = sem_init(&th_sem[i],0,1);
        if(ret < 0)
        {
            dbg_print(Bold_Red,"ERR : sem_init(%d) failed due to : %s\n",i,\
                        strerror(errno));            
            dbg_print(Bold_Red,"ERR-SEM-init : Exiting ... ..\n\r");
            return -1;
        }
        dbg_print(Cyan,"DBG-SEM : semaphore-%d created\n",i);
    }


    sTH_ARG_PRD.pth_Prd_Sem = &th_sem[0];
    sTH_ARG_CMD.pth_Inst_Sem = &th_sem[1];
    
    th_args[0] = (void*)&sTH_ARG_PRD;
    th_args[1] = (void*)&sTH_ARG_CMD;
    th_args[2] = (void*)&sTH_ARG_CMD;
    
    for(i=0;i<thd_cnt;i++)
    {
        ret = pthread_create(&th_id[i],NULL,spRtr_thd[i].func_thd,th_args[i]);
        if(ret < 0)
        {
            dbg_print(Bold_Red,\
            "ERR-TH%d : pthread_create(%d) failed due to : %s\n\r",i,i,strerror(errno));
            dbg_print(Bold_Red,"Exiting... .. .\n\r");
            return(-1);
        }
    }
    for(i=0;i<thd_cnt;i++)
    {
        ret = pthread_join(th_id[i],&sts);
    }
    // ret = pthread_join(th_id[1],&sts);
    while(1);
}