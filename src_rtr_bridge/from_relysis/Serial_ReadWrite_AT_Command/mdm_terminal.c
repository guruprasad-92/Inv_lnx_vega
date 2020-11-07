#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <errno.h> 
#include <termios.h>
#include <unistd.h>

//#define RD_ONLY


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
        ret =  2;
    }
    else
    {
        strcat(cmd,"\r");
        ret = 1;
    }
    return ret;
}

int open_ATport()
{

}

int mdm_init(struct termios *tty)
{
	memset (tty, 0, sizeof *tty);
    int ttyFD;
    ttyFD = open("/dev/ttyAT", O_RDWR | O_NONBLOCK | O_NDELAY );
    if(ttyFD < 0)
    {
        perror("open() failed due to :");
        printf("Exiting... .. .\n");
        exit(0);
    }
    
	if (tcgetattr ( ttyFD, tty ) != 0 )
    {
        perror("tcgetattr() failed due to :");
        printf("Exiting... .. .\n");
        exit(0);
    }
    cfsetospeed (tty, B115200);
    cfsetispeed (tty, B115200);
    tty->c_cflag     &=  ~PARENB;                            // parity 
    tty->c_cflag     &=  ~CSTOPB;                            // stop bits 
    //tty.c_cflag   &=  ~CSIZE;                             // 
    tty->c_cflag     |=  CS8;                                // data bits 
    tty->c_cflag     &=  ~CRTSCTS;                           // no hardware flow control 
    tty->c_iflag     &=  ~(IXON | IXOFF | IXANY);            // no s/w flow ctrl 
    tty->c_lflag     =   0;                                  // non canonical 
    tty->c_oflag     =   0;                                  // no remapping, no delays 
    tty->c_cc[VMIN]  =   0;                                  // read doesn't block 
    tty->c_cc[VTIME] =   0;                                  // read timeout 
    tty->c_cflag     |=  CREAD | CLOCAL;                     // turn on READ & ignore ctrl lines 
    tty->c_lflag     &=  ~(ICANON | ECHO | ECHOE | ISIG);    
    //tty.c_oflag     &=  ~OPOST;                           
    
    tcflush( ttyFD, TCIFLUSH );
    if ( tcsetattr ( ttyFD, TCSANOW, tty ) != 0)
    {
        perror("tcsetattr() failed due to :");
        printf("Exinting... .. .\n\r");
        exit(0);
    }
    return ttyFD;
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
int fd_ctrl(int fd, int ctrl, struct termios *tty)
{

    int ret = 0;
    if (ctrl == 1)
    {
        tcflush( fd, TCIOFLUSH);
        tcflow(fd,TCION);
        tcflow(fd,TCOON);
        ret = 1;
    }
    else if(ctrl == 0)
    {
        tcflow(fd,TCIOFF);
        tcflow(fd,TCOOFF);
        ret = 0;
    }
    
    if ( tcsetattr (fd, TCSANOW, tty ) != 0)
    {
        perror("\n\rIN fd_ctrl(), tcsetattr() failed due to :");
        ret = -1;
    }
    return ret;
}

/**************************
 * read the string
 * Arguments : 
 *      1. src_atr : "\n\rOK\n\r"
 * Replace \n with "<NL>", "\r" with "<CR> and \t with <TB>"
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
        else if(src_str[i] == '\t')
        {
            printf("<TB>");
        }
        else
        {
            printf("%c",src_str[i]);
        }        
    }
    printf("\n\r");
}

int main(void)
{
	int ttyFD = 0;
    struct termios tty;
    unsigned char SampleCmd[50] = {0};
    int sz_cmd = 0;
    int i_WriteBytes = 0;

    ttyFD = mdm_init(&tty);

    char read_buf[2148];
    size_t sz_rd_buf = sizeof(read_buf);
    printf("Please Enter the response type : \n");
    printf("0 - with end line charactes. \n 1 - Without end line characters.\n");
    printf("Enter choice : ");
    uint8_t ch = 0;
    uint8_t rt;
    scanf("%hhd",&ch);
    if( (ch != 1 ) && (ch != 0) )
    {
        printf("Wrong choice.\n");
        printf("Setting default to '0'\n");
        ch = 0;
    }
    
    rt = 1;
    while(1)
    {        
        #ifndef RD_ONLY
        rt = rd_ATcmd(SampleCmd,strlen(SampleCmd));
        if(rt == 2)
        {
            printf("Exiting... .. .\n\r");
            goto END ;
        }
        else if(rt == 0)
        {
            printf("Invalid AT commands. Please try again.\n\r");
        }
        #endif
        if( rt == 1)
        {
            #ifndef RD_ONLY
            printf("\nCMD : %s\n",SampleCmd);
            i_WriteBytes = write(ttyFD, SampleCmd, sizeof(SampleCmd) -1 );
            printf("written bytes = %d\n", i_WriteBytes);
            if (i_WriteBytes < 0)
            {
                printf("failed to write value on port\n");
                return 1;
            }
            #endif
            //sleep(1);
            for(int i=0;i<5;i++)       
            {
                memset(read_buf, '\0', 2048);
                int num_bytes = read(ttyFD, &read_buf, 2048);
                printf("Read count = %d\n\r",num_bytes);
                if (num_bytes < 0) 
                {
                    printf("Error reading: %s", strerror(errno));
                }        
                else if(num_bytes == 0)
                {
                    printf("No response from modem. Waiting for response now.\n\r");
                    sleep(1);
                    // if(i == 4)
                    // {
                    //     i = 6;
                    // }
                }
                else
                {
                    //i = 6;
                    switch(ch)
                    {
                        case 1:
                            print_mdm(read_buf);
                        break;
                        case 0:
                            printf("%s",read_buf);
                        break;

                    }
                    
                }
            }
        }
        
        //printf("Read %i bytes.\n Received message: %s", num_bytes, read_buf);
    }
    
    END:;
    close(ttyFD);
    return 0;
}

