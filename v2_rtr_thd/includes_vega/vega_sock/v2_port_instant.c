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

#ifndef SO_REUSEPORT
	#define SO_REUSEPORT 15
#endif

#define TH_BUF 1024
#define IN_BUF  200
#define PORT_40_CMD 40
#define PORT_41_PER 41
#define DEV_IP "10.1.1.107"

#define IP_SZMIN    ( 3+(4*1) )
#define IP_SZMAX    ( 3+(4*3) )


int fd_in = -1;
struct termios tty;

void msleep(const unsigned int mili)
{
    int i;
    for(i=0;i<mili;i++)
    {
        usleep(1000);
    }
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
				printf("\nConnection Failed \n"); 
				printf("connect() failed due to : %s\n",strerror(errno));
				printf("Error no = %d\n",errno);
				if (errno == 111)
					printf("Server is not available\n");
			}
			msleep(300);
		}
		while( (c < 0) );
        ret = 0;
	}
	return ret;
}


/********************************************************
 * Return : 
 *  Success = count
 *  Error   = -1
 * *****************************************************/
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

/***************************************************
 * For flt : 0
 *      get the integers only
 * For flt : 1
 *      get the decimal(multy dots are valid Ex : 1.23.4) and digits as well.
 * For flt : 2
 *      get the decimal(multy dots are invalid Ex : 1.23.4) and digits as well.
 *       
 * ************************************************/
int str2numstr(char *str, char *numstr[], int len, u_int32_t flt)
{
    int sz = strlen(str);
    int c = 0, j =0;
    char *st, *end;
    int brk=0,i;
    int dot = 0;
    int no_dot=0;
    uint32_t c0=0, c1=0;
    if(flt==0)
    {
      dot=3;
    }
    else if(flt != 1)
    {
      return 0;
    }
    for(i=0;i<sz;i++)
    {
        // puts("here3");
        if( (str[i] >= '0') && (str[i] <= '9') )
        {
            if( (i+1) < sz )
            {
                (str[i] != '.') ? (no_dot=1) : (no_dot=0) ;
            }
            if( (dot==2) && flt )
            {
              dot = 3; //Observed a decimal number.
            }
            if( j==0 )
            {
                if( c < len )
                {
                    numstr[c++] = &str[i];
                    //printf("start num = %c idx = %d\n",str[i],i);
                }
                else
                {
                    brk = 1;
                }
                
            }

            if(i==0)
            {
                j = -1;
            }
            else
            {
                j = i;
            }
        }
        else if( ( ( (i-j==1) || (j==-1) ) &&  (str[i] == '.' )  )  && flt )
        {
            dot = 2;
        }
        else if( ( ( (i-j) == 1) || (j==-1) ) && ( (dot == 3) ||  no_dot ) )
        {
            j = 0;
            str[i] = 0;
            if(flt)
            {
              dot = 0;
            }
        }
        // printf("i-j=%d; j=%d\n",i-j,j);

        // if(brk)
        // {
        //     break;
        // }
    }
    // printf("C value: %d\n",c);
    return c;
}


/*********************
 * Return : 
 *  valid IP   : 0
 *  Invalid IP : -1
 ********************/
uint32_t validate_ip(const char *ip)
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
        ret = str_split(ip_dup,".",num,4);
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
        printf("connect() failed due to : %s\n",strerror(errno));
        printf("errno = %d\n",errno);
        printf("sizeof(*srvr_addr) = %ld\n",sizeof(*srvr_addr));
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
        printf("ERR : socket() failed due to : %s\n",strerror(errno));
        printf("Exiting... .. .\n");
        return -1;
    }
    ret = fcntl(*fd,F_SETFD,SOL_SOCKET | SO_REUSEADDR \
                | SO_REUSEPORT | SO_KEEPALIVE | O_NONBLOCK);
    do
    {
        rt_connect = connect(*fd,(struct sockaddr*)srvr_addr,sizeof(*srvr_addr));
        sleep(1);
        printf("connect() failed due to : %s\n",strerror(errno));
    } while (rt_connect < 0);
    printf("\n-------Connection re-established------\n");
    return 0;
}

int main(void)
{
    struct sockaddr_in addr;
    int fd_clnt = 0, rt_rcv=-1, rt_snd=-1;
    char buf[TH_BUF] = {0};
    char buf_stdin[] = {0};
    fd_clnt = start_clnt(&addr,DEV_IP,PORT_40_CMD);
    int fd_stdin = 0, rd_stdin=0;
    if(fd_clnt < 0)
    {
        printf("start_clnt() failed.\n");
        printf("Returning... .. .\n");
        return -1;
    }
    fd_stdin = open("/dev/stdin",\
        O_RDONLY| O_NONBLOCK | O_NDELAY);
    if(fd_stdin < 0)
    {
        printf("open() failed due to : %s\n",\
                strerror(errno));
        printf("Exiting... .. .\n");
        return -1;
    }
    while(1)
    {
        memset(buf,0,strlen(buf));
        // if(rd_stdin == 0)
        // {
        //     printf("\nEnter command : \n");
        //     rd_stdin = 1;
        // }
        rt_rcv = read(fd_stdin,buf,IN_BUF);
        if(rt_rcv > 0)
        {
            send(fd_clnt,buf,strlen(buf),MSG_DONTWAIT);
            memset(buf,0,strlen(buf));
            rd_stdin = 0;
        }
        rt_rcv = recv(fd_clnt,buf,TH_BUF,MSG_DONTWAIT);
        if(rt_rcv == 0)
        {
            printf("\n------ Disconnected from the server ------\n");
            printf("Reconnecting ... .. .\n");
            restart_clnt(&fd_clnt,DEV_IP,&addr);
            rd_stdin = 0;
        }
        else if(rt_rcv > 0)
        {
            printf("%s\n",buf);
            rd_stdin = 0;
        }
        msleep(10);
    }
}

/*   
int main(void) 
{ 
    int sock = 0, valread; 
    int ret = 0, c = 0;
    int opt = 1;
    struct sockaddr_in serv_addr; 
    char buffer[1024] = {0}; 
    char str_ip[] = {"10.1.1.107"};
    
    prcs_ip(str_ip);
    if(validate_ip(str_ip)== 0)
    {
        printf("Invalid IP. Exiting...\n");
        exit(EXIT_FAILURE);
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\nsocket() failed due to : %s\n",strerror(errno)); 
        perror("socket() failed due to :");
        return -1;
    } 

    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    }
    uint8_t tm=0;
	int fd = 0;
	fd = fcntl(sock,F_SETFD,SOL_SOCKET | SO_REUSEADDR | SO_REUSEPORT | SO_KEEPALIVE);
	printf("fntl = %d\n", fd);
	if (fd < 0)
		return -1;
    do 
    {		
		c = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
		printf("c = %d\n",c);
		if ( c < 0 ) 
		{ 
			printf("\nConnection Failed \n"); 
			printf("\nconnect() failed due to : %s\n",strerror(errno)); 
			printf("Error no = %d\n",errno);
			if (errno == 111)
				printf("Server is not available\n");
		}
		//msleep(500);
	}
	while( (c < 0) );
    memset (&tty, 0, sizeof tty);
    
	fd_in = open("/dev/stdin", O_RDWR| O_NONBLOCK | O_NDELAY );
	if(fd_in < 0)
	{
		perror("open() failed due to :");
	}
	else
	{
		printf("open() success.....\n");
	}
    opt = 0;
    while(1)
    {
        if(opt == 0)
        {
            printf("\nEnter command : \n");
            opt = 1;
        }
		if(strlen(buffer) > 0 )
			memset(buffer,0,sizeof(buffer));
		 
		ret = read(fd_in, buffer,1024);
		if(ret > 0)
		{
			send(sock , buffer,sizeof(buffer) , MSG_DONTWAIT );
			memset(buffer,0,sizeof(buffer));
			ret = 0;
		}
		valread = recv(sock, buffer,sizeof(buffer),MSG_DONTWAIT);
		if(valread == 0)
		{
			printf("Server disconnected.....\n");
			// close (sock);
			//goto CONNECT;
			if ( sock_reconnect(&sock,&serv_addr) == 0)
            {
                 printf("---- Connection re-established. ------\n");
                 opt = 0;
            }
		} 
		else if(valread > 0)
		{
			printf("%s\n",buffer );
            opt = 0;
        }
		
	}
		return 0; 
} 

*/
