//#include <modem/modem.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h> 
#include <errno.h> 
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>


struct termios tty;


int mdm_init(int ttyFD)
{
	memset (&tty, 0, sizeof tty);
    
    ttyFD = open("/dev/ttyAT", O_RDWR| O_NONBLOCK | O_NDELAY );
    if(ttyFD < 0)
    {
        perror("open() failed due to :");
        printf("Exiting... .. .\n");
        exit(0);
    }
    else
    {
		printf("AT port opened successfully\n");
	}
	if (tcgetattr ( ttyFD, &tty ) != 0 )
    {
        printf("Error from tcgetattr\n");
    }
    cfsetospeed (&tty, B115200);
    cfsetispeed (&tty, B115200);
    printf("i-Speed = %d, o-speed = %d\n",cfgetospeed(&tty), cfgetispeed(&tty));
    tty.c_cflag     &=  ~PARENB;                            // parity 
    tty.c_cflag     &=  ~CSTOPB;                            // stop bits 
    //tty.c_cflag   &=  ~CSIZE;                             // 
    tty.c_cflag     |=  CS8;                                // data bits 
    tty.c_cflag     &=  ~CRTSCTS;                           // no hardware flow control 
    tty.c_iflag     &=  ~(IXON | IXOFF | IXANY);            // no s/w flow ctrl 
    tty.c_lflag     =   0;                                  // non canonical 
    tty.c_oflag     =   0;                                  // no remapping, no delays 
    tty.c_cc[VMIN]  =   0;                                  // read doesn't block 
    tty.c_cc[VTIME] =   0;                                  // read timeout 
    tty.c_cflag     |=  CREAD | CLOCAL;                     // turn on READ & ignore ctrl lines 
    tty.c_lflag     &=  ~(ICANON | ECHO | ECHOE | ISIG);    
    //tty.c_oflag     &=  ~OPOST;                           
    
    tcflush( ttyFD, TCIFLUSH );
    if ( tcsetattr ( ttyFD, TCSANOW, &tty ) != 0)
    {
        printf("Error from tcsetattr\n");
    }
    return ttyFD;

}


/******************************
 * Return :
 *  Exit    : 2
 *  Success : 1 ( Send to modem )
 *  Error   : 0
 * ***************************/
uint8_t rd_ATcmd(char *cmd, const uint8_t sz)
{
    uint8_t ret = 0;
    char *ptr1=NULL;
    char *ptr2=NULL;
    char *ptr3=NULL;
    memset(cmd,0,sz);
    printf("Enter the AT command : ");
    scanf("%s",cmd);
    ptr1 = strchr(cmd,'\n');
    ptr2 = strchr(cmd,'\r');
    if (ptr1 != NULL)
    {
        ptr1 = '\0';
    }
    if (ptr1 != NULL)
    {
        ptr1 = '\0';
    }
    if (strstr(cmd,"AT") == NULL && strstr(cmd,"at")==NULL )
    {
        printf("\n\rERR-AT : Command received is not AT cmd.\n\r");
        ret = 0;
    }
    else if(cmd[0]=='q' || cmd[0]=='Q')
    {

    }
    else
    {
        strcat(cmd,"\r");
    }
}

int open_ATport()
{

}

/***************************************************
 * Arg : 
 *  1 (clean the fd and open for read/write)
 *  0 (Turn off the file descripter for read/write)
 * Ret :
 *  Success :
 *      1 : Active. open for R/W
 *      0 : Sleep. Can not R/W
 *  Error : 
 *      -1
 **************************************************/
// int fd_ctrl(int fd, int ctrl)
// {
//     if (ctrl == 1)
//     {
//         tcflush( fd, TCIOFLUSH);
//         tcflow(fd,);
//     }
//     else if(ctrl == 0)
//     {
//         tcflow(fd,);
//     }
    
//     if ( tcsetattr (fd, TCSANOW, &tty ) != 0)
//     {
//         printf("Error from tcsetattr\n");
//         return 1;
//     }
// }

/**************************
 * read the string
 * Arguments : 
 *      1. src_atr : "\n\rOK\n\r"
 * Replace \n with "<NL>" and "\r" with "<CR>"
 * ***********************/
void print_mdm(char *src_str)
{
    int len = strlen(src_str);
    for(int i=0;i<len;i++)
    {
        if(src_str[i] == '\n')
        {
            printf("<NL>");
        }
        else if(src_str[i] == '\r')
        {
            printf("<CR>");
        }
        else
        {
            printf("%c",src_str[i]);
        }        
    }
    printf("\n\r");
}

uint8_t get_nth_line(char *src)
{
	int count=0;
	char *ptr = strrchr(src,'\n');
	while (src != ptr)
	{
		if(*src == '\n')
		count++;
		src++;
	}
	return count;
}


/*******************************
 * 
 * 
 * 
 * ****************************/
uint8_t Mrsp_chk(char *src, char *dest)
{
	int sz;
	char cr = '\r', nl = '\n';
	char *ptr = strrchr(src,'\n');
	char *sptr = strchr(src,nl);
	long int start, end;
	int count=0;
	
	if (ptr != NULL) 
	{
		if(sptr != NULL)
		{
			--ptr;
			while(*ptr != '\n')
			{
					--ptr;
			}
			if(strstr(ptr,"OK"))
			{
				count = get_nth_line(src);
				printf("Number of times 'new line' comes in response= %d\n", count);
				if(count > 2)
				{
					while((*ptr != '\n') && (*ptr != '\r'))
					{
						--ptr;
					}
					++sptr;
					start = (long int)sptr;
					end = (long int)ptr;
					sz = end-start;
					strncpy (dest, sptr, sz);
					return 1;
				}
				else
				{
					//printf("OK\n");
					return 2;
				} 
			}
			else
			{
				//printf("ERROR\n");
				return 0;
			}
		}
	}
}

int8_t mdm_response(int ttyFD,char *response)
{
	
	char read_buf[256];

	/*i_WriteBytes = write(ttyFD, SampleCmd, sizeof(SampleCmd) -1 );
    printf("written bytes = %d\n", i_WriteBytes);
    if (i_WriteBytes < 0)
    {
		perror("Write() failed due to:\n");
		return 1;
	}*/
	for(int i=0;i<5;i++)
	{
		memset(&read_buf, '\0', 256);
		int num_bytes = read(ttyFD, &read_buf, 256);
		printf("Read count = %d\n\r",num_bytes);
		if (num_bytes < 0) 
		{
			printf("Error reading: %s", strerror(errno));
		}        
		else if(num_bytes == 0)
		{
			printf("No response from modem. Waiting for response now.\n\r");
			sleep(1);
			if(i == 4)
			{
				i = 6;
			}
		}
		else
		{
			/*switch(ch)
			{
				case 1:
					print_mdm(read_buf);
				break;
				case 0:
					//printf("%s",read_buf);
					Mrsp_chk(read_buf,response);
					printf("%s",response);
				break;

			}*/
			printf("\n\rresponse in read_buf: %s",read_buf);
			printf("\n\rMrsp_chk() = %d\n\r",Mrsp_chk(read_buf,response));
			printf("\n\rFinal response: %s",response);
		}
	}
}

int main(int cnt, const char *arg[])
{
    unsigned char SampleCmd[50] = {0};
    char response[256]={0};
    int sz_cmd = 0;
    int i_WriteBytes = 0;
    int ttyFD = 0;

    if (cnt == 2)
    {
        strcpy(SampleCmd,arg[1]);
        printf("cnt = %d\n",cnt);
        printf("arg[1] = %s\n",arg[1]);
        sz_cmd = strlen(SampleCmd);
        SampleCmd[sz_cmd] = '\r';
        SampleCmd[sz_cmd+1] = 0;
        printf("SampleCmd = %s --\n",SampleCmd);
    }
  
    /*printf("Please Enter the response type : \n");
    printf("0 - with end line charactes. \n 1 - Without end line characters.\n");
    printf("Enter choice : ");
    scanf("%hhd",&ch);
    if( (ch != 1 ) && (ch != 0) )
    {
        printf("Wrong choice.\n");
        printf("Setting default to '0'\n");
        ch = 0;
    }*/
    ttyFD = mdm_init(ttyFD);
    
    while(1)
    { 
		rd_ATcmd(SampleCmd,strlen(SampleCmd));
		printf("\nCMD : %s\n",SampleCmd);
		i_WriteBytes = write(ttyFD, SampleCmd, sizeof(SampleCmd) -1 );
		printf("written bytes = %d\n", i_WriteBytes);
		if (i_WriteBytes < 0)
		{
			perror("Write() failed due to:\n");
			return 1;
		}
        mdm_response (ttyFD,response);
        memset(response,0,strlen(response));
        memset(SampleCmd,0,strlen(SampleCmd));
    }  
    
    close(ttyFD);
    return 0;
}


