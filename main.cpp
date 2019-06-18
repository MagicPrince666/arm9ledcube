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
#include <iostream>

#include "move.h"

using namespace std;

int hc595_dat = -1;
int hc595_st = -1;
int hc595_sh = -1;
int gpio_layer[8] = {-1};

int setup_gpio(int pin){
    FILE* set_export = NULL;
    char setpin[64] = {0};
    sprintf(setpin,"%d",pin);
    set_export = fopen ("/sys/class/gpio/export", "w");
    if(set_export == NULL)printf ("Can't open /sys/class/gpio/export!\n");
    else {
        fprintf(set_export,setpin);//gpioE_4 4*32+4 hc595_ht
    }
    fclose(set_export);
    return 0;
}
int open_gpio(int *fd, int pin){
    char setpin[64] = {0};
    sprintf(setpin,"/sys/class/gpio/gpio%d/direction",pin);
    *fd = open (setpin, O_RDWR);
    if(*fd <= 0)printf ("open %s error\n",setpin);
    else write(*fd,"out",3);
    close(*fd);
    sprintf(setpin,"/sys/class/gpio/gpio%d/value",pin);
    *fd = open (setpin, O_RDWR);
    if(*fd <= 0)printf ("can not open %s\n",setpin);
    return 0;
}

void hc595(unsigned char dat)
{
  unsigned char i;
  for(i = 0; i < 8; i++)
  {
     if((dat<<i)&0x80)
        write(hc595_dat,"1",1);
	 else
	    write(hc595_dat,"0",1);

     write(hc595_sh,"0",1);
     write(hc595_sh,"1",1);
  }  
}

void hc595out()
{
    write(hc595_st,"0",1);
    write(hc595_st,"1",1);
}

void cen_on(u_int8_t y){
    for(int i = 0 ; i < 8; i++)
        write(gpio_layer[i],"0",1);
    switch(y){
      case 0x00:
            write(gpio_layer[0],"1",1);break;
      case 0x01:
            write(gpio_layer[1],"1",1);break;
      case 0x02:
            write(gpio_layer[2],"1",1);break;
      case 0x03:
            write(gpio_layer[3],"1",1);break;
      case 0x04:
            write(gpio_layer[4],"1",1);break;
      case 0x05:
            write(gpio_layer[5],"1",1);break;
      case 0x06:
            write(gpio_layer[6],"1",1);break;
      case 0x07:
            write(gpio_layer[7],"1",1);break;
      default:break;
  }
}

void display(u_int16_t time,u_int8_t dat)
{
  unsigned char x,y,z;
  int times = 0;
  for(z = 0; z < 1; z++)
  {
    while(times < time)
	{
	  	for(y = 0;y < 8; y++)
		{
		    for(x = 0;x < 8; x++){
		        hc595(dat);
		    }
		    hc595out();
        cen_on(y);
        usleep(1);
        cen_on(8);
		}
        times++;
	}
  }
}

const unsigned char test[][8][8]= {
 {
 {0xFF,0x81,0x81,0x81,0x81,0x81,0x81,0xFF},		//8
 {0x00,0x7E,0x42,0x42,0x42,0x42,0x7E,0x00},
 {0x00,0x00,0x3C,0x24,0x24,0x3C,0x00,0x00},
 {0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00},
 {0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00},
 {0x00,0x00,0x3C,0x24,0x24,0x3C,0x00,0x00},
 {0x00,0x7E,0x42,0x42,0x42,0x42,0x7E,0x00},
 {0xFF,0x81,0x81,0x81,0x81,0x81,0x81,0xFF}
 }
};

void general(const unsigned char po[][8][8], unsigned char cnt, int tv) {
  unsigned char x,y,z;
  int times = 0;
  for(z = 0; z < cnt; z++){
    while(times < tv){
      
      for(y = 0;y < 8;y++){
        for(x = 0; x < 8; x++){
          hc595(po[z][y][x]);
        }
        hc595out();
        cen_on(y);
        usleep(100);
        cen_on(8);
      }
      
      times++;
	  }
  }
}

static void sigint_handler(int sig)
{
    cen_on(8);
    close(hc595_dat);
    close(hc595_st);
    close(hc595_sh);
    for (int i = 0 ;i < 8; i++){
        close(gpio_layer[i]);
    }
    cout << "--- quit the loop! ---" << endl;
    exit(0);
}

int main(int argc, char *argv[])
{
    setup_gpio(0);
    setup_gpio(1);
    setup_gpio(131);
    setup_gpio(132);
    setup_gpio(133);
    setup_gpio(134);//error
    setup_gpio(135);
    setup_gpio(136);
    setup_gpio(137);
    setup_gpio(138);
    setup_gpio(139);
    setup_gpio(140);    

    open_gpio(&hc595_sh, 0);
    open_gpio(&hc595_st, 131);
    open_gpio(&hc595_dat, 132);

    open_gpio(&gpio_layer[0], 133);//layer1
    open_gpio(&gpio_layer[1], 1);//layer2
    open_gpio(&gpio_layer[2], 135);//layer3
    open_gpio(&gpio_layer[3], 136);//layer4
    open_gpio(&gpio_layer[4], 137);//layer5
    open_gpio(&gpio_layer[5], 138);//layer6
    open_gpio(&gpio_layer[6], 139);//layer7
    open_gpio(&gpio_layer[7], 140);//layer8

    signal(SIGINT, sigint_handler);//信号处理

    while(1){
        //display(1000,0xFF);
        //general( test, 1, 1000);
        general( king, 8, 200);
        
        usleep(100);        
    }
    close(hc595_dat);
    close(hc595_st);
    close(hc595_sh);
    for (int i = 0 ;i < 8; i++){
        close(gpio_layer[i]);
    }
}