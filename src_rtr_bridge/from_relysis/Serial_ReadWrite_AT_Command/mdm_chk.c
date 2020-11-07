#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h> 
#include <errno.h> 
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include <semaphore.h>
#include <pthread.h>


struct termios tty;

#define CMD_SND "AT+CGSN\r\0"
#define CMD_RSP "AT+CGSN\r\r\n352913090126978\r\n\r\nOK\r\n\0"

typedef struct TH_ARG__
    {
        int fd;
        sem_t *sem;
    }TH_ARG_;

void print_time_only(void)
{
    time_t tm = time(NULL);
    char Tm[50]={0};
    char *Tm_ptr[10];
    strcpy(Tm,ctime(&tm));
    Tm_ptr[0] = strtok(Tm," ");
    int i = 0;
    if (Tm_ptr[0])
    {
        for(i=1;i<10;i++)
        {
            Tm_ptr[i] = strtok(NULL," ");
            if(Tm_ptr[i] == NULL)
            {
                break;
            }
        }
    }
    if(Tm_ptr[3])
    printf("%s",Tm_ptr[3]);
}

int mdm_init(void)
{
	memset (&tty, 0, sizeof tty);
    int ttyFD;
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

void *TH_write(void *Th_arg)
{
    TH_ARG_ *th_arg = (TH_ARG_*)Th_arg;
    int ret = 0;
    while(1)
    {
        
        sem_wait(th_arg->sem);
        ret = write(th_arg->fd,CMD_SND,strlen(CMD_SND));
        sem_post(th_arg->sem);
        printf("\n\rTH1-wr : ");
        print_time_only();
        printf(" : CMD : ");  
        print_mdm(CMD_SND);
        sleep(60);
    }
}

void *TH_read(void *Th_arg)
{
    TH_ARG_ *th_arg = (TH_ARG_*)Th_arg;
    int ret = 0, res=0;
    char rd_buf[255] = {0};
    uint32_t cnt_suc = 0;
    uint32_t cnt_err = 0;
    while(1)
    {
        usleep(1000*90);
        sem_wait(th_arg->sem);
        ret = read(th_arg->fd,rd_buf,255);
        sem_post(th_arg->sem);
        if(ret > 0)
        {
            if( !strcmp(CMD_RSP,rd_buf) )
            {
                res = 1;
                cnt_suc += 1;
            }
            else
            {
                res = 0;
                cnt_err += 1;
            }
        }
        if(ret > 0)
        {
            printf("\n\rTH2-rd : ");
            print_time_only();
            printf(" : RSP : ");
            puts("\n");
            puts("RSP : ");
            print_mdm(rd_buf);
            puts("CMP : ");
            print_mdm(CMD_RSP);
            printf("\n---SCS=%d, ERR=%d---\n\r",cnt_suc,cnt_err);
            memset(rd_buf,0,strlen(rd_buf));
        }      

    }
}

int main()
{
    int fd = 0,ret = 0;
    fd = mdm_init();
    sem_t sem[2] = {0};

    
    TH_ARG_ th_arg[2];

    for(int i=0;i<2;i++)
    {
        ret = sem_init(&sem[i],0,1);
        if(ret < 0)
        {
            printf("ERR : sem_init(%d) failed due to : %s\n",i,\
                        strerror(errno));            
            printf("ERR-SEM-init : Exiting ... ..\n\r");
            return 0;
        }
        printf("DBG-SEM : semaphore-%d created\n",i);
    }

    th_arg[0].fd = th_arg[1].fd = fd;
    th_arg[0].sem = &sem[0];
    th_arg[1].sem = &sem[1];

    int rt_th = 0;
    void *sts;
    pthread_t th_id[2] = {0};
    pthread_attr_t attr;
    pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);


    rt_th = pthread_create(&th_id[0],NULL,&TH_write,(void*)&th_arg[0]);
    if(rt_th < 0)
    {
        printf("ERR-TH1 : pthread_create(1) failed due to : %s\n\r",strerror(errno));
        printf("Exiting... .. .\n\r");
        pthread_exit(NULL);
        exit(EXIT_FAILURE);
    }
    rt_th = pthread_create(&th_id[1],NULL,&TH_read,(void*)&th_arg[1]);
    if(rt_th < 0)
    {
        printf("ERR-TH2 : pthread_create(2) failed due to : %s\n\r",strerror(errno));
        printf("Exiting... .. .\n\r");
        pthread_exit(NULL);
        exit(EXIT_FAILURE);
    }
    pthread_attr_destroy(&attr);
    ret = pthread_join(th_id[0],&sts);
    ret = pthread_join(th_id[1],&sts);
}