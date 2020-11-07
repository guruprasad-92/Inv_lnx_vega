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

#ifndef SO_REUSEPORT
    #define SO_REUSEPORT 15
#endif

//#define USE_NONBLK
#define BUF_LEN ( 1024 * 300 )
#define IP_SZMIN    ( 3+(4*1) )
#define IP_SZMAX    ( 3+(4*3) )

int set_sock_opt(int *fd_srvr, int sock_no);
void print_time(void);
void prcs_ip(char *str);
int str_split(char *str, const char *dlm, char *ptr[],int max_cnt);
unsigned char str_isNumeric(const char *str);
unsigned char validate_ip(const char *ip);
void prcs_ip(char *str);
int get_connect_sts(int *rt_clnt);
int reconnect(int *fd_sock, const struct sockaddr_in *addr, unsigned char sock_no);

static char err_msg[200] = {0};

int main(int c, char *arg[])
{
    int fd_clnt[2] = {0};
    int rt_connect[2] = {0};
    int rt_rcv=-1, rt_snd=-1, ret = -1, i =0;

    
    char *ip[4]= {0};

    struct sockaddr_in addr[2];
    int len_addr = sizeof(addr[0]);
    char buf[BUF_LEN] = {0};

    int cnt = 0;

    if(c < 3)
    {
        printf("Not enough argumrnts.\n");
        exit(EXIT_FAILURE);
    }
    else if (c > 3)
    {
        printf("Too many arguments.\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        for(i=1;i<3;i++)
        {
            prcs_ip(arg[i]);
            if(validate_ip(arg[i]))
            {
                cnt += 1;
            }
        }

        if(cnt != 2)
        {
            printf("Invalid IP. Exiting...\n");
            exit(EXIT_FAILURE);
        }
    }
    

    for(i=0;i<2;i++)
    {
        memset(&addr[i],0,len_addr);
        #if defined USE_NONBLK
            fd_clnt[i] = socket(AF_INET, \
                        SOCK_STREAM | SOCK_NONBLOCK, 0 );
        #else
            fd_clnt[i] = socket(AF_INET, SOCK_STREAM, 0 );
        #endif
        if(fd_clnt[i] == -1)
        {
            sprintf(err_msg,"Socket-%d failed due to : ",i);
            perror(err_msg);
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("fd_clnt[%d] = %d\n",i,fd_clnt[i]);
        }        
    }

    if( set_sock_opt(fd_clnt,2) == 2 )
    {
        printf("setsockopt() succedded\n");
    }
    

    addr[0].sin_family = AF_INET;
    addr[0].sin_port = htons(PORT_90); //remote-server

    addr[1] = addr[0];
    addr[1].sin_port = htons(PORT_80); // web-server

    for(i=0;i<2;i++)
    {
        ret = inet_pton(AF_INET, arg[1+i],&addr[i].sin_addr);
        if(ret == -1)
        {
            memset(err_msg,0,strlen(err_msg));
            sprintf(err_msg,"inet_pton(%d) failed due to : ",i);
            perror(err_msg);
            exit(EXIT_FAILURE);
        }
    }
    printf("IP has been processed for communication.\n");

    rt_connect[0] = -1;
    rt_connect[1] = -1;
    cnt = 0;
    CONNECT : ;
    
    cnt = get_connect_sts(rt_connect);
    printf("cnt = %d\n",cnt);
    do
    {
        for(i=0;i<2;i++)
        {
            if(rt_connect[i] < 0)
            {
                rt_connect[i] = connect(fd_clnt[i], \
                                (struct sockaddr*)&addr[i], len_addr);
                if(rt_connect[i] < 0)
                {
                    if(errno == EISCONN)
                    {
                        perror("");
                        cnt += 1;
                        rt_connect[i] = 0;
                    }
                    else
                    {
                        memset(err_msg,0,strlen(err_msg));
                        sprintf(err_msg,"connect(%d) failed due to : ",i);
                        perror(err_msg);
                        printf("Error no : %d\n",errno);
                        //exit(EXIT_FAILURE);
                    }
                    
                    
                }
                else
                {
                    printf("connect(clnt-%d) succedded.\n",i);
                    cnt += 1;
                }
            }
            printf("rt_connect[%d] = %d\n",i,rt_connect[i]);
            printf("cnt = %d\n",cnt);
        }
        sleep(2);
    } while ( cnt < 2);
    
    //---------- Perform read write operation ---------------
    printf("--------------- CONNECTED -----------------\n");
    rt_rcv = rt_snd -1;
    while(1)
    {
        for(i=0;i<2;i++)
        {
            rt_rcv = recv(fd_clnt[i],buf,BUF_LEN,MSG_DONTWAIT);
            //printf("read-%d = %d\n",i,rt_rcv);
            if (rt_rcv > 0)
            {
                printf("Writing to sock[%d] : %d\n",i,rt_rcv);
                rt_snd = send(fd_clnt[!i],buf,rt_rcv,MSG_DONTWAIT);
                if(rt_snd > 0)
                {
                    memset(buf,0,rt_snd);
                }
            }
            else if(rt_rcv == 0)
            {
                print_time();
                printf("Disconnected from socket(%d)\nRetrying....\n",i);
                
                if ( reconnect(&fd_clnt[i],&addr[i],1) == 0)
                {
                    printf("---- Connection re-established. ------\n");
                }
                                
            }
        }
    }
}


int set_sock_opt(int *fd_srvr, int sock_no)
{
#ifdef TM_OUT
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 1000;
#endif

    int ret = 0, opt = 1, cnt = 0,i;
    for(i=0;i<sock_no;i++)
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

void print_time(void)
{
    time_t tm = time(NULL);
    printf("Current time : %s\n",ctime(&tm));
}

int str_split(char *str, const char *dlm, char *ptr[],int max_cnt)
{
    int cnt = 0;
    if(strstr(str,dlm) != NULL)
    {
        do
        {
            if(cnt==0)
                ptr[cnt] = strtok(str,dlm);
            else
            {
                ptr[cnt] = strtok(NULL,dlm);
            }
            if(cnt > max_cnt)
            {
                break;
            }
            
        } while (ptr[cnt++] != NULL);
        cnt -= 1;
    }
    return cnt;
}


/*****************************
 * str is Numeric     : 1
 * str is not numeric : 0
 * **************************/
unsigned char str_isNumeric(const char *str)
{
    unsigned char cnt = 0, ret = 0;
    int i,len = strlen(str);
    for(i=0;i<len;i++)
    {
        if( (str[i] >= '0') && (str[i] <= '9') )
        {
            cnt += 1;
        }
    }
    if (len == cnt)
    {
        ret = 1;
    }
    return ret;
}

/*********************
 * Return : 
 *  valid IP   : 1
 *  Invalid IP : 0
 ********************/
unsigned char validate_ip(const char *ip)
{
    int len = strlen(ip);
    char ip_dup[IP_SZMAX] = {0};
    char *ptr[IP_SZMIN] = {0};
    int fld = 0, cnt = 0;
    int idx[4] = {0}, ret=0,i;

    memcpy(ip_dup,ip,len);
    if( (len >= IP_SZMIN) && (len <= IP_SZMAX) )
    {        
        fld = str_split(ip_dup,".",ptr,10);
        if (fld == 4)
        {
            for(i=0;i<4;i++)
            {
                if(str_isNumeric(ptr[i]))
                {
                    idx[i] = strtol(ptr[i],NULL,10);
                    if( (idx[i] >= 0) && (idx[i]<=255) )
                    {
                        cnt += 1;
                    }
                }
            }
            if(cnt == 4)            
            {
                ret = 1;
            }
        }
    }
    return ret;
}

void prcs_ip(char *str)
{
    int len = strlen(str),i;
    for(i=0;i<len;i++)
    {
        if( (str[i] == '\n') || (str[i]=='\r') )
        {
            str[i] = '\0';
        }
    }
}

int get_connect_sts(int *rt_connect)
{
    int ret = 0,i;
    for(i=0;i<2;i++)
    {
        if (rt_connect[i] == 0)
        {
            ret += 1;
        }
    }
    return ret;
}

int reconnect(int *fd_sock, const struct sockaddr_in *addr, unsigned char sock_no)
{
    int ret = -1;
    if (close(*fd_sock) == -1)
    {
        memset(err_msg,0,strlen(err_msg));
        sprintf(err_msg,"close() failed for socket-%d due to : ",sock_no);
        perror(err_msg);
        ret = -1;
    }
    else
    {
        #if defined USE_NONBLK
            *fd_sock = socket(AF_INET, \
                        SOCK_STREAM | SOCK_NONBLOCK, 0 );
        #else
            *fd_sock = socket(AF_INET, SOCK_STREAM, 0 );
        #endif
        if (*fd_sock == -1)
        {
            memset(err_msg,0,strlen(err_msg));
            sprintf(err_msg,"socket() failed for socket-%d due to : ",sock_no);
            perror(err_msg);
            ret = -1;
        }
        else
        {
            int arr_fd[1] = {*fd_sock};
            if(set_sock_opt(arr_fd,1)==1)
            {
                printf("set_sock_opt() succedded\n");
            }
            printf("Re connecting to sock-%d .... ... .. .\n",sock_no);
            do
            {
                ret = connect(*fd_sock,(struct sockaddr*)addr, sizeof(*addr));
            } while (ret < 0);
            printf("--- Reconnected to socket-%d ------\n",sock_no);
        }
    }
    return ret;
}
