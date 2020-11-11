
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <basic/basic.h>
#include <sys/time.h>
#include <sys/reboot.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>




/***************************
 * verify wheather the
 *  string is numeric or not
 *  RETURN : 
 *      +(size(str)) : numeric (+ve)
 *      -(size(str)) : numeric (-ve)
 *      0 : Not numeric
 * ************************/
int is_numeric(char *str)
{
    int ret = 1;
    int sz = strlen(str);
    int cnt = 0;
    uint8_t flg_neg = 0;
    if(str == NULL)
    {
        ret = 0;
    }
    else
    {
        for(int i=0;i<sz;i++)
        {
            if( (str[i] >= '0') && (str[i] <= '9') )
            {
                cnt += 1;
            }
            else if( (str[i] == '-') && (i==0) )
            {
                flg_neg = 1;
            }
            else
            {
                ret = 0;
                i = sz+3;
            }            
        }
        if(ret)
        {
            if(cnt && flg_neg)
            {
                ret = 0-cnt;
            }
            else
            {
                ret = cnt;
            }
        }        
    }
    return ret;    
}

/***************************
 * verify wheather the
 *  string is numeric or not
 *  RETURN : 
 *      +(size(str)) : numeric (+ve)
 *      -(size(str)) : numeric (-ve)
 *      0 : Not numeric
 * ************************/
void dbg_print(const char *clr, const char *format, ...)
{
    #ifdef DBG_PRINT
        char buf[2048]={0};
        va_list args;
        va_start(args,format);
        vsprintf(buf,format,args);    
        if(clr != NULL)
            puts(clr);
        puts(buf);
        if(clr != NULL)
            printf(Reset);
        va_end(args);
    #endif
}

void msleep(const unsigned int mili)
{
    for(int i=0;i<mili;i++)
    {
        usleep(1000);
    }
}

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

void get_time(char *str)
{
    time_t tm = time(NULL);
    memset(str,0,strlen(str));
    sprintf(str,"%s",ctime(&tm));
}



void sys_reboot(void)
{
    sync();
    int rt = reboot(RB_AUTOBOOT);
    if(rt < 0)
    {
        perror("ERR-RBT : reboot() failed due to : ");
    }
}

/******************************
 * rmv_nlcr() : remove the 
 *  <NL> and <CR> characters
 *  from the 'str' by 
 *  replacing '\0' with those.
 * ****************************/
void rmv_nlcr(char *str)
{
    uint32_t c = 0;
    char *tmp[10] = {0};
    for(int i=0;i<strlen(str);i++)
    {   
        if( (str[i] == '\n') || (str[i] == '\r') )
        {
            tmp[c++] = &str[i];
        }
    }
    for(int i=0;i<c;i++)
    {
        tmp[i][0] = '\0';
    }
}

int spilt_string(char *str, char *dst[], const char *dlm)
{

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
    for(int i=0;i<cnt;i++)
    {
        cln_NL_CR(dst[i]);
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
    int brk=0;
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
        dbg_print(Bold_Red,\
            "popen() failed due to : %s\n",strerror(errno));
        ret = -1;
    }
    else
    {
        memset(cmd,0,strlen(cmd));
        printf("cmd : %s\n",cmd);
        ret = fread((void*)cmd,1,70,fs_rsp);
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
            printf("open(fd[%d]) failed due to : %s\n",i,strerror(errno));
            printf("Returning... .. .\n");
            for(int i=0;i<3;i++)
            {
                close(fd[i]);
            }
            return -1;
        }
    }

    for(int i=0;i<3;i++)
    {
        ret = read(fd[i],(void*)str[i],100);
        if(ret <= 0)
        {
            printf("read(fd[%d]) failed due to : %s\n",i,strerror(errno));
            printf("Returning... .. .\n");
            for(int i=0;i<3;i++)
            {
                close(fd[i]);
            }
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
    for(int i=0;i<3;i++)
    {
        close(fd[i]);
    }
    fd[0] = open(f_eth[0],O_RDONLY);
    if(fd[0] < 0)
    {
        printf("open(%s) failed due to : %s\n",f_eth[0],strerror(errno));
        printf("Exiting ... .. .\n");
        close(fd[0]);
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
    close(fd[0]);
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
            // if(ret <= 0)
            // {
            //     printf("Could not get digits from the cmd (uptime -p).\n");
            //     printf("exiting... .. .\n");
            //     fclose(fd_pop2);
            //     fclose(fd_pop);
            //     return -1;
            // }
            switch(ret)
            {
                case 0:
                {
                    UPTM->day = 0;
                    sprintf(UPTM->tm_dif,"00:00");
                    UPTM->idiff_mn = 0;
                }break;
                case 1:
                {
                    UPTM->day = 0;
                    sprintf(UPTM->tm_dif,"00:%s",num_str[0]);
                    UPTM->idiff_mn = atoi(num_str[0]);
                }
                break;
                case 2:
                {
                    UPTM->day = 0;
                    sprintf(UPTM->tm_dif,"%s:%s",num_str[0],num_str[1]);
                    UPTM->idiff_hr = atoi(num_str[0]);
                    UPTM->idiff_mn = atoi(num_str[1]);
                }
                break;
                case 3:
                {
                    UPTM->day = atoi(num_str[0]);
                    sprintf(UPTM->tm_dif,"%s:%s",num_str[1],num_str[2]);
                    UPTM->idiff_hr = atoi(num_str[1]);
                    UPTM->idiff_mn = atoi(num_str[2]);
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
    fclose(fd_pop);
    fclose(fd_pop2);
    return ret;
}

void get_formated_time(char *str, uint8_t frmt)
{
    time_t rw_tm;
    struct tm tm_info;
    time(&rw_tm);
    localtime_r(&rw_tm, &tm_info);
    tm_info.tm_year += 1900;
    tm_info.tm_mon += 1;
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

/***********************************************
 * Check ping stsus 2 times from the ip 8.8.8.8
 * Return :
 *  Success = 1
 *  Error = (0,-1)
 *      0 : ping failed due to network failure
 *     -1 : Other system related error.
 * ********************************************/
int get_ping_sts(void)
{
    FILE *fp_snd;
    int ret = 0, ping_no=2;
    char buf[512];
    char cmd[25]={0};
    sprintf(cmd,"ping -w %d 8.8.8.8",ping_no);
    // printf("cmd : %s\n",cmd);

    fp_snd = popen(cmd,"r");
    if(fp_snd == NULL)
    {
        dbg_print(Bold_Red,"popen() failed due to : %s\n",strerror(errno));
        fclose(fp_snd);
        return -1;
    }
    else
    {
        memset(buf,0,strlen(buf));
        ret = fread((void*)buf,1,330,fp_snd);
        // printf("fread() = %d\n",ret);
        if(ret > 0)
        {
            char *lns[10];
            ret = split_line(buf,lns, 10);
            if(ret > 0)
            {
                for(int i=0;i<ret;i++)
                {
                    if(strstr(lns[i],"packet loss"))
                    {
                        char *numstr[3];
                        ret = str2numstr(lns[i],numstr,2,0);
                        if(ret >= 2)
                        {
                            if((atoi(numstr[0]) == ping_no) && \
                                (atoi(numstr[1]) == ping_no) )
                            {
                                ret = 1;
                                break;
                            }
                            else
                            {
                                ret = 0;
                            }                            
                        }
                        else
                        {
                            ret = 0;
                        }
                    }
                }
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
    fclose(fp_snd);
    return ret;
}


void wait_upto_uptm(uint32_t tm_max)
{
    UPTM_INFO_ sUPTM = {0};
    uint32_t cnt = 0;
    uint32_t tm_max_sec = tm_max*60;
    do
    {
        get_sys_uptm(&sUPTM);
        if( (sUPTM.idiff_mn >= tm_max ) || (sUPTM.idiff_hr > 0 ) )
        {
            break;
        }
        cnt += 5;
        dbg_print(NULL,"Waiting for system uptime : %d Min\n",tm_max);
        dbg_print(NULL,"Current uptime = %02d:%02d\n",sUPTM.idiff_hr, sUPTM.idiff_mn);
        sleep(5);
    }while (cnt <= tm_max_sec);
    dbg_print(NULL,"System uptime = %02d:%02d \n",sUPTM.idiff_hr,sUPTM.idiff_mn);
}


