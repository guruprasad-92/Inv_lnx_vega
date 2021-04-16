#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "basic/color.h"
#include "basic/basic.h"
#include "fl_chk.h"

#define LG_DIR "/home/root/logs/"
#define LG_PTH LG_DIR"rbt.log"

int main(void)
{
    printf(LG_PTH);
}