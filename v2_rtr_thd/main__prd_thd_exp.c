#include <stdio.h> 
#include <stdlib.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <time.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <semaphore.h>

#include "vega_sock/vega_sock.h"
#include "basic.h"
#include "parser_mdm.h"

int main(void)
{
    struct sockaddr_in addr;
    int fd_clnt = 0, rt_rcv=-1, rt_snd=-1;
    char buf[TH_BUF] = {0};
    int temp1 = 0, ret,i, rt_prsMdm, pkt_typ;
    int rt_prs = 0, cnt = 0, prs_typ = 0;
    
    char *dest[35]={0};
    sVEGA_PRD_ obj = {0};

    Parse_func_ parse_func[35];
    rt_prsMdm = prs_mdm_init(parse_func);

    fd_clnt = start_clnt(&addr,DEV_IP,PORT_41_PER);
    printf("start_clnt() = %d\n",fd_clnt);
    if(fd_clnt < 0)
    {
        printf("start_clnt() failed.\n");
        printf("Returning... .. .\n");
        return -1;
    }
    prs_typ = PRS_TYP_BTP;//Bootup
    while(1)
    {
        memset(buf,0,TH_BUF);
        rt_rcv = recv(fd_clnt,buf,TH_BUF,MSG_DONTWAIT);
        if(rt_rcv == 0)
        {
            printf("\n------ Disconnected from the server ------\n");
            printf("Reconnecting ... .. .\n");
            restart_clnt(&fd_clnt,DEV_IP,&addr);
        }
        else if(rt_rcv > 0)
        {
            //printf("\nFrom Server : \n%s\n",buf);
            pkt_typ = read_pkt_typ(buf);
            ret = split_line(buf,dest,35);
            printf("split_line() = %d\n",ret);
            cnt = 0;
            if(prs_typ == PRS_TYP_BTP)
            {
                // for(i=0;i<rt_prsMdm;i++)
                // {
                //     printf("i=%d;\t",i);
                //     printf("rt_prs = %d;\t",rt_prs);
                //     printf("cnt = %d;\t\n",cnt);
                //     // dbg_print(Bold_Yellow,"data = %s\n",dest[i]);
                //     rt_prs = parse_func[i].f_prs_mdm(dest,ret,&obj,1);
                //     if(rt_prs == 0)
                //     {
                //         cnt++;
                //     }
                // }
                cnt = get_btp_rcrd(dest,ret,rt_prsMdm,&obj,parse_func,pkt_typ);
                if(cnt == 0)
                {
                    prs_typ = PRS_TYP_PRD;
                    rt_snd = send(fd_clnt,"ACK_BTP",8,MSG_DONTWAIT);
                    dbg_print(Bold_Yellow,"Requested Periodic packet.\n");
                    print_btp_rcrd(&obj);
                }
                else
                {
                    printf("cnt = %d\n",cnt);
                    rt_snd = send(fd_clnt,"RQST_BTP",8,MSG_DONTWAIT);
                    cnt = 0;
                    prs_typ = PRS_TYP_BTP;
                    dbg_print(Bold_Yellow,"Requested Boot-up packet.\n\n");
                    print_btp_rcrd(&obj);
                    print_periodic_rcrd(&obj);
                }
            }
            else
            {
                get_prd_rcrd(dest,ret,rt_prsMdm,&obj,parse_func,pkt_typ);
                print_periodic_rcrd(&obj);
                dbg_print(Bold_Yellow,"Periodic record : \n");
                print_btp_rcrd(&obj);
            }
            
            printf("-------- Router data has been parsed and stored in the required field --------\n");
        }
        msleep(10);

    }
}
