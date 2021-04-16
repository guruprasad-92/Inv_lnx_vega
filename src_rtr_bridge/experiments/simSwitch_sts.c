#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/time.h>
#include <time.h>

#include <linux/rtc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>

#define ACTIVE_SLOT "/reap/etc/config/ActiveSIMStatus"

typedef struct MEM_INFO__ 

{
    u_int32_t f_memtotal;
    u_int32_t f_memfree;
    u_int32_t f_memavailable;
    u_int32_t f_buffers;
    u_int32_t f_cached;

    float r_memused;
    float r_cached;
}MEM_INFO_;


typedef struct UPTM_INFO__
{
    char tm_up[25];
    char tm_dif[20];    
    uint32_t day;
}UPTM_INFO_;


typedef struct ETH_INFO__
{
    char IP[25];
    char NM[25];
    char ecm[25];
}ETH_INFO_;


typedef struct TM_ZONE_ 
{
    char zn; // '+' or '-'
    int hr;
    int mn;
    uint8_t utc;
}tm_zone_;
/*
int get_time__rtc(char *str, char *dev, const tm_zone_ *tm_zone)
{
    int fd_rtc, ret;
    ret = 0;
    struct rtc_time rtc_tm;
    int hr=0;
    fd_rtc = open(dev,O_RDONLY);
    if(fd_rtc < 0)
    {
        printf("open(%s) failed due to : %s",dev,strerror(errno));
        ret = -1;
    }
    else
    {
        ret = ioctl(fd_rtc,RTC_RD_TIME,&rtc_tm);
        if(ret < 0)
        {
            printf("ioctl(%s) failed due to : %s",dev,strerror(errno));
            ret = -1;
        }
        else
        {
            
            printf("str : %s\n",str);

            if(tm_zone->utc == 0)
            {
                rtc_tm.tm_min += tm_zone->mn;
                if(rtc_tm.tm_min > 60)
                {
                    rtc_tm.tm_min -= 60;
                    hr = 1;
                }
                else if(rtc_tm.tm_min < 0)
                {
                    rtc_tm.tm_min += 60;
                    hr = -1;
                }
                
                rtc_tm.tm_hour += tm_zone->hr + hr; // Indian time Zone
                if(rtc_tm.tm_hour > 24 )
                {
                    rtc_tm.tm_hour -= 24;
                    rtc_tm.tm_mday += 1;
                }
                else if(rtc_tm.tm_hour < 1)
                {
                    rtc_tm.tm_hour += 24;
                    rtc_tm.tm_mday -= 1;
                }

                if(rtc_tm.tm_mday < 30 )
                {
                    
                }
            }           
            
            rtc_tm.tm_year += 1900;

            memset(str,0,strlen(str));
            sprintf(str,"date - %02d/%02d/%04d; time - %d:%d:%d",\
                rtc_tm.tm_mday, rtc_tm.tm_mon, rtc_tm.tm_year, \
                rtc_tm.tm_hour,rtc_tm.tm_min , rtc_tm.tm_sec);
        }
    }
    return ret;
}
*/

void cln_NL_CR(char *str)
{
    char *p1,*p2;
    p1 = strchr(str,'\n');
    p2 = strchr(str,'\r');
    int sz = strlen(str);
    for(int i=0;i<sz;i++)
    {
        if( (str[i] == '\n') || (str[i]=='\r') )
        {
            str[i] = 0;
        }
    }

    // if(p1 )
    // {
    //     *p1 = 0;
    // }
    // if(p2)
    // {
    //     *p2 = 0;
    // }
}

void get_time_rtc(char *str)
{
    time_t raw_tm;
    struct tm time_info;
    time(&raw_tm);
    localtime_r(&raw_tm,&time_info);
    sprintf(str,"D-%02d/%02d/%04d T-%02d:%02d:%02d",\
            time_info.tm_mday, time_info.tm_mon, time_info.tm_year + 1900, \
            time_info.tm_hour, time_info.tm_min, time_info.tm_sec);
}

/***************************************************
 * For flt : 0
 *      get the integers only
 * For flt : 1
 *      get the decimal ind digits as well.
 *       
 * ************************************************/
int str2numstr(char *str, char *numstr[], int len, u_int32_t flt)
{
    int sz = strlen(str);
    int c = 0, j =0;
    char *st, *end;
    int brk=0;
    int dot = 0;
    int no_dot=0;
    if(flt==0)
    {
      dot=3;
    }
    else if(flt != 1)
    {
      return 0;
    }
    for(int i=0;i<sz;i++)
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

/**********************************************
 *   ------ get the system uptime info ------
 * *******************************************/
int get_sys_uptm(UPTM_INFO_ *UPTM)
{
    FILE *fd_pop = NULL;
    FILE *fd_pop2 = NULL;
    char str[70] = {0};
    char *ptr1=NULL;
    int ret,ret2;

    fd_pop = popen("uptime -p", "r");
    fd_pop2 = popen("uptime -s", "r");
    if(fd_pop2 == NULL)
    {
        printf("popen(fd_pop2) failed due to : %s\n",strerror(errno));
        printf("exiting....\n");
        fclose(fd_pop2);
        fclose(fd_pop);
        return -1;
    }

    if(fd_pop == NULL )
    {
        printf("popen(fd_pop) failed due to : %s\n",strerror(errno));
        printf("exiting....\n");
        ret = -1;
    }
    else
    {
        ret = fread((void*)str,1,70,fd_pop);
        if(ret > 0)
        {
            // printf("uptime =%s\n",str);
            char *num_str[3] = {0};
            ret = str2numstr(str,num_str,3,0);
            if(ret <= 0)
            {
                printf("Could not get digits from the cmd (uptime -p).\n");
                printf("exiting... .. .\n");
                fclose(fd_pop2);
                fclose(fd_pop);
                return -1;
            }
            switch(ret)
            {
                case 1:
                {
                    UPTM->day = 0;
                    sprintf(UPTM->tm_dif,"00:%s",num_str[0]);
                }
                break;
                case 2:
                {
                    UPTM->day = 0;
                    sprintf(UPTM->tm_dif,"%s:%s",num_str[0],num_str[1]);
                }
                break;
                case 3:
                {
                    UPTM->day = atoi(num_str[0]);
                    sprintf(UPTM->tm_dif,"%s:%s",num_str[1],num_str[2]);
                }
                break;
            }

            memset(str,0,sizeof(str));
            ret = fread(str,1,70,fd_pop2);
            if(ret > 0)
            {
                cln_NL_CR(str);
                strncpy(UPTM->tm_up,str,sizeof(UPTM->tm_up));
                ret = 0;
            }
        }
    }
    pclose(fd_pop);
    pclose(fd_pop2);
    return ret;
}


/************************************
 *  --- Read the memory Usage ---
 *      Success : 0
 *      Error   : -1
 * *********************************/
int read_meminfo(MEM_INFO_ *memdata)
{
    int fd,ret;
    char str[150]={0};
    char *numstr[10]={0};
    fd = open("/proc/meminfo", O_RDONLY);
    if (fd < 0) 
    { 
        printf("open(/proc/meminfo) failed due to : %s\n",strerror(errno));
        ret = -1;
    } 
    else
    {
        ret = read(fd,(void *)str, 150);
        if (ret < 0) 
        { 
            printf("read(fd_meminfo) failed due to : %s\n",strerror(errno));
            ret = -1;
        } 
        else
        {
            ret = str2numstr(str,numstr,5,1);
            if(ret == 5)
            {
                memdata->f_memtotal = atoi(numstr[0]);
                // printf("MEMINFO.MemTotal: %d\n",memdata->f_memtotal);
                memdata->f_memfree = atoi(numstr[1]);
                // printf("MEMINFO.MemFree: %d\n",memdata->f_memfree);
                memdata->f_memavailable = atoi(numstr[2]);
                //  printf("MEMINFO.MemAvailable: %d\n",memdata->f_memavailable);
                memdata->f_buffers = atoi(numstr[3]);
                // printf("MEMINFO.Buffers: %d\n",memdata->f_buffers);
                memdata->f_cached = atoi(numstr[4]);
                // printf("MEMINFO.Cached: %d\n",memdata->f_cached);

                u_int32_t temp;
                temp = (memdata->f_memtotal - memdata->f_memavailable);
                memdata->r_memused = ((float)temp / ((float)memdata->f_memtotal)) * 100.0F;
                // printf("Memory used = %.4f\n",memdata->r_memused);
                memdata->r_cached = ((float)(memdata->f_cached)/(float)(memdata->f_memtotal))*100.0;
                // printf("Cache = %.4f\n",memdata->r_cached);
                ret = 0;
            }
        }
    }
    close(fd);
    return ret;    
}


/**************************************
 * Arguments : 
 *  size(str) = 60
 *  fd : Not required.
 * ***********************************/
int pack_meminfo(int fd, char *str, uint32_t sz)
{
    int ret = 0;
    MEM_INFO_ MEM_DT;

    if(sz < 60)
    {
        return -1;
    }
    ret = read_meminfo(&MEM_DT);
    if(ret == 0)
    {
        sprintf(str,"MEM_INFO : Total=%d; used=%f; cached=%f\n",\
                    MEM_DT.f_memtotal, MEM_DT.r_memused, MEM_DT.r_cached);
        ret = strlen(str);
    }
    return ret;
}

/*****************************
 * Minimum size of the 
 *  str = 75
 * **************************/
int pack_sys_uptime(char *str, uint32_t sz)
{
    UPTM_INFO_ UP_tm = {0};
    int ret;
    if(sz < 75)
    {
        return -1;
    }
    ret = get_sys_uptm(&UP_tm);
    if(ret == 0)
    {
        memset(str,0,sz);//19+5+2
        sprintf(str,"SYS_UPTIME : uptime=%s;diff_time=%s;diff_day=%d;\n",\
                UP_tm.tm_up, UP_tm.tm_dif, UP_tm.day);
    }
    return ret;
}


/**********************************************
 *    get the system time in format according 
 *            to the variable frmt
 * 
 * for frmt : 1
 *      time format = "D-DD/MM/YYYY T-hh:mm:ss"
 *     frmt : 2
 *      time format = "DDMMYYYYhhmmss"
 *     frmt : 3
 *      time format = "DDMMYYhhmmss"
 * *******************************************/
void get_formated_time(char *str, uint8_t frmt)
{
    time_t rw_tm;
    struct tm tm_info;
    time(&rw_tm);
    localtime_r(&rw_tm, &tm_info);
    tm_info.tm_year += 1900;
    if(frmt == 1)
    {
        sprintf(str,"D-%02d/%02d/%04d T-%02d:%02d:%02d", \
                tm_info.tm_mday, tm_info.tm_mon, tm_info.tm_year, \
                tm_info.tm_hour, tm_info.tm_min, tm_info.tm_sec);
    }
    else if(frmt == 2)
    {
        sprintf(str,"%02d%02d%04d%02d%02d%02d", \
                tm_info.tm_mday, tm_info.tm_mon, tm_info.tm_year, \
                tm_info.tm_hour, tm_info.tm_min, tm_info.tm_sec);
    }
    else if(frmt == 3)
    {
        sprintf(str,"%02d%02d%02d%02d%02d%02d", \
                tm_info.tm_mday, tm_info.tm_mon, tm_info.tm_year - 2000, \
                tm_info.tm_hour, tm_info.tm_min, tm_info.tm_sec);
    }
}


int read_active_sim(void)
{
    char sim_pth[35] = {0};
    char str[2]= {0};
    int ret;
    int fd = -1;
    sprintf(sim_pth,"/reap/etc/config/ActiveSIMStatus");
    fd = open(sim_pth,O_RDONLY);
    
    ret = read(fd,str,1);
    if(ret <= 0)
    {
        close(fd);
        return -1;
    }
    ret = atoi(str);
    close(fd);
    return ret;
}



int mdm_selSim(uint32_t sim)
{
    FILE *fp_rsp = NULL;
    FILE *fp_snd = NULL;
    int ret = 0;
    char mos_cmd[120]={0};
    // char buf[1024] = {0};
    int Dtm=140; //Maximum delay time 
    int Asim = read_active_sim();
    int sts = 0;
    if(Asim < 0)
    {
        return Asim;
    }
    else if(Asim == sim)
    {
        printf("sim=%d is already active.\n",sim);
        return Asim;
    }
    if( (sim < 1) && (sim > 2) )
    {
        printf("\nInvalid sim-%d\n",sim);
        ret = -1;
    }
    else
    {
        sprintf(mos_cmd,"mosquitto_pub -d -t \"Isensev2/SimSwitch/CommandRequest\" -m \"<,1,SELSIM%d,\\\"2020-09-21 11:00:00\\\",>\"",sim);
        fp_snd = popen(mos_cmd,"r");
        if(fp_snd == NULL)
        {
            perror("\nmosquitto_pub() failed due to : ");
            ret = -1;
        }
        else
        {
            printf("\nSelecting SIM%d. Mnimum processing time : %dSec.\n",sim,Dtm);
            do
            {
                Asim = read_active_sim();
                sleep(1);
            } while ( (Asim!=sim) && (sts++ <= Dtm) );
            printf("Time taken to switch = %d\n",sts);
            pclose(fp_snd);
            ret = Asim;
        }
    }
    return ret;
}


int split_line(char *src, char *dst[], int ln_max)
{
    int sz = strlen(src);
    char *tmp=NULL;
    int cnt = 0;
    tmp = strtok(src,"\n");
    for(int i=0;i<sz;i++)
    {        
        if(tmp != NULL)
        {
            if(cnt < ln_max)
            {
                dst[cnt++] = tmp;
            }
            else
            {
                break;
            }
            tmp = strtok(NULL,"\n");
        }
        else
        {
            //printf("\n\rNo more <NL>\n\r");
            break;
        }        
    }
    return cnt;
}

char* pop_ping(uint32_t cnt)
{
    FILE *fp_snd;
    int ret = 0;
    static char buf[512];
    char cmd[25]={0};
    char *rt = (void*)NULL;
    sprintf(cmd,"ping -w %d 8.8.8.8",cnt);
    printf("cmd : %s\n",cmd);

    fp_snd = popen(cmd,"r");
    if(fp_snd == NULL)
    {
        perror("popen() failed due to :");
        return (char*)NULL;
    }
    else
    {
        memset(buf,0,strlen(buf));
        ret = fread((void*)buf,300,300,fp_snd);
        printf("fread() = %d\n",ret);
        if(ret > 0)
        {
            rt = buf;
        }
        else
        {
            rt = (char*)NULL;
        }
    }
    pclose(fp_snd);
    return rt;
}

/*
int str2numstr(char *str, char *numstr[], int len)
{
    int sz = strlen(str);
    int c = 0, j =0;
    char *st, *end;
    int brk=0;
    int dot = 0;
    for(int i=0;i<sz;i++)
    {
        if( ( (str[i] >= '0') && (str[i] <= '9') ) )
        {

            if( j==0 )
            {
                if( c < len )
                {
                    numstr[c++] = &str[i];
                    // printf("start num = %c idx = %d\n",str[i],i);
                }
                else
                {
                    // brk = 1;
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
        else if( ((i-j) == 1)  || (j==-1) )
        {
            j = 0;
            str[i] = 0;
        }
        

        // if(brk)
        // {
        //     break;
        // }
    }
    return c;
}
*/

/*************************
 * Return : 
 *      Success = 0
 *      Error   = -1
 * **********************/
int get_sys_uptime(char *up_tm)
{
    
    int ret = -1;
    char cmd[70] = {0};
    sprintf(cmd,"uptime");
    FILE *fs_rsp;

    
    fs_rsp = popen(cmd,"r");
    if(fs_rsp == NULL)
    {
        perror("popen() failed due to :");
        ret = -1;
    }
    else
    {
        memset(cmd,0,strlen(cmd));
        printf("cmd : %s\n",cmd);
        ret = fread((void*)cmd,70,70,fs_rsp);
        printf("cmd : %s\n",cmd);
        printf("fread() = %d\n",ret);
        if(ret > 0)
        {
            
            char *num[6] = {0};
            ret = str2numstr(cmd,num,6,0);
            printf("ret = %d\n",ret);
            if(ret == 2)
            {
                strcat(up_tm,num[5]);
                strcat(up_tm,":");
                strcat(up_tm,num[6]);
                ret = 0;
            }
        }
    }

    pclose(fs_rsp);
    return ret;
}

int get_eth_info(ETH_INFO_ *ETH_INFO)
{
    char f_eth[3][21] = {0}, *ptr=NULL;
    int ret, fd[3];
    char str[3][100] = {0};
    char *lns[3][4] = {0};
    char *lns2[6]={0};
    char str_ecm[230]={0};


    for(int i=0;i<3;i++)
    {
        sprintf(f_eth[i],"/etc/network/eth%d.cfg",i);
        fd[i] = open(f_eth[i],O_RDONLY);
        if(fd[i] < 0)
        {
            printf("fd[%d] failed due to : %s\n",i,strerror(errno));
            return -1;
        }
    }

    for(int i=0;i<3;i++)
    {
        ret = read(fd[i],(void*)str[i],100);
        if(ret <= 0)
        {
            printf("read(fd[%d]) failed due to : %s\n",i,strerror(errno));
            return -1;
        }
        // printf("str[%d] = %s\n\n",i,str[i]);
        ret = split_line(str[i],lns[i],4);
        // printf("ret = %d\n",ret);
        ptr = strchr(lns[i][2],' ');
        ptr += 2;
        // printf("ETH-%d IP :%s\n",i,ptr);
        sprintf(ETH_INFO[i].IP,"%s",ptr);
        // printf("ETH_INFO[%d].IP :%s\n",i,ETH_INFO[i].IP);
        
        ptr = strchr(lns[i][3],' ');
        ptr += 2;
        
        sprintf(ETH_INFO[i].NM,"%s",ptr);
        // printf("ETH_INFO[%d].NM :%s\n",i,ETH_INFO[i].NM);

    }

    memset(f_eth[0],0,strlen(f_eth[0]));
    sprintf(f_eth[0],"/reap/etc/config/Mode"); // To read the IP for ecm0
    fd[0] = open(f_eth[0],O_RDONLY);
    if(fd[0] < 0)
    {
        printf("open(%s) failed due to : %s\n",f_eth[0],strerror(errno));
        printf("Exiting ... .. .\n");
        return -1;
    }
    else
    {
        ret = read(fd[0],(void*)str_ecm,220);
        if(ret > 0)
        {            
            ret = split_line(str_ecm,lns2,6);
            // printf("lines = %d\n",ret);
            for(int i=0;i<ret;i++)
            {
                ptr = strstr(lns2[i],"USBStaticIP '");
                if(ptr)
                {
                    ptr += strlen("USBStaticIP '");
                    strcpy(ETH_INFO[0].ecm,ptr);
                    ptr = strchr(ETH_INFO[0].ecm,'\'');
                    *ptr = (char)0;
                    ret = 0;
                }
            }
        }
    }
    // printf("ECM0 = %s\n",ETH_INFO[0].ecm);
    return ret;
}

/*********************************************
 *  Minimum size of the str = 
 *      (25*4) + (5*4) = 120
 * ******************************************/
int pack_eth_info(char *str, uint32_t sz)
{
    ETH_INFO_ eth_info[3] = {0};
    char tmp[70] = {0};
    int ret;

    if(sz < 120)
    {
        return -1;
    }
    memset(str,0,sz);
    ret = get_eth_info(eth_info);
    if(ret != 0)
    {
        return -1;
    }
    for(int i=0;i<3;i++)
    {
        if(eth_info[i].IP == NULL)
        {
            return -1;
        }
        if(i==0)
        {
            if(eth_info[i].ecm == NULL)
            {
                return -1;
            }
            sprintf(str,"IP_ecm%d : %s\n",i,eth_info[i].ecm);
        }
        sprintf(tmp,"IP_eth%d : %s\n",i,eth_info[i].IP);
        strcat(str,tmp);
        memset(tmp,0,strlen(tmp));
    }
    return 0;
}

/*********************************************
 *  Minimum size of the str = 25
 * ******************************************/
int pack_sys_time(char *str, uint32_t sz)
{
    char tmp[25]={0};
    if(sz < 25)
    {
        return -1;
    }
    memset(str,0,sz);
    get_formated_time(tmp,2);
    sprintf(str,"RTC_TM : %s\n",tmp);
    return 0;
}


int main(int c, char *arg[] )
{
    char str[500]={0};
    char *str_ln[15] = {0};
    char *rt_str=NULL;
    /*
    // rt_str = pop_ping(2);
    // if(rt_str == NULL)
    // {
    //     perror("pop_ping() failed due to :");
    // }
    // strcpy(str,rt_str);
    // int ln = split_line(str,str_ln,12);
    
    // printf("ln[%d] : %s\n",ln-3,str_ln[ln-3]);
    // printf("ln[%d] : %s\n",ln-2,str_ln[ln-2]);

    // if(strstr(str_ln[ln-2],"0% packet loss") )
    // {
    //     printf("Device is ready\n");
    // }
    // else
    // {
    //     printf("Device is not ready\n");
    // }

    // memset(str,0,500);
    // printf("size(str) = %ld\n",sizeof(str));
    // int a = get_sys_uptime(str);
    // printf("a = %d\n",a);
    // printf(" system uptime = %s\n",str);
    // char str2[90] = {"8:08:18 up 1 day, 20:25,  1 user,  load average: 0.39, 0.29, 0.26"};
    // char *ptr_num[20] = {0};
    // int ret = str2numstr(str2,ptr_num,20);
    // printf("str2numstr() = %d\n",ret);
    // for(int i=0;i<ret;i++)
    // {
    //     printf("num[%d] = %s\n",i,ptr_num[i]);
    // }

    //if()
*/
    for(int i=0;i<c;i++)
    {
        printf("arg[%d] = %s\n",i,arg[i]);
    }

/*    
    get_formated_time(str,1);
    printf("str : %s\n",str);
    memset(str,0,strlen(str));
    get_formated_time(str,2);
    printf("str : %s\n",str);

    memset(str,0,strlen(str));
    get_formated_time(str,3);
    printf("str : %s\n",str);
*/
    int ret = pack_sys_uptime(str,75);
    if(ret == 0)
    printf("str(1) = {%s}\n",str);
    ret = pack_eth_info(str,120);
    if(ret == 0)
    printf("str(2) = {%s}\n",str);
    ret = pack_sys_time(str,25);
    printf("str(3) = {%s}\n",str);
    ret = pack_meminfo(0,str,60);
    printf("ret = %d\n",ret);
    
    printf("Active sim = %d\n",read_active_sim());

    if( (arg[1][0] == '1') || (arg[1][0] == '2') )
    {
        c = atoi(arg[1]);
        ret = mdm_selSim(c);
        printf("ret = %d\n",ret);
    }

    
}

