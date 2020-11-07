#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

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

void calculate_meminfo(MEM_INFO_ *memdata)
{
    u_int32_t temp;
    temp = (memdata->f_memtotal - memdata->f_memavailable);
   // printf("temp = %d\n",temp);
    memdata->r_memused = ((float)temp / ((float)memdata->f_memtotal)) * (100.0);
    printf("Memory used = %.4f%%\n",memdata->r_memused);
    memdata->r_cached = ((float)(memdata->f_cached)/(float)(memdata->f_memtotal))*100.0;
    printf("Cache = %.4f%%\n",memdata->r_cached);
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
                // cnt -= 1;
                break;
            }
            tmp = strtok(NULL,"\n");
        }
        else
        {
            break;
        }        
    }
    
    return cnt;
}

/********************************************
 * flt : 0
 *      extract integer only
 * flt : 1 
 *      extract digit alongwith float
 * *****************************************/
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
                }
                else
                {
                    
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
    }
    return c;
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
                ret = 0;
            }
        }
    }
    close(fd);
    return ret;    
}

int main()
{
    MEM_INFO_ memdata;
    read_meminfo(&memdata);
    calculate_meminfo(&memdata);
}