/**********************************
 * Date : 03/09/20
 * Author : Guruprasad
 * Purpose : Local communication
 * *******************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>
#include <sys/time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT_30 30
#define BF_LEN  200
#define TBF_LEN 10
#define TM_OUT
#define MSG "\nFrom server :"
#define LN_MSG strlen(MSG)


static int fd_srvr = 0;
static int fd_clnt = 0;
static int rt_rcv=0, rt_snd=0;
static struct sockaddr_in addr;
static unsigned long len_addr = sizeof(addr);
static char tmp_buf[TBF_LEN] = {0};
static char buf[BF_LEN] = {0};

void msleep(const unsigned int mili);
uint8_t update_mem(char *mem, const char *srs, \
                    uint8_t *mem_sts, uint8_t flg);
int set_sock_opt(int fd_srvr);
int start_srvr( int *fd_sock, int *fd_clnt, \
                struct sockaddr_in *addr, \
                unsigned long len_addr);
void sk_flush(int fd_sk);

int main(void)
{
    int sts = start_srvr(&fd_srvr,&fd_clnt,&addr,len_addr);
    if (sts < 0)
    {
        printf("ERR : start_srvr() failed. \n\r");
        printf("Exiting : ");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Communication established.\n\r");
        printf("fd_clnt = %d\n",fd_clnt);
        rt_rcv = rt_snd = -1;
        while(1)
        {
            rt_rcv = recv(fd_clnt,tmp_buf,TBF_LEN,MSG_DONTWAIT);
            msleep(100);
            if(rt_rcv > 0)
            {
                printf("Writing to clnt...\n\r");
                rt_snd = send(fd_clnt,MSG,LN_MSG,0);
                rt_snd = send(fd_clnt,tmp_buf,rt_rcv,0);
                memset(tmp_buf,0,strlen(tmp_buf));
            }
            if (rt_rcv == 0)
            {
                close(fd_clnt);
                close(fd_srvr);
                rt_rcv = rt_snd = -1;
                sts = start_srvr(&fd_srvr,&fd_clnt,&addr,len_addr);
                if(sts > 0)
                {
                    printf("Communication re-established.\n");
                }
            }
        }
    }
    
}


/*************************************
 * Arg : 
 *  flg
 *    1 - concatinate
 *    2 - copy
 *    3 - clean & copy
 * **********************************/
uint8_t update_mem(char *mem, const char *srs, \
                    uint8_t *mem_sts, uint8_t flg)
{

}
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

void msleep(const unsigned int mili)
{
    for(int i=0;i<mili;i++)
    {
        usleep(1000);
    }
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
                printf("Exiting ... .. .\n")                ;
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
                struct sockaddr_in *addr, unsigned long len_addr)
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
            addr->sin_port = htons(PORT_30);
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
                        printf("Wainting count : %d\n",cnt);
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
