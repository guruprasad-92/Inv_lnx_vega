#ifndef FL_CHK__H
#define FL_CHK__H

    #define Nof_PRCS 25
    #define APP_INV_ST  "Inv_"
    #define APP_INV_EXT "\\.bin"
    #define MAX_NUM_BINS 20
    #define MAX_FILE_NAME 256

    int fl_chkX(char *pth);
    long int get_pid_pth(char *pth);
    void chk_pidNkill(char *pcPth);
    // uint8_t Get_Directory_Info (char *dir_path, char cmFls[MAX_NUM_BINS][MAX_FILE_NAME]);
#endif