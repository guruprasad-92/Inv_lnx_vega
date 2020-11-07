#ifndef VEGA_THRDS__H
#define VEGA_THRDS__H

void* th_rtr_prd(void* th_arg);
void *thd_rtr_cmd(void *arg);

/*********************************
 * 
 *  Not required for V2 
 *  Required for testing
 * 
 * ******************************/
void *thd_rtr_cmd_test(void *arg);

#endif