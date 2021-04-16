/**********************************
 * 
 *  Author  : Guruprasad Prusty
 *  Date    : 11/nov/2020
 *  Time    : 11:57:00
 * 
 * ******************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

#include "basic.h"
#include "fl_chk.h"

/****************************************
 * Check wheather the owner has
 * execute permision of the *pth or not
 *  
 *  Success : 0
 *  Error   : -1
****************************************/
int fl_chkX(char *pth)
{
    struct stat fl_stat = {0};
    int ret = 0;
    if(pth != NULL)
    {
        ret = stat(pth,&fl_stat);
        // printf("st_mode = 0x%x\n",fl_stat.st_mode);
        // printf("S_IRWXU = 0x%x\n",S_IRWXU);
        // printf("st_mode& ~S_IFREG = 0x%x\n",(fl_stat.st_mode & ~S_IFREG));
    }
    if(ret < 0)
    {
        dbg_print(Bold_Red,\
            "ERR : chk_fl_mode() failed due to : %s\n",strerror(errno));
        ret = -1;
    }
    else if( (fl_stat.st_mode & ~S_IFREG) & (S_IXOTH | S_IXGRP | S_IXUSR ))
    {
        ret = 0;
    }

    else
    {
        ret = -1;
    }
    return ret;    
}

int get_pid_inv(char *pth[], long int *pid[])
{
    FILE *fs;
}

long int get_pid_pth(char *pth)
{
    FILE *fs;
    long int ret = 0;
    size_t sz = 8;
    char *numstr[5] = {0};
    // char *str = NULL;
    sz += strlen(pth);
    // str = (char*)calloc(sz,sizeof(char));
    char str[300] = {0};
    
    if(str)
    {
        memset(str,0,300);
        sprintf(str,"pidof %s",pth);
        fs = popen(str,"r");
        ret = fread(str,1,250,fs);
        if(ret > 0)
        {
            ret = str2numstr(str,numstr,1,0);
            if(ret == 1)
            {
                ret = atoi(numstr[0]);
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
    else
    {
        ret = -1;
    }
    pclose(fs);
    return ret;
}


int8_t Get_Directory_Info (char *dir_path, \
        char cmFls[MAX_NUM_BINS][MAX_FILE_NAME])
{
	char path [1024]; 
	struct dirent *directory_entry; 							/* Pointer for directory entry */ 
	struct stat *file_stat;
	int16_t n16PID = -1;
	int n64PID_Num = -1;
    uint32_t fl_cnt = 0;

	DIR *dir = opendir(dir_path); 								// opendir() returns a pointer of DIR type. 
	memcpy (path, dir_path, sizeof (path));

	if (dir == NULL) 											// opendir returns NULL if couldn't open directory 
	{ 
		printf("\n****\tCould not open the directory \"%s\"\t****\n", dir_path);
		return -1;
	} 

	while (((directory_entry = readdir(dir)) != NULL) && (fl_cnt < MAX_NUM_BINS))			// for readdir() 
	{	
		if (directory_entry->d_type == DT_DIR) 
        {
			if (strcmp(directory_entry->d_name, ".") == 0 || strcmp(directory_entry->d_name, "..") == 0)
					continue;
		}
		else
		{
			/** Copying the Process Name to the Array */
			strncpy (cmFls[fl_cnt], directory_entry->d_name, strlen(directory_entry->d_name));

			// printf ("PID of %s = %d\n",directory_entry->d_name, g_n32PID_arr [g_Num_of_File]);
			
			fl_cnt += 1;
		}
	}		

	closedir(dir);	 
	return fl_cnt;
}


void chk_pidNkill(char *pcPth)
{
    long int pid;
    int cnt;
    char str[105] = {0};
    int fd_kmsg = open("/dev/kmsg",O_WRONLY | O_APPEND);

    do
    {
        pid = get_pid_pth(pcPth);
        if(pid > 0)
        {
            memset(str,0,100);
            sprintf(str,"RTR-BRZ : killing the app '%s', PID : %ld\n",pcPth,pid);
            dbg_print(Bold_Green,str);
            if(fd_kmsg > 0)
                write(fd_kmsg,str,strlen(str));
            
            memset(str,0,100);
            sprintf(str,"kill -9 %ld",pid);
            system(str);            
        }
        else
        {
            sprintf(str,"RTR-BRZ : App '%s' is already dead.\n",pcPth);
            dbg_print(Bold_Green,str);
            if(fd_kmsg > 0)
                write(fd_kmsg,str,strlen(str));
            memset(str,0,100);
        }        
        pid = get_pid_pth(pcPth);
    } while( (pid > 0) && (cnt++ <= Nof_PRCS) );
    close(fd_kmsg);
}