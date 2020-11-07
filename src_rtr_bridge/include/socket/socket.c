#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>
#include <sys/time.h>

#include <semaphore.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>


#include "basic/color.h"
#include "basic/basic.h"
#include "socket/socket.h"
#include "parser/parser.h"
#include "operation/mdm_payload.h"
#include "operation/mdm_operation.h"


static char p1_Tbuf[SZ_TBF]={0};
static char p2_Tbuf[SZ_TBF]={0};


/*
int main(void)
{
    sem_t   sem[2] = {0};
    int ret = 0;

    pthread_t th_id[2] = {0};    
    void *sts;
    TH_ARG_ th_arg[2];
    th_arg[0].port = P1_40;
    th_arg[1].port = P2_41;

    memset(th_arg[0].th_info,0,5);
    memset(th_arg[1].th_info,0,5);

    sprintf(th_arg[0].th_info,"%s","THD-1");
    sprintf(th_arg[1].th_info,"%s","THD-2");

    dbg_print(Yellow,"Wellcome = %d\n",5);
    dbg_print(Bold_Yellow,"Wellcome = %d\n",ret);
    dbg_print(NULL,"HI\n");

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
    
    ret = pthread_create(&th_id[0],NULL,&th_port1,(void*)&th_arg[0]);
    if(ret < 0)
    {
        dbg_print(Bold_Red,\
        "ERR-TH1 : pthread_create(1) failed due to : %s\n\r",strerror(errno));
        dbg_print(Bold_Red,"Exiting... .. .\n\r");
        exit(EXIT_FAILURE);
    }
    ret = pthread_create(&th_id[1],NULL,&th_port1,(void*)&th_arg[1]);
    if(ret < 0)
    {
        dbg_print(Bold_Red,\
        "ERR-TH1 : pthread_create(1) failed due to : %s\n\r",strerror(errno));
        dbg_print(Bold_Red,"Exiting... .. .\n\r");
        exit(EXIT_FAILURE);
    }
    ret = pthread_join(th_id[0],&sts);
    ret = pthread_join(th_id[1],&sts);
    if(ret)
    {
        dbg_print(Bold_Red,\
        "TH1-ERR : pthread_join(1) failed due to : %s\n\r",
        strerror(errno));
        dbg_print(Bold_Red,"Exiting ... .. .\n\r");
        exit(EXIT_FAILURE);
    }
    //while(1);
}
*/

//------------------------------------------------------------------

/****************************************
 * Flushing a streamed socket.
 * *************************************/
void sk_flush(int fd_sk)
{
    char arr[1024];
    int ret = 5;
    do
    {
        ret = recv(fd_sk,arr,1024,MSG_DONTWAIT);
    } while (ret > 0);
}


int set_sock_opt(int fd_srvr)
{
    #ifdef TM_OUT
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 1000;
    #endif
    int ret=0, opt=1;
    ret = setsockopt(fd_srvr,SOL_SOCKET, SO_KEEPALIVE | \
            SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
        if(ret == -1)
        {
            perror("setsockopt(fd_srvr*) failed due to :");
            return ret;
        }
    #ifdef TM_OUT
        ret = setsockopt(fd_srvr,SOL_SOCKET, \
                SO_RCVTIMEO | SO_SNDTIMEO, \
                (void*)&timeout, sizeof(timeout) );
            if (ret == -1)
            {
                perror("setsockopt(fd_srvr*) <time out> failed due to :");
                printf("Exiting ... .. .\n");
                exit(EXIT_FAILURE);
            }
    #endif
    return ret;
}

/*********************************
 * Arg : fd_srvr, fd_clnt, addr (pointer no array)
 *      len_addr = length of "const struct sockaddr_in"
 * Return : 
 *      
 * ******************************/
int start_srvr( int *fd_srvr, int *fd_clnt, \
                struct sockaddr_in *addr, unsigned long len_addr, int port)
{
    int ret = 0;
    unsigned int tm = 0, cnt=0;
    *fd_srvr = socket(AF_INET, \
                    SOCK_STREAM | SOCK_NONBLOCK, 0 );
    if (*fd_srvr < 0)
    {
        perror("\nSocket failed due to : ");
        ret = 0;
    }
    else
    {
        printf("fd_socket = %d\n",*fd_srvr);
        if( set_sock_opt(*fd_srvr) < 0 )
        {
            printf("set_sock_opt() failed.\n");
            ret = -1;
        }
        else
        {
            printf("set_sock_opt() succedded.\n");
            //addr->sin_family = AF_LOCAL;
            addr->sin_family = AF_INET;
            addr->sin_addr.s_addr = INADDR_ANY;
            addr->sin_port = htons(port);
            ret = bind(*fd_srvr, (struct sockaddr*)addr, \
                    len_addr);
            if(ret < 0)
            {
                perror("bind(fd_srvr) failed due to :");
                ret = -1;
            }
            else
            {
                printf("bind succedded\n");
                printf("bind = %d\n",ret);
                ret = listen(*fd_srvr,3);
                if (ret < 0)
                {
                    perror("listen() failed due to : ");
    //                ret = -1;
                }
                else
                {
                    printf("listen has succedded\n");
                    printf("listen = %d\n",ret);
                    printf("Waiting for client .....\n");
                    
                    do
                    {
                        sleep(1);
                        *fd_clnt = accept(*fd_srvr,(struct sockaddr*)addr,\
                                    (socklen_t*)&len_addr);
                        cnt += 1;
                        printf("Waiting count : %d\n",cnt);
                        if (cnt==50)
                        {
                            printf("Reseting the count\n");
                            cnt = 0;
                        }
                    } while (*fd_clnt < 0);
                    
                    
                    if (*fd_clnt < 0)
                    {
                        perror("accept failed due to : ");
                        ret = -1;
                    }
                    else
                    {
                        ret = 1;
                    } // accept()
                }// listen()
            }// bind()
        } //set_sock_opt()
    }// socket()
    return ret;
}                


/************************************
 * Check the availability of the client upto time "Tm".
 * This function fas to called after joining the client.
 * 
 * Tm_typ, defines the Tm
 * Tm_typ {0, Tm is in seconds
 *         1, Tm is in minutes
 *        }
 * 
 * Return : 
 *      On 
//  * *********************************/
// uint8_t chk_clnt(const int fd_clnt, time_t Tm, const uint8_t Tm_typ)
// {
//     char buf[5]={0};
//     int rt_rcv;
//     if(Tm_typ == 1)
//     {
//         Tm *= 60;
//     }
//     else if(Tm_typ != 0)
//     {
        
//     }
    
//     do
//     {
//         rt_rcv = recv(fd_clnt,buf,5,MSG_DONTWAIT);
//         if(rt_rcv == 0)
//         {
//             return 0;
//         }
//     } while (/* condition */);
// }



/******************************
 *          CMD 
 * ***************************/
void *th_port1(void *arg)
{
    TH_ARG_ *th_arg = (TH_ARG_ *)arg;
    int fd_srvr,fd_clnt, rt_rcv,rt_snd;
    struct sockaddr_in addr;
    unsigned long ln_addr = sizeof(addr);
    int cnt = 5;
    int sts = start_srvr(&fd_srvr,&fd_clnt,&addr,ln_addr,th_arg->port);
    char cmd_cpy[4] = {0};
    if(sts < 0)
    {
        dbg_print(Bold_Red,"%s_ERR : start_srvr() failed. \n\r",th_arg->th_info);
        dbg_print(Bold_Red,"%s_Exiting ....\n\r",th_arg->th_info);
        exit(EXIT_FAILURE);
    }
    else
    {
        dbg_print(NULL,"\n\r%s : Communication Established.\n\r",th_arg->th_info);
        rt_rcv = rt_snd = -1;
        int sz = 0;
        uint8_t ret = 0;
        int cmd_ex_sts = -1;
        int ret_simSel = -1;
        uint32_t tm_sim_swch = 0;
        while(1)        
        {
            rt_rcv = recv(fd_clnt,th_arg->th_buf,SZ_TBF,MSG_DONTWAIT);
            msleep(100);
            if(rt_rcv > 0)
            {
                dbg_print(Cyan, \
                "\n\r%s : Writing to client...\n\r",th_arg->th_info);
                /************
                 * Perform the parsing here
                 * *********/
                sz = strlen(th_arg->th_buf);
                if( sz >= CMD_STR_MN || sz <= CMD_STR_MX )
                {
                    cln_NL_CR(th_arg->th_buf);
                    cmd_clean_all(th_arg->CMD_ALL);
                    ret = cmd_parse(th_arg->th_buf,\
                            th_arg->CMD_ALL);
                    
                    printf("cmd_parse() = %d\n",ret);

                    for(int i=0;
                        i<th_arg->CMD_ALL->CMD->cnt[0]; i++)
                    {
                        printf("arg-1 : %s---\n",th_arg->CMD_ALL->CMD->arg_1[i]);
                    }
                    for(int i=0;
                        i<th_arg->CMD_ALL->CMD->cnt[1]; i++)
                    {
                        printf("arg-2 : %s---\n",th_arg->CMD_ALL->CMD->arg_2[i]);
                    }

                    printf("-------%s-----\n",th_arg->CMD_ALL->CMD->arg_1[1]);

                    if(ret > 0)
                    {
                        memset(cmd_cpy,0,strlen(cmd_cpy));
                        sprintf(cmd_cpy,"%s",th_arg->CMD_ALL->CMD->arg_1[1]);
                        memset(th_arg->th_buf,0,SZ_TBF);
                        
                        if(ret == EN_RST)
                        {
                            if(th_arg->CMD_ALL->CMD_RST->cln_sts == 0)
                            {
                                if(th_arg->CMD_ALL->CMD_RST->rst == 1)
                                {
                                    dbg_print(Bold_Green,"Rebooting the system on request\n\r");
                                    sleep(10);
                                    dbg_print(Bold_Green,"Rebooting the system on request\n\r");
                                    cmd_rst_clean(th_arg->CMD_ALL->CMD_RST);
                                    sys_reboot();
                                    cmd_ex_sts = 1;
                                }
                                else
                                {
                                    dbg_print(Bold_Red,"RBT : Wrong parameter.\n");
                                }
                                
                            }
                        }
                        else if(ret == EN_SIM)
                        {
                            dbg_print(Bold_Green,"\nSIM = %d\ncln_sts=%d\n",\
                                    th_arg->CMD_ALL->CMD_SIM->sim,\
                                    th_arg->CMD_ALL->CMD_SIM->cln_sts);

                            if(th_arg->CMD_ALL->CMD_SIM->cln_sts == 0)
                            {
                                // memset(th_arg->th_buf,0,SZ_TBF);
                                // sprintf(th_arg->th_buf,\
                                //         "\nsrvr : Selecting the sim-%d. Minimum process time 120Sec.\n",\
                                //         th_arg->CMD_ALL->CMD_SIM->sim);
                                // rt_snd = send(fd_clnt,th_arg->th_buf,\
                                //             strlen(th_arg->th_buf),MSG_DONTWAIT);
                                memset(th_arg->th_buf,0,SZ_TBF);

                                dbg_print(Bold_Yellow,"%s : waiting for sem_mdm ...\n",th_arg->th_info);
                                sem_wait(th_arg->th_sem);
                                rt_snd = mdm_selSim(th_arg->CMD_ALL->CMD_SIM->sim, &tm_sim_swch);
                                sem_post(th_arg->th_sem);
                                dbg_print(Bold_Yellow,"%s : sem_mdm has released.\n",th_arg->th_info);

                                sprintf(th_arg->th_buf,"\nsrvr : Active_sim = %d, Switching time = %dSec\n",rt_snd,tm_sim_swch);
                                cmd_ex_sts = 1;
                                
                            }
                            cmd_sim_clean(th_arg->CMD_ALL->CMD_SIM); // Otherwise th_buf may corrupt
                        }
                        else
                        {
                            cmd_ex_sts = -1;
                        }

                        if(cmd_ex_sts == -1)
                        {
                            memset(th_arg->th_buf,0,strlen(th_arg->th_buf));
                            sprintf(th_arg->th_buf,\
                                "Srver : CMD(%s) is valid.\n\rSrvr : Operation does not support.\n\r",\
                                cmd_cpy);
                        }
                    }
                    else
                    {   
                        memset(cmd_cpy,0,strlen(cmd_cpy));
                        sprintf(cmd_cpy,"%s",th_arg->CMD_ALL->CMD->arg_1[1]);
                        memset(th_arg->th_buf,0,SZ_TBF);
                        sprintf(th_arg->th_buf,\
                        "SRVR : CMD(%s) is invalid.\n\r",cmd_cpy);
                    }

                    rt_snd = send(fd_clnt,th_arg->th_buf,
                                strlen(th_arg->th_buf),0);
                    
                    memset(th_arg->th_buf,0,strlen(th_arg->th_buf));
                }
                else
                {
                    
                    memset(th_arg->th_buf,0,SZ_TBF);
                    sprintf(th_arg->th_buf,\
                    "SRVR : CMD is invalid.\n\r");

                    rt_snd = send(fd_clnt,th_arg->th_buf,
                                strlen(th_arg->th_buf),0);
                    memset(th_arg->th_buf,0,\
                    strlen(th_arg->th_buf));
                }
            }
            if(rt_rcv == 0)
            {
                close(fd_clnt);
                close(fd_srvr);
                rt_rcv = rt_snd = -1;
                dbg_print(Yellow,
                "\n\r%s_DBG : Communication lost\n\r",th_arg->th_info);
                dbg_print(Yellow,
                "\n\r%s_DBG : Connection in process....\n\r",th_arg->th_info);
                sts = start_srvr(&fd_srvr,&fd_clnt,&addr,ln_addr,th_arg->port);
                if(sts > 0)
                {
                    dbg_print(Yellow,
                    "\n\r%s_DBG : Connection re-established.\n\r",th_arg->th_info);
                }
                else
                {
                    rt_snd = rt_rcv = 0;
                    sleep(1);
                }
            }
        }
    }
}


/************************************************************
 *          Periodic thread
 * *********************************************************/
void *th_port2(void *arg)
{
    TH_ARG_P2_ *th_arg = (TH_ARG_P2_ *)arg;
    int fd_srvr,fd_clnt, rt_rcv,rt_snd;
    struct sockaddr_in addr;
    unsigned long ln_addr = sizeof(addr);
    char buf_btp[1024] = {0};
    char buf_rcv[10] = {0};
    int rt_pkt = 0;
    int cnt = 5;
    uint32_t btup_typ = 0;
    uint8_t interval = 60;
    time_t tm_cur=0, tm_prv=0;

    time_t p_tm, c_tm;
    int t_flg=0;

    uint32_t sz_pkFun = 20, rt_pkFun = 0;
    Pack_func_ Pack_func[20];
    uint32_t arr_pkFun[20];
    // char st_str[10] = {"\n\rPeriodic status : \n\r\0"};
    
    rt_pkFun = pack_func_init(Pack_func,arr_pkFun);
    printf("pack_func_init() = %d\n",rt_pkFun);

    dbg_print(Bold_Yellow,"%s : waiting for sem_mdm ...\n",th_arg->th_info);
    sem_wait(th_arg->th_sem);
    rt_pkt = pkt_btup(buf_btp,1000,Pack_func,rt_pkFun);
    // if(rt_pkt == -1) // Success / error
    // {
    //     btup_typ = PACK_TYP_BTUP2;
    // }
    // else
    // {
    //     btup_typ = PACK_TYP_BTUP;
    // }
    
    sem_post(th_arg->th_sem);
    dbg_print(Bold_Yellow,"%s : sem_mdm has released.\n",th_arg->th_info);


    int sts = start_srvr(&fd_srvr,&fd_clnt,&addr,ln_addr,th_arg->port);
    char pld[2048] = {0}, pld2[2048]={0};
    printf("\nserver started.\n");
    if(sts < 0)
    {
        dbg_print(Bold_Red,"%s_ERR : start_srvr() failed. \n\r",th_arg->th_info);
        dbg_print(Bold_Red,"%s_Exiting ....\n\r",th_arg->th_info);
        exit(EXIT_FAILURE);
    }
    else
    {
        rt_rcv = rt_snd = -1;
        int rt=0;
        while(1)
        {
            // printf("\nIn while(1)\n");
            memset(th_arg->th_buf,0,SZ_TBF);
            
            memset(pld,0,1024);
            memset(pld2,0,1024);
            sprintf(th_arg->th_buf,"\n\rPeriodic status : \n\r");
            // printf("\nIn while(1)\n");
            get_time(th_arg->th_buf+(strlen(th_arg->th_buf)));

            if(btup_typ == PACK_TYP_PRD)
            {
                //------ periodic payload --------
                dbg_print(Bold_Yellow,"%s : waiting for sem_mdm ...\n",th_arg->th_info);
                sem_wait(th_arg->th_sem);

                // rt = mdm_pld_btup(pld,pld2,2040,"|");
                rt = pkt_prd(pld,1024,Pack_func,rt_pkFun);

                sem_post(th_arg->th_sem);
                dbg_print(Bold_Yellow,"%s : sem_mdm has released.\n",th_arg->th_info);
            }
            else
            {
                // send the bootup packet every time
                if(btup_typ == PACK_TYP_BTUP2)
                {
                    dbg_print(Bold_Yellow,"%s : waiting for sem_mdm ...\n",th_arg->th_info);
                    sem_wait(th_arg->th_sem);

                    memset(buf_btp,0,1000);
                    rt_pkt = pkt_btup(buf_btp,1000,Pack_func,rt_pkFun);                

                    sem_post(th_arg->th_sem);
                    dbg_print(Bold_Yellow,"%s : sem_mdm has released.\n",th_arg->th_info);
                    if(rt_pkt == -1)
                    {
                        btup_typ = PACK_TYP_BTUP2;
                    }
                    else
                    {
                        btup_typ = PACK_TYP_PRD;
                    }               
                }                

                // rt = mdm_pld_btup(pld,pld2,2040,"|");
                //rt = pkt_prd(pld,1024,Pack_func,rt_pkFun);
                sprintf(pld,"%s",buf_btp);
                rt = strlen(pld);

            }            

            if(rt > 0)
            {
                strcat(th_arg->th_buf,pld);
                dbg_print(Reset,"size(th_buf) = %d\n",strlen(th_arg->th_buf));
            }

            // if(rt == 100)
            // {
            //     strcat(th_arg->th_buf,pld);
            //     th_arg->th_buf[strlen(th_arg->th_buf)] = '|'; // remove me in the final stage
            //     // strcat(th_arg->th_buf,&del);
            //     strcat(th_arg->th_buf,pld2);
            //     printf("size(th_buf) = %d\n",strlen(th_arg->th_buf));
            // }
            //---------------------------------------------

            dbg_print(Bold_Magenta,"THD-2 : Sending periodic sts : %s\n\r",\
                    th_arg->th_buf);
            rt_snd = send(fd_clnt,th_arg->th_buf,strlen(th_arg->th_buf),MSG_DONTWAIT);
            rt_rcv = recv(fd_clnt,buf_rcv,10,MSG_DONTWAIT);
            if(rt_rcv == 0)
            {
                close(fd_clnt);
                close(fd_srvr);
                rt_rcv = rt_snd = -1;
                dbg_print(Yellow,
                "\n\r%s_DBG : Communication lost\n\r",th_arg->th_info);
                dbg_print(Yellow,
                "\n\r%s_DBG : Connection in process....\n\r",th_arg->th_info);
                sts = start_srvr(&fd_srvr,&fd_clnt,&addr,ln_addr,th_arg->port);
                if(sts > 0)
                {
                    dbg_print(Yellow,
                    "\n\r%s_DBG : Connection re-established.\n\r",th_arg->th_info);
                }
                else
                {
                    rt_snd = rt_rcv = 0;
                    sleep(1);
                }
            }
            if(rt_snd > 0)
            {
                if(strstr(buf_rcv,"RQST_PRD"))
                {
                    dbg_print(Bold_Yellow,"Client : Acknowledgement = %s\n",buf_rcv);
                    btup_typ = PACK_TYP_PRD;
                    memset(buf_rcv,0,strlen(buf_rcv));
                }
                else if(strstr(buf_rcv,"RQST_BTP"))
                {
                    dbg_print(Bold_Yellow,"Client : Bootup request.\n");
                    btup_typ = PACK_TYP_BTUP2; // Creation of bootup packet again.
                    memset(buf_rcv,0,strlen(buf_rcv));
                }
                sleep(30);
            }
        }
    }
}

