#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <semaphore.h>
#include <pthread.h>

#include "parser/parser.h"
#include "cmd/cmd.h"
#include "basic/color.h"
#include "basic/basic.h"
#include "socket/socket.h"

static CMD_ALL_ CMD_ALL;

static CMD_     CMD;
static CMD_SMS_ CMD_SMS;
static CMD_RST_ CMD_RST;
static CMD_SIM_ CMD_SIM;
static CMD_STS_ CMD_STS;

static char p1_Tbuf[SZ_TBF]={0};
static char p2_Tbuf[SZ_TBF]={0};



int main()
{
    //printf("Enter the cmd : ");
    char str[50]={0};
    uint8_t rt = 0;
    int split = 0;

    sem_t sem[2] = {0};
    int ret = 0, rt_th1,rt_th2;
    pthread_t th_id[2] = {0};
    void *sts;
    TH_ARG_ th_arg[2] = {0};
    TH_ARG_P2_ th2_arg = {0};

    th2_arg.port = P2_41;
    th2_arg.pr_sts = NULL;
    th2_arg.th_buf = p2_Tbuf;
    dbg_print(Bold_Yellow,"---------------------Invendis_Router-----------------------\n");
    dbg_print(Bold_Yellow,"FMW_VER = %s\n",APP_VER);
    dbg_print(Bold_Yellow,"Building date : %s__%s\n",__DATE__, __TIME__);
    dbg_print(Bold_Yellow,"Invendis Technologies, Bengaluru\n");
    dbg_print(Bold_Yellow,"-----------------------------------------------------------\n");

    dbg_print(Bold_Green,"\nWaiting for system uptime upto 3mins\n\n");

    wait_upto_uptm(3);

    sprintf(th2_arg.th_info,"THD-2");

    th_arg[0].port = P1_40;
    th_arg[1].port = P2_41;
    memset(th_arg[0].th_info,0,5);
    memset(th_arg[1].th_info,0,5);

    sprintf(th_arg[0].th_info,"%s","THD-1");
    sprintf(th_arg[1].th_info,"%s","THD-2");

    CMD_ALL.CMD     = &CMD;
    CMD_ALL.CMD_RST = &CMD_RST;
    CMD_ALL.CMD_STS = &CMD_STS;
    CMD_ALL.CMD_SIM = &CMD_SIM;
    CMD_ALL.CMD_SMS = &CMD_SMS;


    th_arg[0].th_buf = p1_Tbuf;
    th_arg[1].th_buf = p2_Tbuf;
    th_arg[0].th_buf_sz = th_arg[1].th_buf_sz = SZ_TBF;
    th_arg[0].CMD_ALL = &CMD_ALL;
    th_arg[1].CMD_ALL = NULL;
    
    
    dbg_print(Bold_Magenta,"ptr-arg1 = %p\n",CMD.arg_1);
    dbg_print(Bold_Magenta,"ptr-th_buf = %p\n",th_arg[0].th_buf);


    for(int i=0;i<2;i++)
    {
        ret = sem_init(&sem[i],0,1);
        if(ret < 0)
        {
            dbg_print(Bold_Red,"ERR : sem_init(%d) failed due to : %s\n",i,\
                        strerror(errno));            
            dbg_print(Bold_Red,"ERR-SEM-init : Exiting ... ..\n\r");
            return 0;
        }
        dbg_print(Cyan,"DBG-SEM : semaphore-%d created\n",i);
    }
    th_arg[0].th_sem = &sem[0];
    th2_arg.th_sem = &sem[0];


    rt_th1 = pthread_create(&th_id[0],NULL,&th_port1,(void*)&th_arg[0]);
    if(rt_th1 < 0)
    {
        dbg_print(Bold_Red,\
        "ERR-TH1 : pthread_create(1) failed due to : %s\n\r",strerror(errno));
        dbg_print(Bold_Red,"Exiting... .. .\n\r");
        exit(EXIT_FAILURE);
    }
    
    rt_th2 = pthread_create(&th_id[1],NULL,&th_port2,(void*)&th2_arg);
    if(rt_th2 < 0)
    {
        dbg_print(Bold_Red,\
        "ERR-TH1 : pthread_create(2) failed due to : %s\n\r",strerror(errno));
        dbg_print(Bold_Red,"Exiting... .. .\n\r");
        exit(EXIT_FAILURE);
    }

    ret = pthread_join(th_id[0],&sts);
   ret = pthread_join(th_id[1],&sts);
    while(1)
    {
        // memset(str,0,50);
        // printf("---------------- Enter command : ");
        // scanf("%s",str);     


/*        
        rt = cmd_verify_tok(str);
        if (rt == 1)
        {
            cmd_clean(&CMD);
            split = cmd_split(str,&CMD);
            printf("cmd_split=%d\n",split);
            if(split == 1)
            {
                if(strcmp(CMD.arg_1[1],C_SMS) == 0)
                {
                    if(cmd_verify_sms(&CMD,&CMD_SMS))
                    {
                        printf("DBG-SMS : VAlid SMS.\n");
                        printf("Printing the cmd sms : \n");
                        cmd_sms_print(&CMD_SMS);
                        cmd_sms_clean(&CMD_SMS);
                    }
                    else
                    {
                        printf("ERR-CMD : CMD format is invalid.\n\r");
                    }
                }
                else if(strcmp(CMD.arg_1[1],C_SIM) == 0)
                {
                    if(cmd_verify_sim(&CMD,&CMD_SIM))
                    {
                        printf("DBG-SIM : VAlid SIM.\n");
                        printf("Printing the cmd sim : \n");
                        cmd_sim_print(&CMD_SIM);
                        cmd_sim_clean(&CMD_SIM);
                    }
                }
                else if(strcmp(CMD.arg_1[1],C_RST) == 0)
                {
                    if(cmd_verify_rst(&CMD,&CMD_RST))
                    {
                        printf("DBG-RST : VAlid RST.\n");
                        printf("Printing the cmd RST : \n");
                        cmd_rst_print(&CMD_RST);
                        cmd_rst_clean(&CMD_RST);
                    }
                }
                else if(strcmp(CMD.arg_1[1],C_STS) == 0)
                {   
                    if(cmd_verify_sts(&CMD,&CMD_STS))
                    {
                        printf("DBG-STS : Command STS has validated.\n");
                        printf("Printing the cmd STS : \n");
                        cmd_sts_print(&CMD_STS);
                        cmd_sts_clean(&CMD_STS);
                    }
                }
            }
                     
        }
*/        
        // sem_wait(&sem[0]);
        // printf("\n\rDBG-CMD : cmd_parse() = %d\n\r",\
        // cmd_parse(str,&CMD_ALL));
    }

    ret = pthread_join(th_id[0],&sts);
    ret = pthread_join(th_id[1],&sts);
}