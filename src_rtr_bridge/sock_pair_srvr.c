#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT_80 80
#define PORT_90 90
#define TM_OUT

#define BUF_LEN ( 1024 * 300 )
int set_sock_opt(int *fd_srvr, int sock_no, unsigned char flg);
void print_time(void);
int get_accept_sts(int *rt_connect, int no);
int start_srvr(int *fd_sock, int *fd_clnt, const struct sockaddr_in *addr, \
                unsigned char sock_no, unsigned char flg);
static int fd_srvr[2] = {0};
static int fd_clnt[2] = {0};
static int ret = -1,i;
static int rt_rcv=-1, rt_snd = -1;
static char err_msg[200] = {0};
static struct sockaddr_in addr[2];
static int len_addr = sizeof(addr[0]);
static char buf[BUF_LEN] = {0};


int main(void)
{
    for(int i=0;i<2;i++)
    {
        memset(&addr[i],0,len_addr);
        fd_srvr[i] = socket(AF_INET, \
                    SOCK_STREAM | SOCK_NONBLOCK, 0 );
        if(fd_srvr[i] == -1)
        {
            sprintf(err_msg,"Socket-%d failed due to : ",i);
            perror(err_msg);
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("fd_srvr[%d] = %d\n",i,fd_srvr[i]);
        }
    }

    if( set_sock_opt(fd_srvr,1,0) > 0 )
    {
        printf("setsockopt() succedded\n");
    }

    addr[0].sin_family = AF_INET;
    addr[0].sin_addr.s_addr = INADDR_ANY;
    addr[0].sin_port = htons(PORT_80);

    addr[1] = addr[0];
    addr[1].sin_port = htons(PORT_90);

    LVL_BND_LSN :;
    for(i=0;i<2;i++)
    {
        ret = bind(fd_srvr[i], (struct sockaddr*)&addr[i], \
                len_addr);
        if (ret == -1)
        {
            memset(err_msg,0,strlen(err_msg));
            sprintf(err_msg,"bind(fd_srvr[%d]) failed due to ",i);
            perror(err_msg);
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("bind(%d) succedded\n",i);
            printf("bind(%d) = %d\n",i,ret);
        }
        
        ret = listen(fd_srvr[i],3);
        if (ret == -1)
        {
            memset(err_msg,0,strlen(err_msg));
            sprintf(err_msg,"listen(%d) has failed due to : ",i);
            perror(err_msg);
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("listen(%d) has succedded\n",i);
            printf("listen(%d) = %d\n",i,ret);
        }
    }

    fd_clnt[0] = fd_clnt[1] = -1;
    int cnt;
    LVL_ACCEPT : ;
    print_time();
    cnt = get_accept_sts(fd_clnt,2);
    do
    {
        for(i=0;i<2;i++)
        {
            if(fd_clnt[i] < 0)
            {
                fd_clnt[i] = \
                accept(fd_srvr[i],(struct sockaddr*)&addr[i],\
                    (socklen_t*)&len_addr);
                if (fd_clnt[i] < 0)
                {
                    memset(err_msg,0,strlen(err_msg));
                    sprintf(err_msg,"connect(%d) failed due to : ",i);
                    perror(err_msg);
                    printf("Error no : %d\n",errno);
                }
                else if (fd_clnt[i] > 0)
                {
                    cnt += 1;
                }
            }
        }
        sleep(2);
    }
    while( cnt < 2 );

    print_time();
    printf("cnt = %d\n",cnt);
    printf("fd_clnt[0] = %d\n",fd_clnt[0]);
    printf("fd_clnt[1] = %d\n",fd_clnt[1]);
    printf("----------- connected -----------\n");
    
    rt_rcv = rt_snd = -1;
    while(1)
    {
        for(i=0;i<2;i++)
        {
            rt_rcv = recv(fd_clnt[i],buf,BUF_LEN,MSG_DONTWAIT);
            //printf("read_client-%d = %d\n",i,rt_rcv);
            if (rt_rcv > 0)
            {
                printf("Writing to sock[%d] : %d\n",i,rt_rcv);
                rt_snd = send(fd_clnt[!i],buf,rt_rcv,MSG_DONTWAIT);
                if(rt_snd > 0)
                {
                    memset(buf,0,rt_snd);
                }
            }
            else if (rt_rcv == 0 )
            {
                printf(" --- rt_rcv = %d ----- \n",rt_rcv);
                printf("------- sock-%d disconnected ------\n",i);
                //printf("Close(srvr-%d) =  %d \n",i,close(fd_srvr[i]));
                printf("Close(clnt-%d) = %d\n",i,close(fd_clnt[i]));
                //start_srvr(fd_srvr,fd_clnt,addr,i,1);
                //printf("------- Connection re-established ------\n");
                fd_clnt[i] = -1;
                goto LVL_ACCEPT;
            }
        }
    }

}



/*
int set_sock_opt(int *fd_srvr)
{
#ifdef TM_OUT
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 1000;
#endif
    int ret = 0, opt = 1;
    ret = setsockopt(fd_srvr[0],SOL_SOCKET, SO_KEEPALIVE | \
            SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    if(ret == -1)
    {
        perror("\nsetsockopt(fd_srvr[0]) failed due to :");
        exit(EXIT_FAILURE);
    }
    ret = setsockopt(fd_srvr[1],SOL_SOCKET, SO_KEEPALIVE | \
            SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    if(ret == -1)
    {
        perror("\nsetsockopt(fd_srvr[1]) failed due to :");
        exit(EXIT_FAILURE);
    }
#ifdef TM_OUT    
    ret = setsockopt(fd_srvr[0],SOL_SOCKET, \
            SO_RCVTIMEO | SO_SNDTIMEO, \
             (void*)&timeout, sizeof(timeout) );
    if (ret == -1)
    {
        perror("\nsetsockopt(fd_srvr[0]) <time out> failed due to ");
        exit(EXIT_FAILURE);
    }
    ret = -1;
    ret = setsockopt(fd_srvr[1],SOL_SOCKET, \
            SO_RCVTIMEO | SO_SNDTIMEO, \
            (char*)&timeout, sizeof(timeout) );
    if (ret == -1)
    {
        perror("\nsetsockopt(fd_srvr[1]) <time out> failed due to ");
        exit(EXIT_FAILURE);
    }
#endif    
    return 0;
}
*/

void print_time(void)
{
    time_t tm = time(NULL);
    printf("Current time : %s\n",ctime(&tm));
}


/************************
 * if flg == 0
 *      set options from 0 - sock_no
 * if flg == 1
 *      set options only for sock_no
 * for other values of flg, return = -1
 * 
 *      *fd_srvr = the array
 * *********************/

int set_sock_opt(int *fd_srvr, int sock_no, unsigned char flg)
{
#ifdef TM_OUT
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 1000;
#endif

    int ret = 0, opt = 1, cnt = 0,i;
    if (flg == 0) // set sock_opt from 0 to sock_no
    {
        i = 0;
        sock_no += 1;
    }
    else if(flg == 1) // set sock_opt only for sock_no
    {
        i = sock_no;
        sock_no += 1;        
    }
    else
    {
        return -1;
    }

    for(;i<sock_no;i++)
    {
        ret = setsockopt(fd_srvr[i],SOL_SOCKET, SO_KEEPALIVE | \
            SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
        if(ret == -1)
        {
            memset(err_msg,0,strlen(err_msg));
            sprintf(err_msg,"setsockopt(fd_srvr[%d]) failed due to :",i);
            perror(err_msg);
            exit(EXIT_FAILURE);
        }
        #ifdef TM_OUT
            ret = setsockopt(fd_srvr[i],SOL_SOCKET, \
                SO_RCVTIMEO | SO_SNDTIMEO, \
                (void*)&timeout, sizeof(timeout) );
            if (ret == -1)
            {
                memset(err_msg,0,strlen(err_msg));
                sprintf(err_msg,"setsockopt(fd_srvr[%d]) <time out> failed due to :",i);
                perror(err_msg);
                
                exit(EXIT_FAILURE);
            }
        #endif
        if (ret == 0)
            cnt += 1;
    }
    return cnt;
}

int get_accept_sts(int *rt_connect, int no)
{
    int ret = 0,i;
    for(i=0;i<no;i++)
    {
        if (rt_connect[i] == 0)
        {
            ret += 1;
        }
    }
    return ret;
}

int start_srvr(int *fd_srvr, int *fd_clnt, const struct sockaddr_in *addr, \
                unsigned char sock_no, unsigned char flg)
{
    int i=0,cnt=0, ret=0;
    if(flg == 0)
    {
        i=0;
        sock_no += 1;
    }
    else if(flg == 1)
    {
        i = sock_no;
        sock_no += 1;
    }
    else
    {
        printf("Wrong argument to 'flg(%d)'\n",flg);
        return -1;
    }

    for(;i<sock_no;i++)
    {
        fd_srvr[i] = socket(AF_INET, \
                    SOCK_STREAM | SOCK_NONBLOCK, 0 );
        if(fd_srvr[i] == -1)
        {
            sprintf(err_msg,"Socket-%d failed due to : ",i);
            perror(err_msg);
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("fd_srvr[%d] = %d\n",i,fd_srvr[i]);
        }

        if( set_sock_opt(fd_srvr,i,1) > 0 )
        {
            printf("set_sock_opt(%d) succedded.\n",sock_no);
        }

        ret = bind(fd_srvr[i], (struct sockaddr*)&addr[i], \
                sizeof(addr[i]));
        if (ret == -1)
        {
            memset(err_msg,0,strlen(err_msg));
            sprintf(err_msg,"bind(fd_srvr[%d]) failed due to ",i);
            perror(err_msg);
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("bind(%d) succedded\n",i);
            printf("bind(%d) = %d\n",i,ret);
        }
        
        ret = listen(fd_srvr[i],3);
        if (ret == -1)
        {
            memset(err_msg,0,strlen(err_msg));
            sprintf(err_msg,"listen(%d) has failed due to : ",i);
            perror(err_msg);
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("listen(%d) has succedded\n",i);
            printf("listen(%d) = %d\n",i,ret);
        }
    }
    cnt = get_accept_sts(fd_clnt,2);
    do
    {
        for(i=0;i<2;i++)
        {
            if(fd_clnt[i] < 0)
            {
                fd_clnt[i] = \
                accept(fd_srvr[i],(struct sockaddr*)&addr[i],\
                    (socklen_t*)&len_addr);
                if (fd_clnt[i] < 0)
                {
                    memset(err_msg,0,strlen(err_msg));
                    sprintf(err_msg,"connect(%d) failed due to : ",i);
                    perror(err_msg);
                    printf("Error no : %d\n",errno);
                }
                else if (fd_clnt[i] > 0)
                {
                    cnt += 1;
                }
            }
        }
        sleep(1);
    } while (cnt < 2);
    return cnt;
}
