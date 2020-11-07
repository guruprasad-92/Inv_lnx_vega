/********************************
 * 
 *  Author  : Guruprasad
 *  Date    : 31/10/2020
 * 
 * ****************************/

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#include "vega_sock.h"
#include "parser_mdm.h"
#include "rtr_dtStruct.h"
#include "basic.h"

void* th_rtr_prd(void* th_arg)
{
    sTH_ARG_PRD_ *psTh_arg = (sTH_ARG_PRD_*)th_arg;
    struct sockaddr_in addr;
    char th_info[] = {"THD_PRD"};
    int fd_clnt = 0, rt_rcv=-1, rt_snd=-1;
    char th_buf[TH_BUF] = {0};
    int temp1 = 0, ret,i, rt_prsMdm, pkt_typ;
    int rt_prs = 0, cnt = 0, prs_typ = 0;

    char *lns[40]={0};

    static Parse_func_ parse_func[35];
    rt_prsMdm = prs_mdm_init(parse_func);
    fd_clnt = start_clnt(&addr,psTh_arg->cTh_ip,psTh_arg->cTh_port);
    printf("start_clnt() = %d\n",fd_clnt);
    if(fd_clnt < 0)
    {
        dbg_print(NULL,"start_clnt() failed.\n");
        dbg_print(NULL,"Returning... .. .\n");
        return NULL;
    }
    prs_typ = PRS_TYP_BTP;
    dbg_print(Bold_Green,"%s Periodic thread started\n",th_info);

    while(1)
    {
        memset(th_buf,0,TH_BUF);
        rt_rcv = recv(fd_clnt,th_buf,TH_BUF,MSG_DONTWAIT);
        if(rt_rcv == 0)
        {
            dbg_print(Bold_Red,"\n%s : ------ Disconnected from the server ------\n",th_info);
            dbg_print(Bold_Red,"%s : Reconnecting ... .. .\n",th_info);
            restart_clnt(&fd_clnt,psTh_arg->cTh_ip,&addr);
        }
        else if(rt_rcv > 0)
        {
            pkt_typ = read_pkt_typ(th_buf);
            ret = split_line(th_buf,lns,40);
            dbg_print(Bold_Red,"split_line() = %d\n",ret);
            cnt = 0;
            if(prs_typ == PRS_TYP_BTP)
            {
                // Locking the critical section
                dbg_print(Bold_Red,"%s--LOCK-acq : RTR_periodic THD\n",th_info);
                sem_wait(psTh_arg->pth_Prd_Sem);
                cnt = get_btp_rcrd(lns,ret,rt_prsMdm,&(psTh_arg->VEGA_PRD),\
                                    parse_func,pkt_typ);
                dbg_print(NULL,"get_btp_rcrd() = %d\n",cnt);
                dbg_print(Bold_Green,"%s--LOCK-rel : RTR_periodic THD\n",th_info);
                sem_post(psTh_arg->pth_Prd_Sem);
                if(cnt == 0)
                {
                    prs_typ = PRS_TYP_PRD;
                    rt_snd = send(fd_clnt,RQ_PRD,8,MSG_DONTWAIT);
                    dbg_print(Bold_Yellow,"%s : Requested Periodic packet.\n",th_info);
                    print_btp_rcrd(&psTh_arg->VEGA_PRD);
                }
                else
                {
                    // dbg_print(NULL,"cnt = %d\n",cnt);
                    // rt_snd = send(fd_clnt,RQ_BTP,10,MSG_DONTWAIT);
                    // cnt = 0;
                    // prs_typ = PRS_TYP_BTP;
                    // dbg_print(Bold_Yellow,"%s : Requested Boot-up packet.\n\n",th_info);
                    // print_btp_rcrd(&(psTh_arg->VEGA_PRD));
                    // print_periodic_rcrd(&psTh_arg->VEGA_PRD);
                }
            }
            else if (PRS_TYP_PRD)
            {
                // dbg_print(Bold_Red,"%s--LOCK-acq : RTR_periodic THD\n",th_info);
                // psTh_arg->iTh_bsy = 1;
                sem_wait(psTh_arg->pth_Prd_Sem);
                
                cnt = get_prd_rcrd(lns,ret,rt_prsMdm,&psTh_arg->VEGA_PRD,parse_func,pkt_typ);
                if(cnt == 0)
                {
                    print_periodic_rcrd(&psTh_arg->VEGA_PRD);
                    dbg_print(Bold_Yellow,"Periodic record : \n");
                    print_btp_rcrd(&psTh_arg->VEGA_PRD);
                }
                else
                {
                    rt_snd = send(fd_clnt,RQ_PRD,10,MSG_DONTWAIT);
                    dbg_print(Bold_Green,"%s : Requested Periodic packet.\n",th_info);
                }            
                
                sem_post(psTh_arg->pth_Prd_Sem);
                // dbg_print(Bold_Green,"%s--LOCK-rel : RTR_periodic THD\n",th_info);
                // psTh_arg->iTh_bsy = 0;
                
                // printf("\n-------- Router data has been parsed and stored in the required field --------\n");
            }
        }
        msleep(10);
    }//while(1)
}

void *thd_rtr_cmd(void *arg)
{
    sTH_ARG_CMD_ *spTh_arg = (sTH_ARG_CMD_*)arg;
    struct sockaddr_in addr;
    char th_info[] = {"TH_CMD :"};
    int fd_clnt = 0, rt_rcv=-1, rt_snd=-1;
    char th_buf[TH_BUF] = {0};

    fd_clnt = start_clnt(&addr,spTh_arg->cTh_ip,spTh_arg->cTh_port);
    if(fd_clnt < 0)
    {
        dbg_print(NULL,"%s start_clnt() failed.\n",th_info);
        dbg_print(NULL,"%s Returning... .. .\n",th_info);
        return NULL;
    }
    dbg_print(Bold_Green,"%s Instantaneous thread started\n",th_info);
    while(1)
    {
        msleep(10);

        rt_rcv = recv(fd_clnt,th_buf,1000,MSG_DONTWAIT);
        
        // dbg_print(Bold_Red,"%s--LOCK-acq : RTR_CMD THD\n",th_info);
        sem_wait(spTh_arg->pth_Inst_Sem);
        // printf("%s iFlg_snd = %d\n",th_info, spTh_arg->sVEGA_CMD.iFlg_snd);
        if( spTh_arg->sVEGA_CMD.iFlg_snd == CMD_RQ )
        {
            printf("%s Sending command : %s\n",th_info,spTh_arg->sVEGA_CMD.cCmd_snd);
            if(strlen(spTh_arg->sVEGA_CMD.cCmd_snd) > 0 )
            {
                rt_snd = send(fd_clnt,spTh_arg->sVEGA_CMD.cCmd_snd,\
                    strlen(spTh_arg->sVEGA_CMD.cCmd_snd),MSG_DONTWAIT);
                if(rt_snd > 0)                    
                {
                    printf("%s Command Sent.\n",th_info);
                }
                spTh_arg->sVEGA_CMD.iFlg_snd = 2;
                spTh_arg->sVEGA_CMD.iFlg_rsp = 0;
            }
        }
        sem_post(spTh_arg->pth_Inst_Sem);
        // dbg_print(Bold_Red,"%s--LOCK-relz : RTR_CMD THD\n",th_info);

        if(rt_rcv == 0)
        {
            dbg_print(Bold_Red,"\n%s : ------ Disconnected from the server ------\n",th_info);
            dbg_print(Bold_Red,"%s : Reconnecting ... .. .\n",th_info);
            restart_clnt(&fd_clnt,spTh_arg->cTh_ip,&addr);
        }
        else if( rt_rcv > 0 || rt_rcv < 0)
        {
            // dbg_print(Bold_Red,"%s--LOCK-acq : RTR_CMD THD\n",th_info);
            sem_wait(spTh_arg->pth_Inst_Sem);
            if(spTh_arg->sVEGA_CMD.iFlg_snd == CMD_SNT_PRCS )
            {
                if(strlen(th_buf) > 0)
                {
                    memset(spTh_arg->sVEGA_CMD.cCmd_rsp,0,\
                        strlen(spTh_arg->sVEGA_CMD.cCmd_rsp) );

                    strncpy(spTh_arg->sVEGA_CMD.cCmd_rsp, th_buf, SZ_CMD_RSP);
                    spTh_arg->sVEGA_CMD.iFlg_rsp = 1;
                    spTh_arg->sVEGA_CMD.iFlg_snd = 0;
                    memset(th_buf,0,TH_BUF);
                }
            }
            sem_post(spTh_arg->pth_Inst_Sem);
            // dbg_print(Bold_Red,"%s--LOCK-relz : RTR_CMD THD\n",th_info);
        }
    }    
}

/*********************************
 * 
 *  Not required for V2 
 *  Required for testing
 * 
 * ******************************/
void *thd_rtr_cmd_test(void *arg)
{
    sTH_ARG_CMD_ *spTh_arg = (sTH_ARG_CMD_*)arg;
    char th_buf[TH_BUF] = {0};
    int i=0, sz_cmd_exp = 0;
    time_t t_cur=0, t_prv=0, t_dif=0;
    char th_info[] = {"Th_cTst :"};
    time_t p_tm=0, c_tm=0;
    char frmt_tm[60] = {0};
    int t_flg = 0;
    char cmd_exp[][30] = {"$RTR+SIM=1",\
                            "$RTR+SIM=2",
                        }; //Total 17
    sz_cmd_exp = (sizeof(cmd_exp) / sizeof(cmd_exp[0]));
    spTh_arg->sVEGA_CMD.iFlg_snd = 0;
    spTh_arg->sVEGA_CMD.iFlg_rsp = 0;
    dbg_print(Bold_Green,"%s Supporting thread started\n",th_info);
    while(1)                        
    {
        for(i=0;i<sz_cmd_exp;)
        {
            time(&c_tm);
            if(t_flg == 0)
            {
                p_tm = c_tm;
                t_flg = 1;
            }
            else if(c_tm-p_tm > 60*4)
            {
                t_flg = 0;
                dbg_print(Bold_Red,"%s Locking-sem\n",th_info);
                sem_wait(spTh_arg->pth_Inst_Sem);
                if(spTh_arg->sVEGA_CMD.iFlg_snd == 0) // Will be written by the thd_cmd
                {
                    memset(frmt_tm,0,strlen(frmt_tm));
                    get_formated_time(frmt_tm,1);
                    dbg_print(NULL,"%s %s :Sending command < %s > to TH_CMD\n",th_info,\
                    frmt_tm, cmd_exp[i]);
                    memset(spTh_arg->sVEGA_CMD.cCmd_snd,0,\
                            strlen(spTh_arg->sVEGA_CMD.cCmd_snd));
                    strcpy(spTh_arg->sVEGA_CMD.cCmd_snd,cmd_exp[i]);
                    spTh_arg->sVEGA_CMD.iFlg_snd = CMD_SNT;
                    printf("\n%s iFlg_snd has raised to %d\n",th_info,spTh_arg->sVEGA_CMD.iFlg_snd);
                }
                sem_post(spTh_arg->pth_Inst_Sem);
                dbg_print(Bold_Green,"%s Released-sem\n",th_info);
            }
            else 
            {   
                sem_wait(spTh_arg->pth_Inst_Sem);
                if(spTh_arg->sVEGA_CMD.iFlg_rsp == 1) //
                {
                    // We got the response and cleaned it after printing
                    dbg_print(Bold_Yellow,"%s CMD <%s> response  = \n%s\n",th_info,\
                                spTh_arg->sVEGA_CMD.cCmd_snd,spTh_arg->sVEGA_CMD.cCmd_rsp);
                    memset(spTh_arg->sVEGA_CMD.cCmd_rsp,0,\
                        strlen(spTh_arg->sVEGA_CMD.cCmd_rsp));
                    spTh_arg->sVEGA_CMD.iFlg_rsp = 0;
                    i += 1;            
                    sem_post(spTh_arg->pth_Inst_Sem);
                    dbg_print(Bold_Green,"%s Released-sem\n",th_info);
                }
                sem_post(spTh_arg->pth_Inst_Sem);
            }
        }
    }
}