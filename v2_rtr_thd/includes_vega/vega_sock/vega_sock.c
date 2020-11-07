#include <stdlib.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>



#include "vega_sock.h"
#include "basic.h"

int sock_reconnect(int *fd_sock, const struct sockaddr_in *serv_addr)
{
	int ret = -1;
	int c = 0;

    if (close(*fd_sock) == -1)
    {
       
        printf("close failed due to : %s\n",strerror(errno));
        ret = -1;
    }
    else
    {
		*fd_sock = socket(AF_INET, SOCK_STREAM, 0);
		if(*fd_sock < 0)
		{ 
       		printf("\nsocket() failed due to : %s\n",strerror(errno)); 
        	return -1; 
    	} 
		do 
		{		
			c = connect(*fd_sock, (struct sockaddr *)serv_addr, sizeof(serv_addr));
			printf("c = %d\n",c);
			if ( c < 0 ) 
			{ 
				dbg_print(Bold_Red,"\nConnection Failed \n"); 
				dbg_print(Bold_Red,"connect() failed due to : %s\n",strerror(errno));
				dbg_print(Bold_Red,"Error no = %d\n",errno);
				if (errno == 111)
					dbg_print(Bold_Red,"Server is not available\n");
			}
			msleep(300);
		}
		while( (c < 0) );
        ret = 0;
	}
	return ret;
}

/*********************
 * Return : 
 *  valid IP   : 0
 *  Invalid IP : -1
 ********************/
int validate_ip(const char *ip)
{
    char ip_dup[IP_SZMAX] = {0};
    char *num[5] = {0};
    int ret = 0, chk1 = 0, tmp=0;
    int sz,i;
    strcpy(ip_dup,ip);
    sz = strlen(ip_dup);
    
    for(i=0;i<sz;i++)
    {
        if(ip_dup[i]=='.')
            ret += 1;
        else if( (ip_dup[i] < '0') || (ip_dup[i]>'9') )
        {
            chk1 = 1;
            break;
        }
    }
    if( (ret != 3) || (chk1==1) )
    {
        return -1;
    }
    else if(ret == 3)
    {
        ret = split_string(ip_dup,num,".");
        if(ret == 4)
        {
            for(i=0;i<ret;i++)
            {
                tmp = atoi(num[i]);
                if( (tmp<=255) && (tmp>0) )
                {
                    chk1 += 1;
                }
            }
            if(chk1 == 4)
            {
                ret = 0;
            }
            else
            {
                ret = -1;
            }
        }
        else
        {
            ret = -1;
        }
    }
    
    return ret;
}



int start_clnt(struct sockaddr_in *srvr_addr, char *str_ip, int port)
{
    int ret = -1;
    int c = 0;
    int fd_clnt, rt_connect=0;
    if(validate_ip(str_ip)!=0)
    {
        printf("ERR : Invalid IP (%s)\n",str_ip);
        printf("Returning... .. .\n");
        return -1;
    }
    fd_clnt = socket(AF_INET, SOCK_STREAM , 0);
    if(fd_clnt < 0)
    {
        printf("ERR : sock() failed due to : %s\n",strerror(errno));
        printf("Exiting... .. .\n");
        return -1;
    }
    ret = fcntl(fd_clnt,F_SETFD,SOL_SOCKET | SO_REUSEADDR \
                | SO_REUSEPORT | SO_KEEPALIVE | O_NONBLOCK);
    
    srvr_addr->sin_family = AF_INET;
    srvr_addr->sin_port = htons(port);
    ret = inet_pton(AF_INET,str_ip, &srvr_addr->sin_addr);
    if(ret == -1)
    {
        printf("ERR-sock : inet_pton() failed due to : %s\n",\
                strerror(errno));
        printf("Exiting... .. .\n");
    }
    do
    {
        rt_connect = connect(fd_clnt,(struct sockaddr*)srvr_addr,sizeof(*srvr_addr));
        sleep(1);
        if(rt_connect < 0)
        {
            dbg_print(Bold_Red,"connect() failed due to : %s\n",strerror(errno));
            dbg_print(Bold_Red,"errno = %d\n",errno);
        }
    } while (rt_connect < 0);
    printf("Connected to the server... .. .\n");
    return fd_clnt;
}


int restart_clnt(int *fd, char *str_ip, struct sockaddr_in *srvr_addr)
{
    int ret = -1, rt_connect = -1;;
    close(*fd);
    *fd = socket(AF_INET, SOCK_STREAM , 0);
    if(*fd < 0)
    {
        dbg_print(Bold_Red,"ERR : socket() failed due to : %s\n",strerror(errno));
        dbg_print(Bold_Red,"Exiting... .. .\n");
        return -1;
    }
    ret = fcntl(*fd,F_SETFD,SOL_SOCKET | SO_REUSEADDR \
                | SO_REUSEPORT | SO_KEEPALIVE | O_NONBLOCK);
    do
    {
        rt_connect = connect(*fd,(struct sockaddr*)srvr_addr,sizeof(*srvr_addr));
        sleep(1);
        if(rt_connect < 0)
            dbg_print(Bold_Red,"connect() failed due to : %s\n",strerror(errno));
    } while (rt_connect < 0);
    dbg_print(Bold_Green,"\n-------Connection re-established------\n");
    return 0;
}

