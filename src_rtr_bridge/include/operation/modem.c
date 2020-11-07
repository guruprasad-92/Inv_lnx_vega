#include "operation/modem.h"
#include "basic/color.h"
#include "basic/basic.h"

#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

/***************************************
 * Open the port mentioned in "dev_mdm"
 * and return the file descriptor.
 * ***********************************/
int modem_init(char *dev_mdm)
{
    int fd = -1;
    struct termios tty;
}