#ifndef MODEM__H
#define MODEM__H

#define DEV_MDM "/dev/ttyAT"

/***************************************
 * Open the port mentioned in "dev_mdm"
 * and return the file descriptor.
 * ***********************************/
int modem_init(char *dev_mdm);


#endif