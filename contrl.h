#ifndef __CONTRL_H
#define __CONTRL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>

int init_gpio(void);
void hc595(unsigned char dat);
void hc595out();
void cen_on(u_int8_t y);
int cube_close(void);

#endif
